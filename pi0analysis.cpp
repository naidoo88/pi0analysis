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
        c12.AddAtLeastPid(22,2);          //at least 2 photons

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
        //double neutm = db -> GetParticle(2112)->Mass();

        while(c12.next()) {                         //loop over events
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

          int j = 0;
          int N = photonbuff.size();
          for (int i=0; i<N-1; i++){
            for (int j=i+1; j=N-1; j++){
              phot1.SetXYZM(photonbuff[i]->par()->getPx(),
                            photonbuff[i]->par()->getPy(),
                            photonbuff[i]->par()->getPz(),
                            0);
              phot2.SetXYZM(photonbuff[j]->par()->getPx(),
                            photonbuff[j]->par()->getPy(),
                            photonbuff[j]->par()->getPz(),
                            0);

              TLorentzVector photcomb = phot1 + phot2; 
            }//photons j-loop
          }//photons i-loop

            /*q  = beam-e;
            mm = (beam + target) - (e + pip);
            double Q2  = -1*q.M2();
            double xb  = Q2 / ( 2 * protm * (beam.E()-e.E()) );
            double Mp2 = mm.Perp2();
            double MM2 = mm.M2();

            Q2h       ->Fill(Q2);
            Q2semih   ->Fill(Q2);
            XBh       ->Fill(xb);
            XBsemih   ->Fill(xb);
            Mp2h      ->Fill(Mp2);
            MM2h      ->Fill(MM2);
            Q2XBh     ->Fill(Q2, xb);
            Q2XBsemih ->Fill(Q2, xb);*/


          n_events++;
        }//event while-loop

        sprintf(last_file,"%s",file_name);     // save name of the current file into "last_file"
        n_files++;

      }//duplicate check
    }//file list loop
  }//list open-check

  list_of_files.close();
  cout << "\nTotal no of files in list: " << n_files << endl;

  Out_File->Write();
  Out_File->Close();

}//pi0analysis fxn

  void histos(){
  /*npxh     = new TH1F("npxh", "Neutron px; px (GeV); counts", 100, -1.5, 1.5);
  npyh     = new TH1F("npyh", "Neutron py; py (GeV); counts", 100, -1.5, 1.5);
  npzh     = new TH1F("npzh", "Neutron pz; pz (GeV); counts", 100, -1  , 2);
  nEh      = new TH1F("nEh" , "Neutron E;  E (GeV) ; counts", 200,  0.5, 3);

  Q2h      = new TH1F("Q2h"    , "Q^{2};  Q^{2};  counts",                       100, -1, 9);
  Q2exh    = new TH1F("Q2exh"  , "Q^{2} - Exclusive only;  Q^{2};  counts",      100, -1, 9);
  Q2semih  = new TH1F("Q2semih", "Q^{2} - Semi-inclusive only;  Q^{2};  counts", 100, -1, 9);

  XBh      = new TH1F("XBh"    , "X_{B};  X_{B};  counts",                       100, -0.5, 1.5);
  XBexh    = new TH1F("XBexh"  , "X_{B} - Exclusive only;  X_{B};  counts",      100, -0.5, 1.5);
  XBsemih  = new TH1F("XBsemih", "X_{B} - Semi-inclusive only;  X_{B};  counts", 100, -0.5, 1.5);

  MM2h      = new TH1F("MM2h" , "Missing Mass - Squared;  MM(GeV); counts",     100, -20, 25);
  Mp2h      = new TH1F("Mp2h" , "Missing Momentum - Squared;  MM(GeV); counts", 100, -1, 6);

  Q2XBh     = new TH2F("Q2XBh"    , "Q^{2} vs. X_{B}; Q^{2}; X_{B}", 100, -1, 9, 100, -0.5, 1.5);
  Q2XBsemih = new TH2F("Q2XBsemih", "Q^{2} vs. X_{B}; Q^{2}; X_{B}", 100, -1, 9, 100, -0.5, 1.5);

  //  sectnh   = new TH1F("sectnh"    , "Section - CND Neutrons; Section;  counts",  26, -0.5, 25.5);
  //  sectph   = new TH1F("sectph"    , "Section - CND Pi+s; Section;  counts",  26, -0.5, 25.5);
  sect1nh  = new TH1F("sect1nh"   , "Section - nCND1; Section;  counts",  26, -0.5, 25.5);
  sect2nh  = new TH1F("sect2nh"   , "Section - nCND2; Section;  counts",  26, -0.5, 25.5);
  sect3nh  = new TH1F("sect3nh"   , "Section - nCND3; Section;  counts",  26, -0.5, 25.5);
  sect1ph  = new TH1F("sect1ph"   , "Section - pipCND1; Section;  counts",  26, -0.5, 25.5);
  sect2ph  = new TH1F("sect2ph"   , "Section - pipCND2; Section;  counts",  26, -0.5, 25.5);
  sect3ph  = new TH1F("sect3ph"   , "Section - pipCND3; Section;  counts",  26, -0.5, 25.5);

  sectnh  = new TH1F("sectnh"   , "Section - Neutrons (all layers); Section;  counts",  26, -0.5, 25.5);
  sectph  = new TH1F("sectph"   , "Section - Pi+ (all layers);      Section;  counts",  26, -0.5, 25.5);

  layer0testh  = new TH1F("layer0testh" , "CTOF  ; ~;  counts",  200, 0, 100);
  layer1testh  = new TH1F("layer1testh" , "Layer1; ~;  counts",  200, 0, 100);
  layer2testh  = new TH1F("layer2testh" , "Layer2; ~;  counts",  200, 0, 100);
  layer3testh  = new TH1F("layer3testh" , "Layer3; ~;  counts",  200, 0, 100);*/
}//histos fxn
