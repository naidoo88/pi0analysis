// @(#)root
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include <TFile.h>
#include <TMath.h>
#include <TChain.h>
#include <TF1.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TText.h>
#include <TPaveStats.h>
#include <TString.h>

#include "clas12reader.h"

using namespace clas12;

//void plotPi0IM(TString infile);

void plotIMMM2(TString infile){
	TChain chain("data");
	chain.AddFile(infile);

	double IM_g1g2;
	double MM2_total;
	bool   flag_goodpi0;
    bool   flag_cuts_dis;
    bool   flag_cuts_broadMM2;
    bool   flag_cuts_broadcone;
    bool   flag_cuts_spectMP;
    bool   flag_cuts_3sigMM2;
    bool   flag_cuts_cop;

	chain.SetBranchAddress("IM_g1g2",       &IM_g1g2);
	chain.SetBranchAddress("MM2_total",     &MM2_total);

    chain.SetBranchAddress("flag_goodpi0",        &flag_goodpi0);
    chain.SetBranchAddress("flag_cuts_broadcone", &flag_cuts_broadcone);
    chain.SetBranchAddress("flag_cuts_dis",     &flag_cuts_dis);
    chain.SetBranchAddress("flag_cuts_spectMP", &flag_cuts_spectMP);  
    chain.SetBranchAddress("flag_cuts_3sigMM2", &flag_cuts_3sigMM2);    
    chain.SetBranchAddress("flag_cuts_cop",     &flag_cuts_cop);

	TH1F* ggIM_h           = new TH1F("ggIM_h",            "; M_{#gamma#gamma} (GeV/c^{2}); counts", 200, 0, 0.2);
	TH1F* ggIM_goodcut_h   = new TH1F("ggIM_goodcut_h",    "; M_{#gamma#gamma} (GeV/c^{2}); counts", 200, 0, 0.2);

	TH1F* totMM2_h         =  new TH1F("totMM2_h",         "; Mis.Mass^{2} (GeV^{2}/c^{4}); counts", 400, -0.5, 0.5);
	TH1F* totMM2_goodcut_h =  new TH1F("totMM2_goodcut_h", "; Mis.Mass^{2} (GeV^{2}/c^{4}); counts", 400, -0.5, 0.5);
	

	for(Int_t i=0;i<chain.GetEntries();i++){
		chain.GetEntry(i);

        if(flag_cuts_dis       != 1) continue;
        if(flag_cuts_broadcone != 1) continue;   
        if(flag_cuts_spectMP   != 1) continue;
        if(flag_cuts_3sigMM2   != 1) continue; 
        if(flag_cuts_cop       != 1) continue; 

        ggIM_h   ->Fill(IM_g1g2);
        totMM2_h ->Fill(MM2_total);

        if(flag_goodpi0        != 1) continue;  //corresponds to 3sig cut on pi0-mass
        ggIM_goodcut_h   ->Fill(IM_g1g2);
        totMM2_goodcut_h ->Fill(MM2_total);       

	}

    TCanvas *C1 = new TCanvas("C1");
    C1->cd();
    ggIM_h         ->Draw();
    ggIM_goodcut_h ->SetLineColor(kRed);
    ggIM_goodcut_h ->Draw("sames");


    TCanvas *C2 = new TCanvas("C2");
    C2->cd();
    totMM2_h ->Draw();
    totMM2_goodcut_h ->SetLineColor(kRed);
    totMM2_goodcut_h ->Draw("sames");

}
