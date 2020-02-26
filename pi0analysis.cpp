#include <cstdlib>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include "clas12reader.h"
#include "pi0analysis.h"


using namespace clas12;

void pi0analysis(const Char_t in_list[], const TString outfilename){
  TStopwatch time;
  time.Start();

  bool DEBUG = 0;
  cout << "Start" << endl;
  TFile *Out_File = new TFile(outfilename, "recreate");
  ifstream list_of_files;
  char file_name[200];
  char last_file[200];

  list_of_files.open(in_list);
  cout << "list of files opened" << endl;

  int n_files = 0;
  int n_pairs = 0;
  n_events          = 0;
  n_excl_events     = 0;
  n_postcut_events  = 0;
  n_ECAL_doublehits = 0;

  //##############################################################################
  // Set up Tree/Branches
  //##############################################################################

  TTree *data = new TTree("data", "Processed Data.");
     data->Branch("Q2",   &Q2,   "Q2/D");
     data->Branch("tneg", &tneg, "tneg/D");
     data->Branch("W",    &W,    "W/D");
     data->Branch("xB",   &xB ,  "xB/D");

     data->Branch("n_photons_in_event", &n_photons_in_event, "n_photons_in_event/I");
     data->Branch("pi0im",  &pi0im,  "pi0im/D");
     data->Branch("pi0mm2", &pi0mm2, "pi0mm2/D");
     data->Branch("pi0mp",  &pi0mp,  "pi0mp/D");

     data->Branch("pi0coneangle", &pi0coneangle, "pi0coneangle/D");
     data->Branch("recprotmm",  &recprotmm,  "recprotmm/D");
     data->Branch("recprotmp",  &recprotmp,  "recprotmp/D");
     data->Branch("specneutmp", &specneutmp, "specneutmp/D");
     data->Branch("specneutmm", &specneutmm, "specneutmm/D");

     data->Branch("flag_excl",         &flag_excl,         "flag_excl/O");
     data->Branch("flag_pi0mm2",       &flag_pi0mm2,       "flag_pi0mm2/O");
     data->Branch("flag_spectneutmp",  &flag_spectneutmp,  "flag_spectneutmp/O");

     data->Branch("flag_photon1_ft",   &flag_photon1_ft,   "flag_photon1_ft/O");
     data->Branch("flag_photon1_PCAL", &flag_photon1_PCAL, "flag_photon1_PCAL/O");
     data->Branch("flag_photon1_ECAL", &flag_photon1_ECAL, "flag_photon1_ECAL/O");
     data->Branch("flag_photon1_EIN",  &flag_photon1_EIN,  "flag_photon1_EIN/O");
     data->Branch("flag_photon1_EOUT", &flag_photon1_EOUT, "flag_photon1_EOUT/O");
     data->Branch("flag_photon1_wCAL", &flag_photon1_wCAL, "flag_photon1_wCAL/I");
     data->Branch("flag_photon2_ft",   &flag_photon2_ft,   "flag_photon2_ft/O");
     data->Branch("flag_photon2_PCAL", &flag_photon2_PCAL, "flag_photon2_PCAL/O");
     data->Branch("flag_photon2_ECAL", &flag_photon2_ECAL, "flag_photon2_ECAL/O");
     data->Branch("flag_photon2_EIN",  &flag_photon2_EIN,  "flag_photon2_EIN/O");
     data->Branch("flag_photon2_EOUT", &flag_photon2_EOUT, "flag_photon2_EOUT/O");
     data->Branch("flag_photon2_wCAL", &flag_photon2_wCAL, "flag_photon2_wCAL/I");

  //##############################################################################
  if(list_of_files.is_open()){
    cout << "Successfully opened list:  " << in_list << endl;

    while(!list_of_files.eof()) {
      if (!list_of_files.good()) break;

      list_of_files >> file_name;

      if (strcmp(last_file,file_name) != 0){   // makes sure the last file doesn't get counted twice.

        cout << "File to be read in is: " << file_name << endl;

        std::string file_str(file_name);  //string for clas12reader
        clas12reader c12(file_str);       //read hipo file

        c12.addExactPid(2212,1);          //one proton
        c12.addExactPid(11,1);            //one electron
        c12.addAtLeastPid(22,2);          //at least 2 photons
        c12.addZeroOfRestPid();           //no other particles

        auto db=TDatabasePDG::Instance();
        TLorentzVector beam   (0,0,10.6,10.6);
        TLorentzVector target (0,0,0,db->GetParticle(2212)->Mass());
        TLorentzVector e      (0,0,0,0);
        TLorentzVector prot   (0,0,0,0);
        TLorentzVector phot1  (0,0,0,0);
        TLorentzVector phot2  (0,0,0,0);
        TLorentzVector q      (0,0,0,0);
        TLorentzVector mm     (0,0,0,0);

        double em    = db -> GetParticle(11)  -> Mass();
        double protm = db -> GetParticle(2212)-> Mass();
        double pi0m  = db -> GetParticle(111) -> Mass();
        double neutm = db -> GetParticle(2112)-> Mass();
        //double deutm = db -> GetParticle(1000010020) -> Mass();
        TLorentzVector deut (0,0,0,1.876);

        while(c12.next()) {   //loop over events
          n_events++;
          auto electronbuff = c12.getByID(11);
          auto photonbuff   = c12.getByID(22);
          auto protonbuff   = c12.getByID(2212);

          prot.SetXYZM(protonbuff[0]->par()->getPx(),
                       protonbuff[0]->par()->getPy(),
                       protonbuff[0]->par()->getPz(),
                       protm);

          e.SetXYZM(electronbuff[0]->par()->getPx(),
                    electronbuff[0]->par()->getPy(),
                    electronbuff[0]->par()->getPz(),
                    em);

          int n_pairs = 0;
          TLorentzVector system;  //[e p -> e' p' g1 g2]
          TLorentzVector photcombo;
          TLorentzVector recprot;
          TLorentzVector recspecneut;
          TLorentzVector expected_pi0;

          n_photons_in_event = photonbuff.size();

          for (int i=0; i<n_photons_in_event-1; i++){
            for (int j=i+1; j<n_photons_in_event; j++){
              if(DEBUG) cout << "pair [" << i << "][" << j << "]" << endl;
              phot1.SetXYZM(photonbuff[i]->par()->getPx(),
                            photonbuff[i]->par()->getPy(),
                            photonbuff[i]->par()->getPz(),
                            0);
              phot2.SetXYZM(photonbuff[j]->par()->getPx(),
                            photonbuff[j]->par()->getPy(),
                            photonbuff[j]->par()->getPz(),
                            0);

              //Set Photon flags:
              photonflags(photonbuff[i], photonbuff[j], n_ECAL_doublehits);

              Q2   = -1*(beam - e).M2();
              xB   = Q2 / 2*(target*(beam - e));
              tneg = -(prot-target).M2();
              W    = (e + prot + phot1 + phot2).M2();

              /*======= DIS CUTS =======*/
              //Q2   = -1*(beam - e).M2();
              //if(Q2 < 1)   continue; // could be 1.5, 2... revisit at asymmetry stage
              //tneg = -(prot-target).M2();
              //if(tneg > 1) continue; //**DOUBLE CHECK THIS AGAINST THEORY PAPER**
              //W    = (e + prot + phot1 + phot2).M2();
              //if(W  < 4)   continue;
              /*========================*/

              system = (beam+target)-(e+prot+phot1+phot2); //[e p -> e' p' g1 g2]
              photcombo = phot1 + phot2;
              pi0im  = photcombo.M();
              pi0mm2 = system.M2();
              pi0mp  = system.P();

              recprot     = (beam+target)-(e+phot1+phot2);
              expected_pi0 = target+beam-e-prot;
              pi0coneangle = TMath::RadToDeg()*(expected_pi0.Angle(photcombo.Vect()));

              recspecneut = (beam+deut)-(e+prot+phot1+phot2);
              recprotmm  = recprot.M();
              recprotmp =  recprot.P();
              specneutmp = recspecneut.P();
              specneutmm = recspecneut.M();

              //###################################################################################
              // Set Flags -- DIS cuts, mass/mom' cuts.
              //###################################################################################
              if((Q2 > 1) && (tneg < 1) && (W  > 4)){ //Set flag for pass/fail on exclusivity cuts.
                flag_excl = 1;
                n_excl_events++;
              }
              else flag_excl = 0;

              if(pi0mm2 < -0.0853 || pi0mm2 > 0.0718){//mu = -0.006743, sig = 0.02618
                flag_pi0mm2 = 0;
              }
              else flag_pi0mm2 = 1; //cut passed

              if(specneutmp > 0.3){//300MeV cut on spectator missing momentum
                flag_spectneutmp = 0;
              }
              else flag_spectneutmp = 1;
              //###################################################################################

              n_pairs++;

              data->Fill();
            }//photons j-loop
          }//photons i-loop
          if(DEBUG) cout << "LOOPED THROUGH " << n_pairs << " PAIRS.\n" << endl;


         //SUBSTITUTE THESE LINES TO PROCESS N-EVENTS
         //if (n_events == 10) break;        }//event loop
        }//event loop
      }//duplicate check
      sprintf(last_file,"%s",file_name);     // save name of the current file into "last_file"
      n_files++;
    }//file list loop
  }//list open-check

  list_of_files.close();
  cout << "\nTotal no of files in list: " << n_files << endl;
  cout << "\nTotal no of events processed: " << n_events << endl;
  //cout << "\nTotal no of exclusive events: " << n_excl_events << endl;
  //cout << "\nTotal no of events post-cuts: " << n_postcut_events << endl;
  cout << "\n There were " << n_ECAL_doublehits << " photons which hit both EIN and EOUT in the ECAL." << endl;

  Out_File->Write();
  Out_File->Close();

  time.Stop();
  time.Print();
} //pi0analysis fxn


