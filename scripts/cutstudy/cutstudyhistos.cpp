// @(#)root
#include <cstdlib>
#include <iostream>
#include <vector>

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
#include <TString.h>

#include "clas12reader.h"
#include "cutstudyhistos.h"

using namespace clas12;

void cutstudyhistos(TString datafile, TString outfile){

  TChain chain("data");
  chain.AddFile(datafile);

  double pi0im;
  double pi0mm2;
  double pi0coneangle;
  bool flag_photon1_PCAL;
  bool flag_photon2_PCAL;
  bool flag_photon1_ECAL;
  bool flag_photon2_ECAL;
  bool flag_photon1_ft;
  bool flag_photon2_ft;

  chain.SetBranchAddress("pi0im",             &pi0im);
  chain.SetBranchAddress("pi0mm2",            &pi0mm2);
  chain.SetBranchAddress("pi0coneangle",      &pi0coneangle);
  chain.SetBranchAddress("flag_photon1_PCAL", &flag_photon1_PCAL);
  chain.SetBranchAddress("flag_photon2_PCAL", &flag_photon2_PCAL);
  chain.SetBranchAddress("flag_photon1_ECAL", &flag_photon1_ECAL);
  chain.SetBranchAddress("flag_photon2_ECAL", &flag_photon2_ECAL);
  chain.SetBranchAddress("flag_photon1_ft",   &flag_photon1_ft);
  chain.SetBranchAddress("flag_photon2_ft",   &flag_photon2_ft);

  cuthistos();

  std::cout << "Filling Histograms..." << '\n';
  for(Int_t i=0;i<chain.GetEntries();i++){
    chain.GetEntry(i);

    /*=====PRE-CUT HISTOS=====*/
    if (flag_photon1_PCAL==1 && flag_photon2_PCAL==1) ggIM_h[0]  -> Fill(pi0im);
    if (flag_photon1_ECAL==1 && flag_photon2_ECAL==1) ggIM_h[1]  -> Fill(pi0im);
    if (flag_photon1_ft  ==1 && flag_photon2_ft  ==1) ggIM_h[2]  -> Fill(pi0im);
    if ((flag_photon1_PCAL==1 && flag_photon2_ECAL==1) || (flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) ggIM_h[3]  -> Fill(pi0im);
    if ((flag_photon1_ECAL==1 && flag_photon2_ft  ==1) || (flag_photon2_ECAL==1 && flag_photon1_ft  ==1)) ggIM_h[4]  -> Fill(pi0im);
    if ((flag_photon1_PCAL==1 && flag_photon2_ft  ==1) || (flag_photon2_PCAL==1 && flag_photon1_ft  ==1)) ggIM_h[5]  -> Fill(pi0im);

    /*=====Cone angle histos=====*/
    if (pi0coneangle < 30){
      if (flag_photon1_PCAL==1 && flag_photon2_PCAL==1) ggIM_coneangle_h[0][0]  -> Fill(pi0im);
      if (flag_photon1_ECAL==1 && flag_photon2_ECAL==1) ggIM_coneangle_h[0][1]  -> Fill(pi0im);
      if (flag_photon1_ft  ==1 && flag_photon2_ft  ==1) ggIM_coneangle_h[0][2]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ECAL==1) || (flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) ggIM_coneangle_h[0][3]  -> Fill(pi0im);
      if ((flag_photon1_ECAL==1 && flag_photon2_ft  ==1) || (flag_photon2_ECAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[0][4]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ft  ==1) || (flag_photon2_PCAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[0][5]  -> Fill(pi0im);
    }
    if (pi0coneangle < 20){
      if (flag_photon1_PCAL==1 && flag_photon2_PCAL==1) ggIM_coneangle_h[1][0]  -> Fill(pi0im);
      if (flag_photon1_ECAL==1 && flag_photon2_ECAL==1) ggIM_coneangle_h[1][1]  -> Fill(pi0im);
      if (flag_photon1_ft  ==1 && flag_photon2_ft  ==1) ggIM_coneangle_h[1][2]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ECAL==1) || (flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) ggIM_coneangle_h[1][3]  -> Fill(pi0im);
      if ((flag_photon1_ECAL==1 && flag_photon2_ft  ==1) || (flag_photon2_ECAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[1][4]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ft  ==1) || (flag_photon2_PCAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[1][5]  -> Fill(pi0im);
    }
    if (pi0coneangle < 10){
      if (flag_photon1_PCAL==1 && flag_photon2_PCAL==1) ggIM_coneangle_h[2][0]  -> Fill(pi0im);
      if (flag_photon1_ECAL==1 && flag_photon2_ECAL==1) ggIM_coneangle_h[2][1]  -> Fill(pi0im);
      if (flag_photon1_ft  ==1 && flag_photon2_ft  ==1) ggIM_coneangle_h[2][2]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ECAL==1) || (flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) ggIM_coneangle_h[2][3]  -> Fill(pi0im);
      if ((flag_photon1_ECAL==1 && flag_photon2_ft  ==1) || (flag_photon2_ECAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[2][4]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ft  ==1) || (flag_photon2_PCAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[2][5]  -> Fill(pi0im);
    }
    if (pi0coneangle < 8){
      if (flag_photon1_PCAL==1 && flag_photon2_PCAL==1) ggIM_coneangle_h[3][0]  -> Fill(pi0im);
      if (flag_photon1_ECAL==1 && flag_photon2_ECAL==1) ggIM_coneangle_h[3][1]  -> Fill(pi0im);
      if (flag_photon1_ft  ==1 && flag_photon2_ft  ==1) ggIM_coneangle_h[3][2]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ECAL==1) || (flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) ggIM_coneangle_h[3][3]  -> Fill(pi0im);
      if ((flag_photon1_ECAL==1 && flag_photon2_ft  ==1) || (flag_photon2_ECAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[3][4]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ft  ==1) || (flag_photon2_PCAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[3][5]  -> Fill(pi0im);
    }
    if (pi0coneangle < 5){
      if (flag_photon1_PCAL==1 && flag_photon2_PCAL==1) ggIM_coneangle_h[4][0]  -> Fill(pi0im);
      if (flag_photon1_ECAL==1 && flag_photon2_ECAL==1) ggIM_coneangle_h[4][1]  -> Fill(pi0im);
      if (flag_photon1_ft  ==1 && flag_photon2_ft  ==1) ggIM_coneangle_h[4][2]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ECAL==1) || (flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) ggIM_coneangle_h[4][3]  -> Fill(pi0im);
      if ((flag_photon1_ECAL==1 && flag_photon2_ft  ==1) || (flag_photon2_ECAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[4][4]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ft  ==1) || (flag_photon2_PCAL==1 && flag_photon1_ft  ==1)) ggIM_coneangle_h[4][5]  -> Fill(pi0im);
    }

    /*=====MM2 histos=====*/
    if(pi0mm2>-0.4&&pi0mm2<0.4) {
      if (flag_photon1_PCAL==1 && flag_photon2_PCAL==1) ggIM_mm2_h[0][0]  -> Fill(pi0im);
      if (flag_photon1_ECAL==1 && flag_photon2_ECAL==1) ggIM_mm2_h[0][1]  -> Fill(pi0im);
      if (flag_photon1_ft  ==1 && flag_photon2_ft  ==1) ggIM_mm2_h[0][2]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ECAL==1) || (flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) ggIM_mm2_h[0][3]  -> Fill(pi0im);
      if ((flag_photon1_ECAL==1 && flag_photon2_ft  ==1) || (flag_photon2_ECAL==1 && flag_photon1_ft  ==1)) ggIM_mm2_h[0][4]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ft  ==1) || (flag_photon2_PCAL==1 && flag_photon1_ft  ==1)) ggIM_mm2_h[0][5]  -> Fill(pi0im);
    }
    if(pi0mm2>-0.3&&pi0mm2<0.3){
      if (flag_photon1_PCAL==1 && flag_photon2_PCAL==1) ggIM_mm2_h[1][0]  -> Fill(pi0im);
      if (flag_photon1_ECAL==1 && flag_photon2_ECAL==1) ggIM_mm2_h[1][1]  -> Fill(pi0im);
      if (flag_photon1_ft  ==1 && flag_photon2_ft  ==1) ggIM_mm2_h[1][2]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ECAL==1) || (flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) ggIM_mm2_h[1][3]  -> Fill(pi0im);
      if ((flag_photon1_ECAL==1 && flag_photon2_ft  ==1) || (flag_photon2_ECAL==1 && flag_photon1_ft  ==1)) ggIM_mm2_h[1][4]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ft  ==1) || (flag_photon2_PCAL==1 && flag_photon1_ft  ==1)) ggIM_mm2_h[1][5]  -> Fill(pi0im);
    }
    if(pi0mm2>-0.2&&pi0mm2<0.2){
      if (flag_photon1_PCAL==1 && flag_photon2_PCAL==1) ggIM_mm2_h[2][0]  -> Fill(pi0im);
      if (flag_photon1_ECAL==1 && flag_photon2_ECAL==1) ggIM_mm2_h[2][1]  -> Fill(pi0im);
      if (flag_photon1_ft  ==1 && flag_photon2_ft  ==1) ggIM_mm2_h[2][2]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ECAL==1) || (flag_photon2_PCAL==1 && flag_photon1_ECAL==1)) ggIM_mm2_h[2][3]  -> Fill(pi0im);
      if ((flag_photon1_ECAL==1 && flag_photon2_ft  ==1) || (flag_photon2_ECAL==1 && flag_photon1_ft  ==1)) ggIM_mm2_h[2][4]  -> Fill(pi0im);
      if ((flag_photon1_PCAL==1 && flag_photon2_ft  ==1) || (flag_photon2_PCAL==1 && flag_photon1_ft  ==1)) ggIM_mm2_h[2][5]  -> Fill(pi0im);
    }

  }//chain-loop

  /*=====Plot some canvases=====*/
  //int numpads = ggIM_conecuts_c->GetListOfPrimitives()->GetSize();  //retrieves number of panels in a divided canvas.  May have uses.
  TCanvas *ggIM_conecuts_c = new TCanvas("ggIM_conecuts_c", "#gamma_{1}#gamma_{2}-mass split by region (Cone angle < 30',20',10', 8', 5')");
  ggIM_conecuts_c->Divide(6,5);
  int n=0;
  for (int i = 0; i <= 4; i++) {
    for (int j = 0; j <= 5; j++){
      ggIM_conecuts_c->cd(n+1);
      ggIM_coneangle_h[i][j]->Draw();
      n++;
    }
  }

  TCanvas *ggIM_MM2cuts_c = new TCanvas("ggIM_MM2cuts_c", "#gamma_{1}#gamma_{2}-mass split by region (MM2 +/- 0.4. 0.3, 0.2 Gev^{2}/c^{4})");
  ggIM_MM2cuts_c->Divide(6,3);
  n=0;
  for (int i = 0; i <= 2; i++) {
    for (int j = 0; j <= 5; j++){
      ggIM_MM2cuts_c->cd(n+1);
      ggIM_mm2_h[i][j]->Draw();
      n++;
    }
  }


  /*=====Write out to file=====*/
  std::cout << "Writting out to file..." << '\n';
  TFile *Out_File = new TFile(outfile, "recreate");

  for (int i = 0; i <= 5; i++) {
    ggIM_h[i]->Write();
    for (int j = 0; j <= 4; j++){
      ggIM_coneangle_h[j][i]->Write();
    }
  }
  for (int i = 0; i <= 5; i++) {
    for (int j = 0; j <= 2; j++){
      ggIM_mm2_h[j][i]->Write();
    }
  }
  ggIM_MM2cuts_c->Write();
  ggIM_conecuts_c->Write();

  Out_File->Close();

}//macro

