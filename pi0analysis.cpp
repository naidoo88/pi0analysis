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
  TFile *Out_File = new TFile("out.root", "recreate");
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
          }//close else

          TLorentzVector photcombo = phot1 + phot2;
          TLorentzVector system = (beam+target)-(e+prot+phot1+phot2);
          double pi0s   = photcombo.M();
          double pi0mm2 = system.M2();
          pi0s_h ->Fill(pi0s);
          pi0mm2_h->Fill(pi0mm2);

/*
            q  = beam-e;
            mm = (beam + target) - (e + pip);
            double Q2  = -1*q.M2();
            double xb  = Q2 / ( 2 * protm * (beam.E()-e.E()) );
            double Mp2 = mm.Perp2();
            double MM2 = mm.M2();

            Q2h       ->Fill(Q2);
            XBh       ->Fill(xb);
            Q2XBh     ->Fill(Q2, xb);
*/
          n_events++;
          // if (n_events == 10) break;
        }//event while-loop

        sprintf(last_file,"%s",file_name);     // save name of the current file into "last_file"
        n_files++;

      }//duplicate check
    }//file list loop
  }//list open-check

  list_of_files.close();
  cout << "\nTotal no of files in list: " << n_files << endl;
  cout << "\nTotal no of events processed: " << n_events << endl;

  Out_File->Write();
  Out_File->Close();

}//pi0analysis fxn

  void histos(){
    pi0s_h  = new TH1F("pi0s",   "Invariant mass of paired photons; Mass (GeV); counts", 250, 0, 0.2);
    pi0mm2_h = new TH1F("pi0mm2", "Missing Mass-Squared; Mass (GeV^{2}); counts", 200, -.5, .5);
/*
  Q2h      = new TH1F("Q2h", "Q^{2};  Q^{2};  counts", 200, -1, 9);
  XBh      = new TH1F("XBh", "X_{B};  X_{B};  counts", 100, -0.5, 1.5);
*/
}//histos fxn
