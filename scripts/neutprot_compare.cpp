// @(#)root
//Short script to compare pass0 and pass1 plots of interest

#include <iostream>

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TString.h"

void neutprot_compare()
{
    gStyle->SetOptFit(01111);
	gStyle->SetOptStat(11);

    TFile* protfile = new TFile("plots/prot/p1plots.root", "READ");
    TFile* neutfile = new TFile("plots/neut/neut_plots.root", "READ");
    TFile* protasymfile = new TFile("plots/prot/p1helasymall.root", "READ");
    TFile* neutasymfile = new TFile("plots/neut/neut_asym_all.root", "READ");
    
    TFile* OutFile = new TFile("plots/neut/neutprot_compare/neutprot_compare.root", "RECREATE");

    TH1F* ggIM_raw_h[2];
    TH1F* ggIM_sig_h[2];
    TH2F* Q2_v_xB_raw_h[2];
    TH2F* IMgg_v_MM2tot_raw_h[2];
    TH2F* IMgg_v_MM2tot_sig_h[2];
    TH2F* MEtot_v_MM2eggX_raw_h[2];
    TH2F* MEtot_v_MM2eggX_sig_h[2];
    TH1F* asym_h[2];

    asym_h[0] = (TH1F*) protasymfile->Get("Asym_R_N_h");
    asym_h[1] = (TH1F*) neutasymfile->Get("Asym_R_N_h");

    ggIM_raw_h[0] = (TH1F*) protfile->Get("ggIM_raw_h");
    ggIM_raw_h[1] = (TH1F*) neutfile->Get("ggIM_raw_h");
    ggIM_sig_h[0] = (TH1F*) protfile->Get("ggIM_3sigExcDis_h");
    ggIM_sig_h[1] = (TH1F*) neutfile->Get("ggIM_3sigExcDis_h");

    Q2_v_xB_raw_h[0] = (TH2F*) protfile->Get("Q2_v_xB_raw_h");
    Q2_v_xB_raw_h[1] = (TH2F*) neutfile->Get("Q2_v_xB_raw_h");
    
    IMgg_v_MM2tot_raw_h[0] = (TH2F*) protfile->Get("IMgg_v_MM2tot_raw_h");
    IMgg_v_MM2tot_raw_h[1] = (TH2F*) neutfile->Get("IMgg_v_MM2tot_raw_h");
    IMgg_v_MM2tot_sig_h[0] = (TH2F*) protfile->Get("IMgg_v_MM2tot_3sigExcDis_h");
    IMgg_v_MM2tot_sig_h[1] = (TH2F*) neutfile->Get("IMgg_v_MM2tot_3sigExcDis_h");

    MEtot_v_MM2eggX_raw_h[0] = (TH2F*) protfile->Get("MEtot_v_MM2eggX_h_raw_h");
    MEtot_v_MM2eggX_raw_h[1] = (TH2F*) neutfile->Get("MEtot_v_MM2eggX_h_raw_h");
    MEtot_v_MM2eggX_sig_h[0] = (TH2F*) protfile->Get("MEtot_v_MM2eggX_h_3sigExcDis_h");
    MEtot_v_MM2eggX_sig_h[1] = (TH2F*) neutfile->Get("MEtot_v_MM2eggX_h_3sigExcDis_h");

    TF1* IMgg_sig_fits[2];
    
    // TF1* IMgg_sig_signal[2];
    // TF1* IMgg_sig_background[2];

    IMgg_sig_fits[0] = new TF1("protIMgg_fit", "gaus + [3]", 0, 0.2);
	IMgg_sig_fits[0]->SetParNames("Amp.", "Mean", "#sigma", "BG_{1}");

	IMgg_sig_fits[0]->SetRange(0.08, 0.18);
	IMgg_sig_fits[0]->SetParameter(0, 1000);
	IMgg_sig_fits[0]->SetParLimits(0, 0, 10000);
	IMgg_sig_fits[0]->SetParameter(1, 0.135);
	IMgg_sig_fits[0]->SetParLimits(1, 0.11, 0.15);
	IMgg_sig_fits[0]->SetParameter(2, 0.11);
	IMgg_sig_fits[0]->SetParLimits(2, 0, 1);

    ggIM_sig_h[0]->Fit(IMgg_sig_fits[0], "NQR");
    IMgg_sig_fits[0]->SetRange(0.09, 0.17);
	ggIM_sig_h[0]->Fit(IMgg_sig_fits[0], "RM"); 

    IMgg_sig_fits[1] = new TF1("neutIMgg_fit", "gaus + [3]", 0, 0.2);
	IMgg_sig_fits[1]->SetParNames("Amp.", "Mean", "#sigma", "BG_{1}");

	IMgg_sig_fits[1]->SetRange(0.08, 0.18);
	IMgg_sig_fits[1]->SetParameter(0, 1000);
	IMgg_sig_fits[1]->SetParLimits(0, 0, 10000);
	IMgg_sig_fits[1]->SetParameter(1, 0.135);
	IMgg_sig_fits[1]->SetParLimits(1, 0.11, 0.15);
	IMgg_sig_fits[1]->SetParameter(2, 0.11);
	IMgg_sig_fits[1]->SetParLimits(2, 0, 1);

    ggIM_sig_h[1]->Fit(IMgg_sig_fits[1], "NQR");
    IMgg_sig_fits[1]->SetRange(0.09, 0.17);
	ggIM_sig_h[1]->Fit(IMgg_sig_fits[1], "RM"); 


    TCanvas* IMgg_raw_C = new TCanvas("IMgg_raw_C","Raw IMgg", 1200, 600);
    IMgg_raw_C->Divide(2,1);
    IMgg_raw_C->cd(1);
    ggIM_raw_h[0]->Draw();
    IMgg_raw_C->cd(2);
    ggIM_raw_h[1]->Draw();
    IMgg_raw_C->SaveAs("plots/neut/neutprot_compare/neutprot_IMgg_raw.png", "RECREATE");


    TCanvas* IMgg_sig_C = new TCanvas("IMgg_sig_C","Signal IMgg", 1200, 600);
    IMgg_sig_C->Divide(2,1);
    IMgg_sig_C->cd(1);
    ggIM_sig_h[0]->Draw();
    IMgg_sig_C->cd(2);
    ggIM_sig_h[1]->Draw();
    IMgg_sig_C->SaveAs("plots/neut/neutprot_compare/neutprot_IMgg_sig.png", "RECREATE");

    
    TCanvas* Q2_v_xB_raw_C = new TCanvas("Q2_v_xB_raw_C","Raw Q2 v xB", 1200, 600);
    Q2_v_xB_raw_C->Divide(2,1);
    Q2_v_xB_raw_C->cd(1);
    Q2_v_xB_raw_h[0]->Draw("colz");
    Q2_v_xB_raw_C->cd(2);
    Q2_v_xB_raw_h[1]->Draw("colz");
    Q2_v_xB_raw_C->SaveAs("plots/neut/neutprot_compare/neutprot_Q2xB_raw.png", "RECREATE");


    TCanvas* IMgg_v_MM2tot_raw_C = new TCanvas("IMgg_v_MM2tot_raw_C","Raw IMgg vs. MM2tot", 1200, 600);
    IMgg_v_MM2tot_raw_C->Divide(2,1);
    IMgg_v_MM2tot_raw_C->cd(1);
    IMgg_v_MM2tot_raw_h[0]->Draw("colz");
    IMgg_v_MM2tot_raw_C->cd(2);
    IMgg_v_MM2tot_raw_h[1]->Draw("colz");
    IMgg_v_MM2tot_raw_C->SaveAs("plots/neut/neutprot_compare/neutprot_IMgg_v_MM2tot_raw.png", "RECREATE");


    TCanvas* IMgg_v_MM2tot_sig_C = new TCanvas("IMgg_v_MM2tot_sig_C","Cut IMgg vs. MM2tot", 1200, 600);
    IMgg_v_MM2tot_sig_C->Divide(2,1);
    IMgg_v_MM2tot_sig_C->cd(1);
    IMgg_v_MM2tot_sig_h[0]->Draw("colz");
    IMgg_v_MM2tot_sig_C->cd(2);
    IMgg_v_MM2tot_sig_h[1]->Draw("colz");
    IMgg_v_MM2tot_sig_C->SaveAs("plots/neut/neutprot_compare/neutprot_IMgg_v_MM2tot_sig.png", "RECREATE");


    TCanvas* MEtot_v_MM2eggX_raw_C = new TCanvas("MEtot_v_MM2eggX_raw_C","Cut MEtot vs. MM2eggX", 1200, 600);
    MEtot_v_MM2eggX_raw_C->Divide(2,1);
    MEtot_v_MM2eggX_raw_C->cd(1);
    MEtot_v_MM2eggX_raw_h[0]->Draw("colz");
    MEtot_v_MM2eggX_raw_C->cd(2);
    MEtot_v_MM2eggX_raw_h[1]->Draw("colz");
    MEtot_v_MM2eggX_raw_C->SaveAs("plots/neut/neutprot_compare/neutprot_MEtot_v_MM2eggX_raw.png", "RECREATE");

    TCanvas* MEtot_v_MM2eggX_sig_C = new TCanvas("MEtot_v_MM2eggX_sig_C","Cut MEtot vs. MM2eggX", 1200, 600);
    MEtot_v_MM2eggX_sig_C->Divide(2,1);
    MEtot_v_MM2eggX_sig_C->cd(1);
    MEtot_v_MM2eggX_sig_h[0]->Draw("colz");
    MEtot_v_MM2eggX_sig_C->cd(2);
    MEtot_v_MM2eggX_sig_h[1]->Draw("colz");
    MEtot_v_MM2eggX_sig_C->SaveAs("plots/neut/neutprot_compare/neutprot_MEtot_v_MM2eggX_sig.png", "RECREATE");

    TCanvas* asym_C = new TCanvas("asym_C", "BSA", 1200, 600);
    asym_C->Divide(2,1);
    asym_C->cd(1);
    asym_h[0]->Draw();
    asym_C->cd(2);
    asym_h[1]->Draw();

    IMgg_raw_C->Write();
    IMgg_sig_C->Write();
    Q2_v_xB_raw_C->Write();
    IMgg_v_MM2tot_raw_C->Write();
    IMgg_v_MM2tot_sig_C->Write();
    MEtot_v_MM2eggX_raw_C->Write();
    MEtot_v_MM2eggX_sig_C->Write();
    asym_C->Write();

    OutFile->Close();

}