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

  DEG = TMath::RadToDeg(); //rad->deg conversion

  list_of_files.open(in_list);
  cout << "list of files opened" << endl;

  int n_files = 0;
  int n_pairs = 0;
  n_events          = 0;
  n_dis_events      = 0;
  n_excl_events     = 0;
  n_postcut_events  = 0;
  n_ECAL_doublehits = 0;

  //##############################################################################
  // Set up Tree/Branches
  //##############################################################################

  TTree *data = new TTree("data", "Processed Data.");
     data->Branch("Q2",   &Q2,   "Q2/D");
     data->Branch("tneg", &tneg, "tneg/D");
     data->Branch("W2",   &W2,   "W2/D");
     data->Branch("xB",   &xB ,  "xB/D");

     data->Branch("n_photons_in_event",   &n_photons_in_event,   "n_photons_in_event/I");
     data->Branch("n_goodpi0_candidates", &n_goodpi0_candidates, "n_goodpi0_candidates/I");


     data->Branch("IM_g1g2",   &IM_g1g2,   "IM_g1g2/D");
     data->Branch("MM2_total", &MM2_total, "MM2_total/D");
     data->Branch("MM2_total", &MM2_total, "MM2_total/D");

     data->Branch("MM_rec_recoil",    &MM_rec_recoil,    "MM_rec_recoil/D");
     data->Branch("MM_rec_recoil",    &MM_rec_recoil,    "MM_rec_recoil/D");
     data->Branch("MP_rec_spectator", &MP_rec_spectator, "MP_rec_spectator/D");
     data->Branch("MM_rec_spectator", &MM_rec_spectator, "MM_rec_spectator/D");

     data->Branch("pi0coneangle",   &pi0coneangle,   "pi0coneangle/D");
     data->Branch("phi_Nvg",        &phi_Nvg,        "phi_Nvg/D");
     data->Branch("phi_Nnew",       &phi_Nnew,       "phi_Nnew/D");
     data->Branch("phi_vgnew",      &phi_vgnew,      "phi_vgnew/D");
     data->Branch("cop_Nvg_vgnew",  &cop_Nvg_vgnew,  "cop_Nvg_vgnew/D");
     data->Branch("cop_Nvg_Nnew",   &cop_Nvg_Nnew,   "cop_Nvg_Nnew/D");
     data->Branch("cop_Nnew_vgnew", &cop_Nnew_vgnew, "cop_Nnew_vgnew/D");

     data->Branch("flag_cuts_dis",  &flag_cuts_dis,  "flag_cuts_dis/O");
     data->Branch("flag_cuts_excl", &flag_cuts_excl, "flag_cuts_excl/O");
     data->Branch("flag_goodpi0",   &flag_goodpi0,   "flag_goodpi0/O");
     // data->Branch("flag_MM2_total",       &flag_MM2_total,       "flag_MM2_total/O");
     // data->Branch("flag_spectneutmp",  &flag_spectneutmp,  "flag_spectneutmp/O");

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

          TLorentzVector system;        //[e N -> e' R g1 g2]
          TLorentzVector photcombo;
          TLorentzVector rec_recoil;    //[e p -> e' g1 g2]
          TLorentzVector rec_spectator; //[e D -> e' R g1 g2]
          TLorentzVector expected_pi0;

          int n_pairs              = 0;
          int n_good_pi0candidates = 0;
          n_photons_in_event       = photonbuff.size();

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
              photcombo = phot1 + phot2;

              system = (beam+target)-(e+prot+phot1+phot2); //[e p -> e' p' g1 g2]
              IM_g1g2  = photcombo.M();
              MM2_total = system.M2();
              MP_total  = system.P();

              rec_recoil     = (beam+target)-(e+phot1+phot2); //reconstruct recoil
              expected_pi0 = target+beam-e-prot;
              pi0coneangle = (expected_pi0.Angle(photcombo.Vect()))*DEG;

              rec_spectator = (beam+deut)-(e+prot+phot1+phot2); //reconstruct spectator
              MM_rec_recoil  = rec_recoil.M();
              MM_rec_recoil =  rec_recoil.P();
              MP_rec_spectator = rec_spectator.P();
              MM_rec_spectator = rec_spectator.M();

              //Calc trento-phi and co-planarity angles:
              calc_angles(beam.Vect(), e.Vect(), prot.Vect(), photcombo.Vect());

              //Set Photon flags:
              photonflags(photonbuff[i], photonbuff[j], n_ECAL_doublehits);

              //Set "good"-pi0 event flag
              /*
              // [IM_gg fitted (per detector) with MM2 +/- 0.5GeV && pi0coneangle < 20deg]
              // -- 3 sigma cut gives the following:
              // ---- both PCAL: Mean: 0.131183  Sig: 0.0132358 => Lower: 0.091476   Upper: 0.170891
              // ---- both ECAL: Mean: 0.134668  Sig: 0.0110543 => Lower: 0.101505   Upper: 0.167831
              // ---- both FCAL: Mean: 0.131060  Sig: 0.0060639 => Lower: 0.112868   Upper: 0.149252
              // ---- PCAL/ECAL: Mean: 0.132082  Sig: 0.0127805 => Lower: 0.0937409  Upper: 0.170424
              // ---- ECAL/FCAL: Mean: 0.127126  Sig: 0.0107954 => Lower: 0.0947396  Upper: 0.159512
              // ---- PCAL/FCAL: Mean: 0.126721  Sig: 0.0119468 => Lower: 0.0908801  Upper: 0.162561
              */

              if ((flag_photon1_PCAL==1 && flag_photon2_PCAL==1) && (IM_g1g2 > 0.091476 && IM_g1g2 < 0.170891)) {
                flag_goodpi0 = 1;
                n_goodpi0_candidates++;
              }
              else if ((flag_photon1_ECAL==1 && flag_photon2_ECAL==1) && (IM_g1g2 > 0.101505 && IM_g1g2 < 0.167831)){
                flag_goodpi0 = 1;
                n_goodpi0_candidates++;
              }
              else if ((flag_photon1_ft==1 && flag_photon2_ft==1) && (IM_g1g2 > 0.112868 && IM_g1g2 < 0.149252)){
                flag_goodpi0 = 1;
                n_goodpi0_candidates++;
              }
              else if (((flag_photon1_PCAL==1 && flag_photon2_ECAL==1)||(flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) && (IM_g1g2 > 0.0937409 && IM_g1g2 < 0.170424)){
                flag_goodpi0 = 1;
                n_goodpi0_candidates++;
              }
              else if (((flag_photon1_ECAL==1 && flag_photon2_ft==1)||(flag_photon2_ECAL==1 && flag_photon1_ft==1)) && (IM_g1g2 > 0.0947396 && IM_g1g2 < 0.159512)){
                flag_goodpi0 = 1;
                n_goodpi0_candidates++;
              }
              else if (((flag_photon1_PCAL==1 && flag_photon2_ft==1)||(flag_photon2_PCAL==1 && flag_photon1_ft==1)) && (IM_g1g2 > 0.0908801 && IM_g1g2 < 0.162561)){
                flag_goodpi0 = 1;
                n_goodpi0_candidates++;
              }
              else flag_goodpi0 = 0;


              //###################################################################################
              // Set Cut Flags -- DIS cuts, excl' cuts.
              //###################################################################################

              /*======= DIS CUTS =======*/
              Q2   = -1*(beam - e).M2();
              xB   = Q2 / 2*(target*(beam - e));
              tneg = -(prot-target).M2(); //**DOUBLE CHECK THIS AGAINST THEORY PAPER**
              W2    = (e + prot + phot1 + phot2).M2();

              if((Q2 > 1) && (tneg < 1) && (W2  > 4)){
                flag_cuts_dis = 1;
                n_dis_events++;
                //n_pairs_passed_dis++;
              }
              else flag_cuts_dis = 0;

              // if(MM2_total < -0.0853 || MM2_total > 0.0718){//mu = -0.006743, sig = 0.02618
              //   flag_MM2_total = 0;
              // }
              // else flag_MM2_total = 1; //cut passed

              // if(MP_rec_spectator > 0.3){//300MeV cut on spectator missing momentum
              //   flag_spectneutmp = 0;
              // }
              // else flag_spectneutmp = 1;
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


void calc_angles(TVector3 Ebeam_vect, TVector3 Electron_vect, TVector3 Recoil_vect, TVector3 Newpart_vect){

   //TVector3 Ebeam_vect;          // vector of incoming beam electron
   TVector3 Virtual_photon;      // virtual photon vector
   TVector3 vect_ee;             // vector in the leptonic plane
   TVector3 vect_Nnew;           // three vectors in the hadronic plane
   TVector3 vect_Nvg;
   TVector3 vect_vgnew;

   Virtual_photon = Ebeam_vect - Electron_vect;        // vector of the virtual photon

   // Leptonic plane:
   vect_ee = Ebeam_vect.Cross(Electron_vect);          // cross product of e and e', gives normal to leptonic plane

   // Hadronic plane:
   vect_Nnew = Recoil_vect.Cross(Newpart_vect);        // normal to hadronic plane formed by recoiling nucleon and new particle (photon, meson)
   vect_Nvg = Recoil_vect.Cross(Virtual_photon);       // normal to hadronic plane formed by recoiling nucleon and virtual photon
   vect_vgnew = Virtual_photon.Cross(Newpart_vect);    // gives normal to hadronic plane formed by virtual photon and new particle (photon, meson)

   // Phi angles (between leptonic and hadronic planes):
   // Phi convention here:
      //- e from bottom left to center, e' from center to top left (so vect_ee is up out of page)
      //- gamma* from center left to center right
      //- N' from center to bottom right, new particle (gamma or pi0) from center to top right (so vect_Nnew, vect_Nvg, vect_vgnew all up out of page)
      //
      //- phi is 0 when both leptopnic and hadronic planes are parallel
      //- phi increases as you rotate hadronic plane clockwise, if looking along trajectory of virtual photon
      //- phi is 180 deg when the two planes are again parallel but the N' and the new particle (photon or meson) have swapped directions
      //- phi continues to increase to 360 deg as you continue to rotate hadronic plane clockwise, from point of view of gamma*

   phi_Nvg = (vect_ee.Angle(vect_Nvg))*DEG;            // angle between (e,e') and (N,gamma*) planes, in degrees.
   if ((vect_ee.Dot(Recoil_vect)) > 0.) phi_Nvg = 360. - phi_Nvg;  // To make sure it runs from 0 - 360 degrees.

   phi_Nnew = (vect_ee.Angle(vect_Nnew))*DEG;          // angle between (e,e') and (N,gamma) or (N,pi0) planes, in degrees.
   if ((vect_ee.Dot(Newpart_vect)) < 0.) phi_Nnew = 360. - phi_Nnew;   // Note: the pi0 or gamma are called "New" for simplicity.

   phi_vgnew = (vect_ee.Angle(vect_vgnew))*DEG;        // angle between (e,e') and (gamma*,gamma) or (gamma*,pi0) planes, in degrees.
   if ((vect_ee.Dot(Newpart_vect)) < 0.) phi_vgnew = 360. - phi_vgnew; // Note: the pi0 or gamma are called "New" for simplicity.


   // Coplanarity angles (to make sure the recoil nucleon, the virtual photon and the photon/meson are in the same plane):
   // In the naming convention, "new" refers to produced gamma or pi0.

   cop_Nvg_vgnew = (vect_Nvg.Angle(vect_vgnew))*DEG;     // angle between (N,gamma*) and (gamma*,gamma) or (gamma*,pi0) planes, in degrees.
   if ((vect_Nvg.Dot(Newpart_vect)) < 0) cop_Nvg_vgnew = -1*cop_Nvg_vgnew;   // sort of arbitrary, but makes sure there's a nice peak at zero

   cop_Nvg_Nnew = (vect_Nvg.Angle(vect_Nnew))*DEG;       // angle between (N,gamma*) and (N,gamma) or (N,pi0) planes, in degrees.
   if ((vect_Nvg.Dot(Newpart_vect)) < 0) cop_Nvg_Nnew = -1*cop_Nvg_Nnew;   // sort of arbitrary, but makes sure there's a nice peak at zero

   cop_Nnew_vgnew = (vect_Nnew.Angle(vect_vgnew))*DEG;       // angle between (N,gamma) and (gamma*,gamma) or between (N,pi0) and (gamma*,pi0) planes.
   if ((vect_vgnew.Dot(Recoil_vect)) < 0) cop_Nnew_vgnew = -1*cop_Nnew_vgnew;   // sort of arbitrary, but makes sure there's a nice peak at zero

}//calc_angles fxn