void cuthistos(){
  std::cout << "Generating histograms..." << '\n';

  /* For reg-splits:
  [*][0] - both photons in PCAL
  [*][1] - both photons in ECAL
  [*][2] - both photons in FCAL
  [*][3] - one in PCAL one in ECAL
  [*][4] - one in ECAL one in FCAL
  [*][5] - one in PCAL one in FCAL
  */

  //Raw g1g2-inv.mass split by region.
  //---------------------------------------------------------------------------------------------------------------
  ggIM_h[0] = new TH1F("ggIM_bothPCAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both PCAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_h[1] = new TH1F("ggIM_bothECAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both ECAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_h[2] = new TH1F("ggIM_bothFCAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FCAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_h[3] = new TH1F("ggIM_PCALECAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/ECAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_h[4] = new TH1F("ggIM_ECALFCAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - ECAL/FCAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_h[5] = new TH1F("ggIM_PCALFCAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/FCAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

  //Cone angle cuts - g1g2-inv.mass split by region.
  //---------------------------------------------------------------------------------------------------------------
  ggIM_coneangle_h[0][0] = new TH1F("ggIM_bothPCAL_coneangleL30_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both PCAL - ConeAngle<30^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[0][1] = new TH1F("ggIM_bothECAL_coneangleL30_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both ECAL - ConeAngle<30^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[0][2] = new TH1F("ggIM_bothFCAL_coneangleL30_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FCAL - ConeAngle<30^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[0][3] = new TH1F("ggIM_PCALECAL_coneangleL30_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/ECAL - ConeAngle<30^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[0][4] = new TH1F("ggIM_ECALFCAL_coneangleL30_h", "Invariant mass[#gamma_{1} #gamma_{2}] - ECAL/FCAL - ConeAngle<30^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[0][5] = new TH1F("ggIM_PCALFCAL_coneangleL30_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/FCAL - ConeAngle<30^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

  ggIM_coneangle_h[1][0] = new TH1F("ggIM_bothPCAL_coneangleL20_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both PCAL - ConeAngle<20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[1][1] = new TH1F("ggIM_bothECAL_coneangleL20_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both ECAL - ConeAngle<20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[1][2] = new TH1F("ggIM_bothFCAL_coneangleL20_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FCAL - ConeAngle<20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[1][3] = new TH1F("ggIM_PCALECAL_coneangleL20_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/ECAL - ConeAngle<20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[1][4] = new TH1F("ggIM_ECALFCAL_coneangleL20_h", "Invariant mass[#gamma_{1} #gamma_{2}] - ECAL/FCAL - ConeAngle<20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[1][5] = new TH1F("ggIM_PCALFCAL_coneangleL20_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/FCAL - ConeAngle<20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

  ggIM_coneangle_h[2][0] = new TH1F("ggIM_bothPCAL_coneangleL10_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both PCAL - ConeAngle<10^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[2][1] = new TH1F("ggIM_bothECAL_coneangleL10_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both ECAL - ConeAngle<10^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[2][2] = new TH1F("ggIM_bothFCAL_coneangleL10_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FCAL - ConeAngle<10^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[2][3] = new TH1F("ggIM_PCALECAL_coneangleL10_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/ECAL - ConeAngle<10^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[2][4] = new TH1F("ggIM_ECALFCAL_coneangleL10_h", "Invariant mass[#gamma_{1} #gamma_{2}] - ECAL/FCAL - ConeAngle<10^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[2][5] = new TH1F("ggIM_PCALFCAL_coneangleL10_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/FCAL - ConeAngle<10^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

  ggIM_coneangle_h[3][0] = new TH1F("ggIM_bothPCAL_coneangleL8_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - both PCAL - ConeAngle<8^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[3][1] = new TH1F("ggIM_bothECAL_coneangleL8_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - both ECAL - ConeAngle<8^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[3][2] = new TH1F("ggIM_bothFCAL_coneangleL8_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - both FCAL - ConeAngle<8^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[3][3] = new TH1F("ggIM_PCALECAL_coneangleL8_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/ECAL - ConeAngle<8^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[3][4] = new TH1F("ggIM_ECALFCAL_coneangleL8_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - ECAL/FCAL - ConeAngle<8^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[3][5] = new TH1F("ggIM_PCALFCAL_coneangleL8_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/FCAL - ConeAngle<8^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

  ggIM_coneangle_h[4][0] = new TH1F("ggIM_bothPCAL_coneangleL5_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - both PCAL - ConeAngle<5^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[4][1] = new TH1F("ggIM_bothECAL_coneangleL5_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - both ECAL - ConeAngle<5^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[4][2] = new TH1F("ggIM_bothFCAL_coneangleL5_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - both FCAL - ConeAngle<5^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[4][3] = new TH1F("ggIM_PCALECAL_coneangleL5_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/ECAL - ConeAngle<5^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[4][4] = new TH1F("ggIM_ECALFCAL_coneangleL5_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - ECAL/FCAL - ConeAngle<5^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
  ggIM_coneangle_h[4][5] = new TH1F("ggIM_PCALFCAL_coneangleL5_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/FCAL - ConeAngle<5^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

  //Broad MM2 cuts - g1g2-inv.mass split by region.
  //---------------------------------------------------------------------------------------------------------------
  TString namebuff;
  TString titlebuff;
  TString base         = "ggIM_";
  vector<TString> det  = {"bothPCAL_", "bothECAL_", "bothFCAL_", "PCALECAL_", "ECALFCAL_", "PCALFCAL_"};
  vector<TString> cut  = {"mm2PM0.4_h", "mm2PM0.3_h","mm2PM0.2_h"};
  TString titlebase    = "Invariant mass[#gamma_{1} #gamma_{2}]";
  vector<TString> titledet = {" - both PCAL - ", " - both ECAL - ", " - both FCAL - ", " - PCAL/ECAL - ", " - ECAL/FCAL - ", " - PCAL/FCAL - "};
  vector<TString> titlecut = {"MM^{2}#pm0.4Gev^{2}/c^{4};", "MM^{2}#pm0.3Gev^{2}/c^{4};","MM^{2}#pm0.2Gev^{2}/c^{4};"}; //';' termination for TH1F constr. syntax
  TString xlable = "Inv.Mass (GeV/c^{2});";                                                                             //';' termination for TH1F constr. syntax
  TString ylable = "counts";
  vector <int> nbins = {150, 150, 100, 200, 75, 75};
  double xmin  = 0;
  double xmax  = 0.2;

  for (int i = 0; i < det.size(); i++){
    for (int j = 0; j < cut.size(); j++) {
      namebuff = base + det[i] + cut[j];
      titlebuff = titlebase+titledet[i]+titlecut[j]+xlable+ylable;
      ggIM_mm2_h[j][i] = new TH1F(namebuff, titlebuff, nbins[i], xmin, xmax);
    }
  }

}//histos fxn
