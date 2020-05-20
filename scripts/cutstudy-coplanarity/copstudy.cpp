// @(#)root

/* 
//Script to plot and fit coplanarity distributions for Trento-Phi, defined three ways: 
// 1. Recoil x New Particle (ie. Meson or Photon)
// 2. Recoil x Virtual-Photon
// 3. New Particle x Virtual-Photon
*/

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
#include <TF1.h>
#include <TStyle.h>

#include "clas12reader.h"


using namespace clas12;

void copstudy(TString datafile, TString outfile){

    gStyle->SetOptFit(01111);
	gStyle->SetOptStat(11);

	TChain chain("data");
	chain.AddFile(datafile);

    bool flag_goodpi0;
    bool flag_cuts_dis;
    bool flag_cuts_broadMM2;
    bool flag_cuts_broadcone;
    bool flag_cuts_spectMP;  

    double cop_Nvg_vgnew;  
    double cop_Nvg_Nnew;  
    double cop_Nnew_vgnew;  
    double MM2_total;

    chain.SetBranchAddress("flag_goodpi0",        &flag_goodpi0);
    chain.SetBranchAddress("flag_cuts_dis",       &flag_cuts_dis);
    chain.SetBranchAddress("flag_cuts_broadMM2",  &flag_cuts_broadMM2);
    chain.SetBranchAddress("flag_cuts_broadcone", &flag_cuts_broadcone);
    chain.SetBranchAddress("flag_cuts_spectMP",   &flag_cuts_spectMP);  

    chain.SetBranchAddress("cop_Nvg_vgnew",       &cop_Nvg_vgnew);  
    chain.SetBranchAddress("cop_Nvg_Nnew",        &cop_Nvg_Nnew);  
    chain.SetBranchAddress("cop_Nnew_vgnew",      &cop_Nnew_vgnew);  
    chain.SetBranchAddress("MM2_total",           &MM2_total);

    TH1F *cop_Rvg_vgN_h = new TH1F("cop_Rvg_vgN_h", "Coplanarity: Trento #phi - Recoil/VirtPhot vs NewPart/VirtPhot;  phi; counts", 520, -180, 180);
	TH1F *cop_Rvg_RN_h  = new TH1F("cop_Rvg_RN_h",  "Coplanarity: Trento #phi - Recoil/VirtualPhot vs Recoil/NewPart; phi; counts", 520, -180, 180);
    TH1F *cop_RN_vgN_h  = new TH1F("cop_RN_vgN_h",  "Coplanarity: Trento #phi - Recoil/NewPart vs VirtPhot/NewPart;   phi; counts", 520, -180, 180);

	for(Int_t i=0;i<chain.GetEntries();i++){
        chain.GetEntry(i);

        if(flag_goodpi0        != 1) continue;  //corresponds to 3sig cut on pi0-mass
        if(flag_cuts_dis       != 1) continue;
        if(flag_cuts_broadcone != 1) continue;   
        if(flag_cuts_spectMP   != 1) continue;
        if(!(MM2_total > -0.05155 && MM2_total < 0.03455)) continue; //can be changed to flag_cuts_3sigMM2 soon

        cop_Rvg_vgN_h -> Fill(cop_Nvg_vgnew); 
        cop_Rvg_RN_h  -> Fill(cop_Nvg_Nnew); 
        cop_RN_vgN_h  -> Fill(cop_Nnew_vgnew); 
    }

    TF1* cop_Rvg_vgN_fit = new TF1("cop_Rvg_vgN_fit", "gaus(0)+[3]", -25, 25);
    cop_Rvg_vgN_fit->SetRange(-25, 25);
    cop_Rvg_vgN_fit->SetParameter(1, 0);
    cop_Rvg_vgN_fit->SetParameter(2, 5);
    TF1* cop_Rvg_RN_fit  = new TF1("cop_Rvg_RN_fit",  "gaus(0)+pol2(3)", - 8,  8);
    cop_Rvg_RN_fit->SetRange(-8, 8);
    cop_Rvg_RN_fit->SetParameter(1, 0);
    cop_Rvg_RN_fit->SetParameter(2, 5);
    TF1* cop_RN_vgN_fit  = new TF1("cop_RN_vgN_fit",  "gaus(0)+[3]", -25, 25);
    cop_RN_vgN_fit->SetRange(-25, 25);
    cop_RN_vgN_fit->SetParameter(1, 0);
    cop_RN_vgN_fit->SetParameter(2, 5);

    cop_Rvg_vgN_h -> Fit(cop_Rvg_vgN_fit, "RM");
    cop_Rvg_RN_h  -> Fit(cop_Rvg_RN_fit, "RM");
    cop_RN_vgN_h  -> Fit(cop_RN_vgN_fit, "RM");

    TFile *Out_File = new TFile(outfile, "recreate"); 
    cop_Rvg_vgN_h -> Write();
    cop_Rvg_RN_h  -> Write();
    cop_RN_vgN_h  -> Write();

    Out_File -> Close();



  

}