void photonflags(clas12::region_part_ptr p1, clas12::region_part_ptr p2, int &count_ECAL_doublehits){

  //first photon:
  if(p1->ft(FTCAL)->getDetector()==10) flag_photon1_ft = 1;
  else flag_photon1_ft = 0;

  if(p1->cal(PCAL) ->getDetector()==7) flag_photon1_PCAL = 1;
  else flag_photon1_PCAL = 0;

  if(p1->cal(ECIN) ->getDetector()==7) flag_photon1_EIN  = 1;
  else flag_photon1_EIN = 0;
  if(p1->cal(ECOUT)->getDetector()==7) flag_photon1_EOUT = 1;
  else flag_photon1_EOUT = 0;
  if(flag_photon1_EIN==1 || flag_photon1_EOUT==1) flag_photon1_ECAL=1;
  else flag_photon1_ECAL = 0;

  //second photon:
  if(p2->ft(FTCAL)->getDetector()==10) flag_photon2_ft = 1;
  else flag_photon2_ft = 0;

  if(p2->cal(PCAL) ->getDetector()==7) flag_photon2_PCAL = 1;
  else flag_photon2_PCAL = 0;

  if(p2->cal(ECIN) ->getDetector()==7) flag_photon2_EIN  = 1;
  else flag_photon2_EIN = 0;
  if(p2->cal(ECOUT)->getDetector()==7) flag_photon2_EOUT = 1;
  else flag_photon2_EOUT = 0;
  if(flag_photon2_EIN==1 || flag_photon2_EOUT==1) flag_photon2_ECAL=1;
  else flag_photon2_ECAL = 0;

  if(flag_photon1_EIN == 1 && flag_photon1_EOUT == 1) count_ECAL_doublehits++;
  if(flag_photon2_EIN == 1 && flag_photon2_EOUT == 1) count_ECAL_doublehits++;


}//photonflags fxn

//  LocalWords:  EIN EOUT PCAL ECAL wCAL
