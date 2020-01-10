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

void pi0analysis(const Char_t in_list[]){
  bool DEBUG = 0;
  cout << "Start" << endl;
  TFile *Out_File = new TFile("/scratch/pauln/out.root", "recreate");
  ifstream list_of_files;
  char file_name[200];
  char last_file[200];

  list_of_files.open(in_list);
  cout << "list of files opened" << endl;

  int n_files = 0;
  n_events    = 0;

  histos();

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

        double em    = db -> GetParticle(11)  ->Mass();
        double protm = db -> GetParticle(2212)->Mass();
        double pi0m  = db -> GetParticle(111) ->Mass();
        //double neutm = db -> GetParticle(2112)->Mass();

        while(c12.next()) {   //loop over events
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
                  if(DEBUG) cout << "new pair is ["<< i << "][" << j << "]" << endl;
                }//masscheck
                n_pairs++;
              }//photons j-loop
            }//photons i-loop
            if(DEBUG) cout << "LOOPED THROUGH " << n_pairs << " PAIRS.\n" << endl;
          }//close pair else

          /*=====KINEMATIC CUTS=====*/
          double   Q2 = -1*(beam - e).M2();
          if(Q2 < 1)   continue; // could be 1.5, 2... revisit at asymmetry stage

          double tneg = -(prot-target).M2();
          if(tneg > 1) continue; //**DOUBLE CHECK THIS AGAINST THEORY PAPER**

          double    W = (e + prot + phot1 + phot2).M2();
          if(W  < 4)   continue;
          /*========================*/

          TLorentzVector system = (beam+target)-(e+prot+phot1+phot2);
          TLorentzVector photcombo = phot1 + phot2;
          double pi0im  = photcombo.M();
          double pi0mm2 = system.M2();
          double pi0mp  = system.P();
          double     xB = Q2 / 2*(target*(beam - e));

          TLorentzVector recprot = (beam+target)-(e+phot1+phot2);
          double recprotmm = recprot.M();

          /*=====PRE-CUT HISTOS=====*/
          pi0im_pre_h  -> Fill(pi0im);
          pi0mm2_pre_h -> Fill(pi0mm2);
          pi0mp_pre_h  -> Fill(pi0mp);
          Q2_pre_h     -> Fill(Q2);
          xB_pre_h     -> Fill(xB);
          Q2xB_pre_h   -> Fill(xB, Q2);
          tneg_pre_h   -> Fill(tneg);
          W_pre_h      -> Fill(W);

          pi0im_pi0mm2_pre_h  -> Fill(pi0mm2, pi0im);
          pi0im_pi0mp_pre_h   -> Fill(pi0mp, pi0im);
          pi0mm2_pi0mp_pre_h  -> Fill(pi0mp, pi0mm2);

          recprotmm_pre_h -> Fill(recprotmm);
          /*========================*/

          if(pi0mm2 < -0.0853 || pi0mm2 > 0.0718) continue; //mu = -0.006743, sig = 0.02618

          /*=====POST-CUT HISTOS=====*/
          pi0im_h  -> Fill(pi0im);
          pi0mm2_h -> Fill(pi0mm2);
          pi0mp_h  -> Fill(pi0mp);
          Q2_h     -> Fill(Q2);
          xB_h     -> Fill(xB);
          Q2xB_h   -> Fill(xB, Q2);
          tneg_h   -> Fill(tneg);
          W_h      -> Fill(W);

          pi0im_pi0mm2_h   -> Fill(pi0mm2, pi0im);
          pi0im_pi0mp_h   -> Fill(pi0mp, pi0im);
          pi0mm2_pi0mp_h  -> Fill(pi0mp, pi0mm2);

          recprotmm_h -> Fill(recprotmm);
          /*========================*/

          n_events++;
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

  Out_File->Write();
  Out_File->Close();

}//pi0analysis fxn

 void histos(){
   pi0im_pre_h  = new TH1F("pi0im_pre_h", "Invariant mass of paired photons - Before MM^{2} cut; Inv.Mass (GeV); counts", 200, 0, 0.2);
   pi0im_h      = new TH1F("pi0im_h", "Invariant mass of paired photons; Inv.Mass (GeV); counts",                         200, 0, 0.2);
   pi0mm2_pre_h = new TH1F("pi0mm2_pre_h", "Missing Mass-Squared - Before MM^{2} cut; MissingMass-Sq (GeV^{2}); counts",  200, -.5, .5);
   pi0mm2_h     = new TH1F("pi0mm2_h", "Missing Mass-Squared; MissingMass-Sq (GeV^{2}); counts",                          200, -.5, .5);
   pi0mp_pre_h  = new TH1F("pi0mp_pre_h", "Missing Momentum - Before MM^{2} cut; MissingMomentum (GeV); counts",          200, 0, 6);
   pi0mp_h      = new TH1F("pi0mp_h", "Missing Momentum; MissingMomentum-Sq (GeV); counts",                               200, 0, 6);

   tneg_pre_h   = new TH1F("tneg_pre_h", "-t - Before MM^{2} cut; -t (GeV^{2}); counts",                 150, 0, 10);
   tneg_h       = new TH1F("tneg_h", "-t; -t (GeV^{2}); counts",                                         150, 0, 10);
   Q2_pre_h     = new TH1F("Q2_pre_h", "Q^{2} - Before MM^{2} cut; Q^{2} (GeV^{2}); counts",             150, 0, 8);
   Q2_h         = new TH1F("Q2_h", "Q^{2}; Q^{2} (GeV^{2}); counts",                                     150, 0, 8);
   xB_pre_h     = new TH1F("xB_pre_h", "xB - Before MM^{2} cut; xB; counts",                             100, 0, 20);
   xB_h         = new TH1F("xB_h", "xB - Before MM^{2} cut; xB; counts",                                 100, 0, 20);
   W_pre_h      = new TH1F("W_pre_h", "Centre of Mass Energy - Before MM^{2} cut; W (GeV^{2}); counts",  120, 0, 30);
   W_h          = new TH1F("W_h", "Centre of Mass Energy; W (GeV^{2}); counts",                          120, 0, 30);

   pi0im_pi0mm2_pre_h   = new TH2F ("pi0im_pi0mm2_pre_h", "Invariant Mass of photon-pair vs. MM^{2} of channel - Pre-MM^{2} cut; MissingMass-Sq (GeV^{2}); Inv.Mass (GeV)", 200, -.5, .5, 200, 0, 0.2);
   pi0im_pi0mm2_h       = new TH2F ("pi0im_pi0mm2_h", "Invariant Mass of photon-pair vs. MM^{2} of channel; MissingMass-Sq (GeV^{2}); Inv.Mass (GeV)",                      200, -.5, .5, 200, 0, 0.2);
   pi0im_pi0mp_pre_h    = new TH2F ("pi0im_pi0mp_pre_h", "Invariant Mass of photon-pair vs. Mp of channel - Pre-MM^{2} cut; MissingMomentum-Sq (GeV^{2}); Inv.Mass (GeV)",  200, 0, 6, 200, 0, 0.2);
   pi0im_pi0mp_h        = new TH2F ("pi0im_pi0mp_h", "Invariant Mass of photon-pair vs. Mp of channel; MissingMomentum-Sq (GeV^{2}); Inv.Mass (GeV)",                       200, 0, 6, 200, 0, 0.2);
   pi0mm2_pi0mp_pre_h   = new TH2F ("pi0mm2_pi0mp_pre_h","MM^{2} vs. Mp - Pre-MM^{2} cut; MissingMomentum-Sq (GeV^{2}); MissingMass-Sq (GeV^{2})",                          200, 0, 6, 200, -.5, .5);
   pi0mm2_pi0mp_h       = new TH2F ("pi0mm2_pi0mp_h","MM^{2} vs. Mp; MissingMomentum-Sq (GeV^{2}); MissingMass-Sq (GeV^{2})",                                               200, 0, 6, 200, -.5, .5);

   Q2xB_pre_h   = new TH2F("Q2xB_pre_h", "Q{2} vs. x_{B} - Before MM^{2} cut; x_{B}; Q^{2} (GeV^{2})", 250, 0, 20, 150, 0, 8);
   Q2xB_h       = new TH2F("Q2xB_h", "Q{2} vs. x_{B}; x_{B}; Q^{2} (GeV^{2})",                         250, 0, 20, 150, 0, 8);

   recprotmm_pre_h = new TH1F("recprotmm2_pre_h", "Reconstructed Proton MissingMass-Sq - Before MM^{2} cuts; MM of reconstructed proton (GeV^{2}); counts", 180, 0, 3);
   recprotmm_h     = new TH1F("recprotmm2_h", "Reconstructed Proton MissingMass-Sq; MM of reconstructed proton (GeV^{2}); counts",                          180, 0, 3);
 }//histos fxn
