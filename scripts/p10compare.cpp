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

void p10compare()
{
    gStyle->SetOptFit(01111);
	gStyle->SetOptStat(11);

    TFile* pass0file = new TFile("plots/plots35.root", "READ");
    TFile* pass1file = new TFile("plots/p1plots35.root", "READ");
    TFile* pass0asymfile = new TFile("output/asymmetries/assym_fitted_9bins.root", "READ");
    TFile* pass1asymfile = new TFile("plots/p1asym35.root", "READ");

    TFile* pass1allfile = new TFile("plots/plots.root", "READ");
    TFile* p1asymallfile = new TFile("plots/p1asymall.root", "READ");
    
    TFile* OutFile = new TFile("plots/pass10compare/p10compare.root", "RECREATE");

    TH1F* ggIM_raw_h[2];
    TH1F* ggIM_sig_h[2];
    TH2F* Q2_v_xB_raw_h[2];
    TH2F* IMgg_v_MM2tot_raw_h[2];
    TH2F* IMgg_v_MM2tot_sig_h[2];
    TH2F* MEtot_v_MM2eggX_raw_h[2];
    TH2F* MEtot_v_MM2eggX_sig_h[2];

    TH1F* ggIM_fullsig_h;
    TH1F* fullasym_h;
    TH1F* asym_h[2];

    ggIM_fullsig_h = (TH1F*) pass1allfile->Get("ggIM_3sigExcDis_h");
    fullasym_h = (TH1F*) p1asymallfile->Get("Asym_R_N_h");

    asym_h[0] = (TH1F*) pass0asymfile->Get("Asym_R_N_h");
    asym_h[1] = (TH1F*) pass1asymfile->Get("Asym_R_N_h");

    ggIM_raw_h[0] = (TH1F*) pass0file->Get("ggIM_raw_h");
    ggIM_raw_h[1] = (TH1F*) pass1file->Get("ggIM_raw_h");
    ggIM_sig_h[0] = (TH1F*) pass0file->Get("ggIM_3sigExcDis_h");
    ggIM_sig_h[1] = (TH1F*) pass1file->Get("ggIM_3sigExcDis_h");

    Q2_v_xB_raw_h[0] = (TH2F*) pass0file->Get("Q2_v_xB_raw_h");
    Q2_v_xB_raw_h[1] = (TH2F*) pass1file->Get("Q2_v_xB_raw_h");
    
    IMgg_v_MM2tot_raw_h[0] = (TH2F*) pass0file->Get("IMgg_v_MM2tot_raw_h");
    IMgg_v_MM2tot_raw_h[1] = (TH2F*) pass1file->Get("IMgg_v_MM2tot_raw_h");
    IMgg_v_MM2tot_sig_h[0] = (TH2F*) pass0file->Get("IMgg_v_MM2tot_3sigExcDis_h");
    IMgg_v_MM2tot_sig_h[1] = (TH2F*) pass1file->Get("IMgg_v_MM2tot_3sigExcDis_h");

    MEtot_v_MM2eggX_raw_h[0] = (TH2F*) pass0file->Get("MEtot_v_MM2eggX_h_raw_h");
    MEtot_v_MM2eggX_raw_h[1] = (TH2F*) pass1file->Get("MEtot_v_MM2eggX_h_raw_h");
    MEtot_v_MM2eggX_sig_h[0] = (TH2F*) pass0file->Get("MEtot_v_MM2eggX_h_3sigExcDis_h");
    MEtot_v_MM2eggX_sig_h[1] = (TH2F*) pass1file->Get("MEtot_v_MM2eggX_h_3sigExcDis_h");

    TF1* IMgg_sig_fits[2];
    TF1* IMgg_fullsig_fits;
    
    // TF1* IMgg_sig_signal[2];
    // TF1* IMgg_sig_background[2];

    IMgg_sig_fits[0] = new TF1("p0IMgg_fit", "gaus + [3]", 0, 0.2);
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

    IMgg_sig_fits[1] = new TF1("p1IMgg_fit", "gaus + [3]", 0, 0.2);
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


    IMgg_fullsig_fits = new TF1("p1IMggfull_fit", "gaus + [3]", 0, 0.2);
	IMgg_fullsig_fits->SetParNames("Amp.", "Mean", "#sigma", "BG_{1}");

	IMgg_fullsig_fits->SetRange(0.08, 0.18);
	IMgg_fullsig_fits->SetParameter(0, 1000);
	IMgg_fullsig_fits->SetParLimits(0, 0, 10000);
	IMgg_fullsig_fits->SetParameter(1, 0.135);
	IMgg_fullsig_fits->SetParLimits(1, 0.11, 0.15);
	IMgg_fullsig_fits->SetParameter(2, 0.11);
	IMgg_fullsig_fits->SetParLimits(2, 0, 1);

    ggIM_fullsig_h->Fit(IMgg_fullsig_fits, "NQR");
    IMgg_fullsig_fits->SetRange(0.09, 0.17);
	ggIM_fullsig_h->Fit(IMgg_fullsig_fits, "RM"); 


    TCanvas* IMgg_raw_C = new TCanvas("IMgg_raw_C","Raw IMgg", 1200, 600);
    IMgg_raw_C->Divide(2,1);
    IMgg_raw_C->cd(1);
    ggIM_raw_h[0]->Draw();
    IMgg_raw_C->cd(2);
    ggIM_raw_h[1]->Draw();
    IMgg_raw_C->SaveAs("plots/pass10compare/p10compare_IMgg_raw.png", "RECREATE");


    TCanvas* IMgg_sig_C = new TCanvas("IMgg_sig_C","Signal IMgg", 1200, 600);
    IMgg_sig_C->Divide(2,1);
    IMgg_sig_C->cd(1);
    ggIM_sig_h[0]->Draw();
    IMgg_sig_C->cd(2);
    ggIM_sig_h[1]->Draw();
    IMgg_sig_C->SaveAs("plots/pass10compare/p10compare_IMgg_sig.png", "RECREATE");

    
    TCanvas* Q2_v_xB_raw_C = new TCanvas("Q2_v_xB_raw_C","Raw Q2 v xB", 1200, 600);
    Q2_v_xB_raw_C->Divide(2,1);
    Q2_v_xB_raw_C->cd(1);
    Q2_v_xB_raw_h[0]->Draw("colz");
    Q2_v_xB_raw_C->cd(2);
    Q2_v_xB_raw_h[1]->Draw("colz");
    Q2_v_xB_raw_C->SaveAs("plots/pass10compare/p10compare_Q2xB_raw.png", "RECREATE");


    TCanvas* IMgg_v_MM2tot_raw_C = new TCanvas("IMgg_v_MM2tot_raw_C","Raw IMgg vs. MM2tot", 1200, 600);
    IMgg_v_MM2tot_raw_C->Divide(2,1);
    IMgg_v_MM2tot_raw_C->cd(1);
    IMgg_v_MM2tot_raw_h[0]->Draw("colz");
    IMgg_v_MM2tot_raw_C->cd(2);
    IMgg_v_MM2tot_raw_h[1]->Draw("colz");
    IMgg_v_MM2tot_raw_C->SaveAs("plots/pass10compare/p10compare_IMgg_v_MM2tot_raw.png", "RECREATE");


    TCanvas* IMgg_v_MM2tot_sig_C = new TCanvas("IMgg_v_MM2tot_sig_C","Cut IMgg vs. MM2tot", 1200, 600);
    IMgg_v_MM2tot_sig_C->Divide(2,1);
    IMgg_v_MM2tot_sig_C->cd(1);
    IMgg_v_MM2tot_sig_h[0]->Draw("colz");
    IMgg_v_MM2tot_sig_C->cd(2);
    IMgg_v_MM2tot_sig_h[1]->Draw("colz");
    IMgg_v_MM2tot_sig_C->SaveAs("plots/pass10compare/p10compare_IMgg_v_MM2tot_sig.png", "RECREATE");


    TCanvas* MEtot_v_MM2eggX_raw_C = new TCanvas("MEtot_v_MM2eggX_raw_C","Cut MEtot vs. MM2eggX", 1200, 600);
    MEtot_v_MM2eggX_raw_C->Divide(2,1);
    MEtot_v_MM2eggX_raw_C->cd(1);
    MEtot_v_MM2eggX_raw_h[0]->Draw("colz");
    MEtot_v_MM2eggX_raw_C->cd(2);
    MEtot_v_MM2eggX_raw_h[1]->Draw("colz");
    MEtot_v_MM2eggX_raw_C->SaveAs("plots/pass10compare/p10compare_MEtot_v_MM2eggX_raw.png", "RECREATE");

    TCanvas* MEtot_v_MM2eggX_sig_C = new TCanvas("MEtot_v_MM2eggX_sig_C","Cut MEtot vs. MM2eggX", 1200, 600);
    MEtot_v_MM2eggX_sig_C->Divide(2,1);
    MEtot_v_MM2eggX_sig_C->cd(1);
    MEtot_v_MM2eggX_sig_h[0]->Draw("colz");
    MEtot_v_MM2eggX_sig_C->cd(2);
    MEtot_v_MM2eggX_sig_h[1]->Draw("colz");
    MEtot_v_MM2eggX_sig_C->SaveAs("plots/pass10compare/p10compare_MEtot_v_MM2eggX_sig.png", "RECREATE");

    TCanvas* asym_C = new TCanvas("asum_C", "BSA", 1200, 600);
    asym_C->Divide(2,1);
    asym_C->cd(1);
    asym_h[0]->Draw();
    asym_C->cd(2);
    asym_h[1]->Draw();

    TCanvas* IMasym_all_C = new TCanvas("IMasym_all_C", "All files", 1200, 600);
    IMasym_all_C->Divide(2,1);
    IMasym_all_C->cd(1);
    ggIM_fullsig_h->Draw();
    IMasym_all_C->cd(2);
    fullasym_h->Draw();

    // ggIM_raw_h[0]->Write();
    // ggIM_raw_h[1]->Write();
    // ggIM_sig_h[0]->Write();
    // ggIM_sig_h[1]->Write();
    // Q2_v_xB_raw_h[0]->Write();
    // Q2_v_xB_raw_h[1]->Write();
    // IMgg_v_MM2tot_raw_h[0]->Write();
    // IMgg_v_MM2tot_raw_h[1]->Write();
    // IMgg_v_MM2tot_sig_h[0]->Write();
    // IMgg_v_MM2tot_sig_h[1]->Write();
    // MEtot_v_MM2eggX_raw_h[0]->Write();
    // MEtot_v_MM2eggX_raw_h[1]->Write();
    // MEtot_v_MM2eggX_sig_h[0]->Write();
    // MEtot_v_MM2eggX_sig_h[1]->Write();

    IMgg_raw_C->Write();
    IMgg_sig_C->Write();
    Q2_v_xB_raw_C->Write();
    IMgg_v_MM2tot_raw_C->Write();
    IMgg_v_MM2tot_sig_C->Write();
    MEtot_v_MM2eggX_raw_C->Write();
    MEtot_v_MM2eggX_sig_C->Write();
    asym_C->Write();
    IMasym_all_C->Write();

    OutFile->Close();

}