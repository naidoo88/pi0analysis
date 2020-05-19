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

void plotpi0IM(TString infile){
	TChain chain("data");
	chain.AddFile(infile);

	double IM_g1g2;
	double MM2_total;
	double pi0coneangle;
	bool   flag_goodpi0;

	chain.SetBranchAddress("IM_g1g2",       &IM_g1g2);
	chain.SetBranchAddress("MM2_total",     &MM2_total);
	chain.SetBranchAddress("pi0coneangle",  &pi0coneangle);
	chain.SetBranchAddress("flag_goodpi0",  &flag_goodpi0);

	TH1F* ggIM_raw_h      = new TH1F("ggIM_raw_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - Raw; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	TH1F* ggIM_good_h     = new TH1F("ggIM_good_h", "Invariant mass[#gamma_{1} #gamma_{2}] - Raw; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	TH1F* ggIM_rawcut_h   = new TH1F("ggIM_rawcut_h", "Invariant mass[#gamma_{1} #gamma_{2}] - MM^{2} +/- 0.5GeV, #pi_{0}-coneangle < 20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	TH1F* ggIM_goodcut_h  = new TH1F("ggIM_goodcut_h", "Invariant mass[#gamma_{1} #gamma_{2}] - MM^{2} +/- 0.5GeV, #pi_{0}-coneangle < 20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

	TH1F* totMM2_raw_h     =  new TH1F("totMM2_raw_h",     "MM^{2}[ep->e'p'#gamma_{1} #gamma_{2}] - Raw; Mis.Mass^{2} (GeV^{2}/c^{4}); counts", 100, -0.5, 0.5);
	TH1F* totMM2_good_h     = new TH1F("totMM2_good_h",    "MM^{2}[ep->e'p'#gamma_{1} #gamma_{2}] - Raw; Mis.Mass^{2} (GeV^{2}/c^{4}); counts", 100, -0.5, 0.5);
	TH1F* totMM2_rawcut_h   = new TH1F("totMM2_rawcut_h",  "MM^{2}[ep->e'p'#gamma_{1} #gamma_{2}] - MM^{2} +/- 0.5GeV, #pi_{0}-coneangle < 20^{o}; Mis.Mass^{2} (GeV^{2}/c^{4}); counts", 100, -0.5, 0.5);
	TH1F* totMM2_goodcut_h  = new TH1F("totMM2_goodcut_h", "MM^{2}[ep->e'p'#gamma_{1} #gamma_{2}] - MM^{2} +/- 0.5GeV, #pi_{0}-coneangle < 20^{o}; Mis.Mass^{2} (GeV^{2}/c^{4}); counts", 100, -0.5, 0.5);

	TH1F* ggIM_detsplit_h[6];
	ggIM_detsplit_h[0] = new TH1F("ggIM_bothPCAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both PCAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	ggIM_detsplit_h[1] = new TH1F("ggIM_bothECAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both ECAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	ggIM_detsplit_h[2] = new TH1F("ggIM_bothFCAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FCAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	ggIM_detsplit_h[3] = new TH1F("ggIM_PCALECAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/ECAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	ggIM_detsplit_h[4] = new TH1F("ggIM_ECALFCAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - ECAL/FCAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	ggIM_detsplit_h[5] = new TH1F("ggIM_PCALFCAL_h", "Invariant mass[#gamma_{1} #gamma_{2}] - PCAL/FCAL; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

	for(Int_t i=0;i<chain.GetEntries();i++){
		chain.GetEntry(i);

		ggIM_raw_h->Fill(IM_g1g2);
		if (flag_goodpi0==1)                                                            ggIM_good_h->   Fill(IM_g1g2);
		if((MM2_total > -0.5 && MM2_total < 0.5)&&(pi0coneangle<20)&&(flag_goodpi0==1)) ggIM_goodcut_h->Fill(IM_g1g2);
		if((MM2_total > -0.5 && MM2_total < 0.5)&&(pi0coneangle<20))                    ggIM_rawcut_h-> Fill(IM_g1g2);

		totMM2_raw_h->Fill(MM2_total);
		if (flag_goodpi0==1)                                                            totMM2_good_h->   Fill(MM2_total);
		if((MM2_total > -0.5 && MM2_total < 0.5)&&(pi0coneangle<20)&&(flag_goodpi0==1)) totMM2_goodcut_h->Fill(MM2_total);
		if((MM2_total > -0.5 && MM2_total < 0.5)&&(pi0coneangle<20))                    totMM2_rawcut_h-> Fill(MM2_total);
	}

	TCanvas* ggIM_compare_c = new TCanvas("ggIM_compare_c");
	ggIM_compare_c->Divide(2,1);

	ggIM_compare_c->cd(1);
	ggIM_raw_h->Draw();
	ggIM_good_h->SetLineColor(kRed);
	ggIM_good_h->Draw("same");

	ggIM_compare_c->cd(2);
	ggIM_rawcut_h->Draw();
	ggIM_goodcut_h->SetLineColor(kRed);
	ggIM_goodcut_h->Draw("same");

	// TCanvas* totMM2_compare_c = new TCanvas("totMM2_compare_c");
	// totMM2_compare_c->Divide(2,2);
	//
	// totMM2_compare_c->cd(1);
	// totMM2_raw_h->Draw();
	// totMM2_good_h->SetLineColor(kRed);
	// totMM2_good_h->Draw("same");
	//
	// totMM2_compare_c->cd(2);
	// totMM2_rawcut_h->Draw();
	// totMM2_goodcut_h->SetLineColor(kRed);
	// totMM2_goodcut_h->Draw("same");
	//
	// totMM2_compare_c->cd(3);
	// //totMM2_good_h->SetLineColor(kRed);
	// totMM2_good_h->Draw();
	//
	// totMM2_compare_c->cd(4);
	// totMM2_goodcut_h->SetLineColor(kRed);
	// totMM2_goodcut_h->Draw();


}
