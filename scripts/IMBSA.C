{
    TFile* protfile = new TFile("plots/prot/p1plots.root", "READ");
    TFile* neutfile = new TFile("plots/neut/neut_plots.root", "READ");
    TFile* protasymfile = new TFile("plots/prot/p1helasymall.root", "READ");
    TFile* neutasymfile = new TFile("plots/neut/neut_plotsn.root", "READ");

    TH1F* ggIM_prot_h;
    TH1F* ggIM_neut_h;
    TH1F* asym_prot_h;
    TH1F* asym_neut_h;

    ggIM_prot_h = (TH1F*) protfile->Get("ggIM_3sigExc_h");
    ggIM_neut_h = (TH1F*) neutfile->Get("ggIM_3sigExc_h");
    asym_prot_h = (TH1F*) protasymfile->Get("Asym_R_N_h");
    asym_neut_h = (TH1F*) neutasymfile->Get("Asym_R_N_h");

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

    ggIM_prot_h->Fit(IMgg_sig_fits[0], "NQR");
    IMgg_sig_fits[0]->SetRange(0.09, 0.17);
	ggIM_prot_h->Fit(IMgg_sig_fits[0], "RM"); 

    IMgg_sig_fits[1] = new TF1("neutIMgg_fit", "gaus + [3]", 0, 0.2);
	IMgg_sig_fits[1]->SetParNames("Amp.", "Mean", "#sigma", "BG_{1}");

	IMgg_sig_fits[1]->SetRange(0.08, 0.18);
	IMgg_sig_fits[1]->SetParameter(0, 1000);
	IMgg_sig_fits[1]->SetParLimits(0, 0, 10000);
	IMgg_sig_fits[1]->SetParameter(1, 0.135);
	IMgg_sig_fits[1]->SetParLimits(1, 0.11, 0.15);
	IMgg_sig_fits[1]->SetParameter(2, 0.11);
	IMgg_sig_fits[1]->SetParLimits(2, 0, 1);

    ggIM_neut_h->Fit(IMgg_sig_fits[1], "NQR");
    IMgg_sig_fits[1]->SetRange(0.09, 0.17);
	ggIM_neut_h->Fit(IMgg_sig_fits[1], "RM"); 

    TLatex* tex = new TLatex(0.,3.,"Preliminary");
    tex->SetTextColor(17);
    tex->SetTextSize(0.1991525);
    tex->SetTextAngle(26.15998);
    tex->SetLineWidth(2);


    TCanvas* P = new TCanvas("P", "Protons", 1400, 600);
    P->Divide(2,1);
    P->cd(1);
    ggIM_prot_h->Draw();
    tex->Draw("SAME");
    P->cd(2);
    asym_prot_h->Draw();
    tex->Draw("SAME");

   
   //gr->Draw("LP");
   //gPad->RedrawAxis();

    TCanvas* N = new TCanvas("N", "Neutrons", 1400, 600);
    N->Divide(2,1);
    N->cd(1);
    ggIM_neut_h->Draw();
    tex->Draw("SAME");
    N->cd(2);
    asym_neut_h->Draw();
    tex->Draw("SAME");

    TFile* outfile = new TFile("./IMBSAplots.root", "RECREATE");
    P->Write();
    N->Write();
}