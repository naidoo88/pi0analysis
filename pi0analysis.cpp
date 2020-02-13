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
  bool DEBUG = 0;
  cout << "Start" << endl;
  TFile *Out_File = new TFile(outfilename, "recreate");
  ifstream list_of_files;
  char file_name[200];
  char last_file[200];

  list_of_files.open(in_list);
  cout << "list of files opened" << endl;

  int n_files = 0;
  n_events         = 0;
  n_excl_events    = 0;
  n_postcut_events = 0;
  n_FTreg_flags    = 0;
  n_FTCAL_flags    = 0;

  //  histos();
  TTree *data = new TTree("data", "Processed Data.");
     data->Branch("Q2",   &Q2,   "Q2/D");
     data->Branch("tneg", &tneg, "tneg/D");
     data->Branch("W",    &W,    "W/D");
     data->Branch("xB",   &xB ,  "xB/D");

     data->Branch("pi0thetadiff", &pi0thetadiff, "pi0thetadiff/D");
     data->Branch("pi0anglediff", &pi0anglediff, "pi0anglediff/D");

     data->Branch("pi0im",  &pi0im,  "pi0im/D");
     data->Branch("pi0mm2", &pi0mm2, "pi0mm2/D");
     data->Branch("pi0mp",  &pi0mp,  "pi0mp/D");

     data->Branch("recprotmm",  &recprotmm,  "recprotmm/D");
     data->Branch("recprotmp",  &recprotmp,  "recprotmp/D");
     data->Branch("specneutmp", &specneutmp, "specneutmp/D");
     data->Branch("specneutmm", &specneutmm, "specneutmm/D");

     data->Branch("flag_excl",         &flag_excl,         "flag_excl/O");
     data->Branch("flag_pi0thetadiff", &flag_pi0thetadiff, "flag_pi0thetadiff/O");
     data->Branch("flag_pi0mm2",       &flag_pi0mm2,       "flag_pi0mm2/O");
     data->Branch("flag_spectneutmp",  &flag_spectneutmp,  "flag_spectneutmp/O");

     data->Branch("flag_photon1_FT",    &flag_photon1_FT,    "flag_photon1_FT/O");
     data->Branch("flag_photon1_ft",    &flag_photon1_ft,    "flag_photon1_ft/O");
     data->Branch("flag_photon1_CAL",   &flag_photon1_CAL,   "flag_photon1_CAL/O");
     data->Branch("flag_photon1_wCAL",  &flag_photon1_wCAL,  "flag_photon1_wCAL/I");
     data->Branch("flag_photon2_FT",    &flag_photon2_FT,    "flag_photon2_FT/O");
     data->Branch("flag_photon2_ft",    &flag_photon2_ft,    "flag_photon2_ft/O");
     data->Branch("flag_photon2_CAL",   &flag_photon2_CAL,   "flag_photon2_CAL/O");
     data->Branch("flag_photon2_wCAL",  &flag_photon2_wCAL,  "flag_photon2_wCAL/I");

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
        //double neutm = db -> GetParticle(2112)-> Mass();

        //double deutm = db -> GetParticle(1000010020) -> Mass();
        TLorentzVector deut (0,0,0,1.876);

        cout << "BEFORE c12.next()" << endl;

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

          int N = photonbuff.size();
          if(DEBUG) cout << "PHOTON BUFF SIZE: " << N << endl;

          if(N == 2){
            if(DEBUG) cout << "ONE PAIR" << endl;
            phot1.SetXYZM(photonbuff[0]->par()->getPx(),
                          photonbuff[0]->par()->getPy(),
                          photonbuff[0]->par()->getPz(),
                          0);
            phot2.SetXYZM(photonbuff[1]->par()->getPx(),
                          photonbuff[1]->par()->getPy(),
                          photonbuff[1]->par()->getPz(),
                          0);

            //set photon detector flags
              if(photonbuff[0]->getRegion() ==1000){
                flag_photon1_FT  = 1;
                flag_photon1_CAL = 0;
                n_FTreg_flags++;
              }
              else if(photonbuff[0]->getRegion()==2000) {
                flag_photon1_FT  = 0;
                flag_photon1_CAL = 1;
              }
              if(photonbuff[0]->ft(FTCAL)->getDetector()==10){
                flag_photon1_ft = 1;
                n_FTCAL_flags++;
              }
              else flag_photon1_ft = 0;
              //cout << "FT out: " << photonbuff[0]->getRegion() << " FT flag: " << flag_photon1_FT <<"  FTCAL out: " << photonbuff[0]->ft(FTCAL)->getDetector() << " FTCAL flag: " << flag_photon1_ft << endl;

              if(photonbuff[0]->cal(PCAL)->getDetector()==7)  flag_photon1_wCAL=1;
              if(photonbuff[0]->cal(ECIN)->getDetector()==7)  flag_photon1_wCAL=2;
              if(photonbuff[0]->cal(ECOUT)->getDetector()==7) flag_photon1_wCAL=3;

              if(photonbuff[1]->getRegion() ==1000){
                flag_photon2_FT  = 1;
                flag_photon2_CAL = 0;
              }
              else if(photonbuff[1]->getRegion()==2000) {
                flag_photon2_FT  = 0;
                flag_photon2_CAL = 1;
              }
              if(photonbuff[1]->ft(FTCAL)->getDetector()==10) flag_photon2_ft = 1;

              if(photonbuff[1]->cal(PCAL)->getDetector()==7)  flag_photon2_wCAL=1;
              if(photonbuff[1]->cal(ECIN)->getDetector()==7)  flag_photon2_wCAL=2;
              if(photonbuff[1]->cal(ECOUT)->getDetector()==7) flag_photon2_wCAL=3;
          }//if one pair

          else{                           //loop through combinations, keeping pair closest to pi0-mass
            double diff, mindiff = 10000; //mindiff initialised high
            TLorentzVector phot1buff, phot2buff, combobuff;
            int n_pairs = 0;

            for (int i=0; i<N-1; i++){
              for (int j=i+1; j<N; j++){
                if(DEBUG) cout << "pair [" << i << "][" << j << "]" << endl;
                phot1buff.SetXYZM(photonbuff[i]->par()->getPx(),
                                  photonbuff[i]->par()->getPy(),
                                  photonbuff[i]->par()->getPz(),
                                  0);
                phot2buff.SetXYZM(photonbuff[j]->par()->getPx(),
                                  photonbuff[j]->par()->getPy(),
                                  photonbuff[j]->par()->getPz(),
                                  0);

                combobuff = phot1buff + phot2buff;
                diff = abs(pi0m - combobuff.M());

                if (diff < mindiff){
                  phot1.SetXYZM(photonbuff[i]->par()->getPx(),
                                    photonbuff[i]->par()->getPy(),
                                    photonbuff[i]->par()->getPz(),
                                    0);
                  phot2.SetXYZM(photonbuff[j]->par()->getPx(),
                                    photonbuff[j]->par()->getPy(),
                                    photonbuff[j]->par()->getPz(),
                                    0);

                  mindiff = diff;

                  //set photon detector flags
                    if(photonbuff[i]->getRegion() ==1000){
                      flag_photon1_FT  = 1;
                      flag_photon1_CAL = 0;
                    }
                    else if(photonbuff[i]->getRegion()==2000) {
                      flag_photon1_FT  = 0;
                      flag_photon1_CAL = 1;
                    }
                    if(photonbuff[i]->cal(PCAL) ->getDetector()==7) flag_photon1_wCAL=1;
                    if(photonbuff[i]->cal(ECIN) ->getDetector()==7) flag_photon1_wCAL=2;
                    if(photonbuff[i]->cal(ECOUT)->getDetector()==7) flag_photon1_wCAL=3;

                    if(photonbuff[j]->getRegion() ==1000){
                      flag_photon2_FT  = 1;
                      flag_photon2_CAL = 0;
                    }
                    else if(photonbuff[j]->getRegion()==2000) {
                      flag_photon2_FT  = 0;
                      flag_photon2_CAL = 1;
                    }
                    if(photonbuff[j]->cal(PCAL) ->getDetector()==7) flag_photon2_wCAL=1;
                    if(photonbuff[j]->cal(ECIN) ->getDetector()==7) flag_photon2_wCAL=2;
                    if(photonbuff[j]->cal(ECOUT)->getDetector()==7) flag_photon2_wCAL=3;

                  if(DEBUG) cout << "new pair is ["<< i << "][" << j << "]" << endl;
                }//masscheck
                n_pairs++;
              }//photons j-loop
            }//photons i-loop
            if(DEBUG) cout << "LOOPED THROUGH " << n_pairs << " PAIRS.\n" << endl;
          }//close pair else

          /*=====KINEMATIC CUTS=====*/
          //Q2   = -1*(beam - e).M2();
          //if(Q2 < 1)   continue; // could be 1.5, 2... revisit at asymmetry stage
          //tneg = -(prot-target).M2();
          //if(tneg > 1) continue; //**DOUBLE CHECK THIS AGAINST THEORY PAPER**
          //W    = (e + prot + phot1 + phot2).M2();
          //if(W  < 4)   continue;
          /*========================*/

          Q2   = -1*(beam - e).M2();
          xB   = Q2 / 2*(target*(beam - e));
          tneg = -(prot-target).M2();
          W    = (e + prot + phot1 + phot2).M2();

          TLorentzVector expected_pi0 = target+beam-e-prot;
          TLorentzVector reconstr_pi0 = phot1+phot2;
          pi0thetadiff = TMath::RadToDeg()*((target+beam-e-prot).Theta() - (phot1+phot2).Theta());
          pi0anglediff = TMath::RadToDeg()*(expected_pi0.Angle(reconstr_pi0.Vect()));

          TLorentzVector system = (beam+target)-(e+prot+phot1+phot2); //[e p -> e' p' g1 g2]
          TLorentzVector photcombo = phot1 + phot2;
          pi0im  = photcombo.M();
          pi0mm2 = system.M2();
          pi0mp  = system.P();

          TLorentzVector recprot     = (beam+target)-(e+phot1+phot2);
          TLorentzVector recspecneut = (beam+deut)-(e+prot+phot1+phot2);
          recprotmm  = recprot.M();
          recprotmp =  recprot.P();
          specneutmp = recspecneut.P();
          specneutmm = recspecneut.M();

          if((Q2 > 1) && (tneg < 1) && (W  > 4)){ //Set flag for pass/fail on exclusivity cuts.
            flag_excl = 1;
            n_excl_events++;
          }
          else flag_excl = 0;

          data->Fill();


          // pi0thetadiff_h -> Fill(pi0thetadiff);

          // /*=====PRE-CUT HISTOS=====*/
          // pi0im_h[0]  -> Fill(pi0im);
          // pi0mm2_h[0] -> Fill(pi0mm2);
          // pi0mp_h[0]  -> Fill(pi0mp);
          // Q2_h[0]     -> Fill(Q2);
          // xB_h[0]     -> Fill(xB);
          // Q2xB_h[0]   -> Fill(xB, Q2);
          // tneg_h[0]   -> Fill(tneg);
          // W_h[0]      -> Fill(W);

          // pi0_immm2_h[0] -> Fill(pi0mm2, pi0im);
          // pi0_immp_h[0]  -> Fill(pi0mp, pi0im);
          // pi0_mm2mp_h[0] -> Fill(pi0mp, pi0mm2);

          // recprotmm_h[0]      -> Fill(recprotmm);
          // spectneutmm_h[0]    -> Fill(specneutmm);
          // spectneutmp_h[0]    -> Fill(specneutmp);
          // spectneut_mpmm_h[0] -> Fill(specneutmm, specneutmp);
          // /*========================*/

          if(pi0mm2 < -0.0853 || pi0mm2 > 0.0718){
            flag_pi0mm2 = 0;
            continue; //mu = -0.006743, sig = 0.02618
          }
          flag_pi0mm2 = 1; //cut passed

          // /*=====POST-pi0MM2-CUT HISTOS=====*/
          // pi0im_h[1]  -> Fill(pi0im);
          // pi0mm2_h[1] -> Fill(pi0mm2);
          // pi0mp_h[1]  -> Fill(pi0mp);
          // Q2_h[1]     -> Fill(Q2);
          // xB_h[1]     -> Fill(xB);
          // Q2xB_h[1]   -> Fill(xB, Q2);
          // tneg_h[1]   -> Fill(tneg);
          // W_h[1]      -> Fill(W);

          // pi0_immm2_h[1] -> Fill(pi0mm2, pi0im);
          // pi0_immp_h[1]  -> Fill(pi0mp, pi0im);
          // pi0_mm2mp_h[1] -> Fill(pi0mp, pi0mm2);

          // recprotmm_h[1]      -> Fill(recprotmm);
          // spectneutmm_h[1]    -> Fill(specneutmm);
          // spectneutmp_h[1]    -> Fill(specneutmp);
          // spectneut_mpmm_h[1] -> Fill(specneutmm, specneutmp);
          // /*================================*/

          if(specneutmp > 0.3){
            continue; //300MeV cut on spectator missing momentum
            flag_spectneutmp = 0;
          }
          flag_spectneutmp = 1;

          // /*=====POST-spectMP-CUT HISTOS=====*/
          // pi0im_h[2]  -> Fill(pi0im);
          // pi0mm2_h[2] -> Fill(pi0mm2);
          // pi0mp_h[2]  -> Fill(pi0mp);
          // Q2_h[2]     -> Fill(Q2);
          // xB_h[2]     -> Fill(xB);
          // Q2xB_h[2]   -> Fill(xB, Q2);
          // tneg_h[2]   -> Fill(tneg);
          // W_h[2]      -> Fill(W);

          // pi0_immm2_h[2] -> Fill(pi0mm2, pi0im);
          // pi0_immp_h[2]  -> Fill(pi0mp, pi0im);
          // pi0_mm2mp_h[2] -> Fill(pi0mp, pi0mm2);

          // recprotmm_h[2]      -> Fill(recprotmm);
          // spectneutmm_h[2]    -> Fill(specneutmm);
          // spectneutmp_h[2]    -> Fill(specneutmp);
          // spectneut_mpmm_h[2] -> Fill(specneutmm, specneutmp);
          // /*================================*/

          n_postcut_events++;
          //if (n_events == 10) break;        }//event while-loop

        }//event loop
      }//duplicate check
      sprintf(last_file,"%s",file_name);     // save name of the current file into "last_file"
      n_files++;
    }//file list loop
  }//list open-check

  list_of_files.close();
  cout << "\nTotal no of files in list: " << n_files << endl;
  cout << "\nTotal no of events processed: " << n_events << endl;
  cout << "\nTotal no of exclusive events: " << n_excl_events << endl;
  cout << "\nTotal no of events post-cuts: " << n_postcut_events << endl;
  cout << "\nNumber of photons in FT reg: "  << n_FTreg_flags << endl;
  cout << "\nNumber of photons in FTCAL : "  << n_FTCAL_flags << endl;

  Out_File->Write();
  Out_File->Close();

}//pi0analysis fxn

