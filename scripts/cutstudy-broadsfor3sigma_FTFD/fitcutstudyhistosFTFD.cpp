// @(#)root
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include <TFile.h>
#include <TMath.h>
#include <TF1.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TText.h>
#include <TPaveStats.h>
#include <TString.h>

#include "clas12reader.h"
#include "cutstudyhistosFTFD.h"

using std::cout;
using std::endl;


void fitcutstudyhistosFTFD(TString inputFile) //TODO - Figure out less crude SBR, no fit yet for MM^2 ranges.
{
	TString outputFile = "fitted_"+inputFile;

	gStyle->SetOptFit(01111);
	gStyle->SetOptStat(11);
	//gStyle->SetOptFit(0);
	// gStyle->SetTitleFont(132,"a");
	// gStyle->SetTitleX(0.6); //title X location
	// gStyle->SetTitleY(1);   //title Y location
	// gStyle->SetTitleW(0.8); //title width
	// gStyle->SetTitleH(0.1); //title height

	TString fitfxn = "gaus(0)+pol3(3)";
	//TString fitfxn[6] = {"gaus(0)+pol2(3)","gaus(0)+pol2(3)","gaus(0)+pol3(3)"}; //
	Double_t cone_gausamp [4][3];
	Double_t cone_gausmean[4][3];
	Double_t cone_gaussig [4][3];
	Double_t cone_polconst[4][3];
	// Double_t cone_polx [4][3];
	// Double_t cone_polx2[4][3];
	Double_t cone_bg_const[4][3];

	/*=====Read histos from file=====*/
	cout << "Reading out histograms..." << endl;
	TFile* infile = new TFile(inputFile,"READ");

	ggIM_coneangle_h[0][0] = (TH1F*) infile->Get("ggIM_bothFD_coneangleL30_h");
	ggIM_coneangle_h[0][1] = (TH1F*) infile->Get("ggIM_bothFT_coneangleL30_h");
	ggIM_coneangle_h[0][2] = (TH1F*) infile->Get("ggIM_FDFT_coneangleL30_h");

	ggIM_coneangle_h[1][0] = (TH1F*) infile->Get("ggIM_bothFD_coneangleL20_h");
	ggIM_coneangle_h[1][1] = (TH1F*) infile->Get("ggIM_bothFT_coneangleL20_h");
	ggIM_coneangle_h[1][2] = (TH1F*) infile->Get("ggIM_FDFT_coneangleL20_h");

	ggIM_coneangle_h[2][0] = (TH1F*) infile->Get("ggIM_bothFD_coneangleL10_h");
	ggIM_coneangle_h[2][1] = (TH1F*) infile->Get("ggIM_bothFT_coneangleL10_h");
	ggIM_coneangle_h[2][2] = (TH1F*) infile->Get("ggIM_FDFT_coneangleL10_h");

	ggIM_coneangle_h[3][0] = (TH1F*) infile->Get("ggIM_bothFD_coneangleL8_h");
	ggIM_coneangle_h[3][1] = (TH1F*) infile->Get("ggIM_bothFT_coneangleL8_h");
	ggIM_coneangle_h[3][2] = (TH1F*) infile->Get("ggIM_FDFT_coneangleL8_h");

	TCanvas *ggIM_conecuts_c = new TCanvas("ggIM_conecuts_c", "#gamma_{1}#gamma_{2}-mass split by region (Cone angle < 30',20',10', 8')");

	TString namebuff;
  	TString base = "ggIM_";

	std::vector<TString> det  = {"bothFD_", "bothFT_", "FDFT_"};
	std::vector<TString> conecut = {"coneangleL30_h", "coneangleL20_h", "coneangleL10_h", "coneangleL8_h"};

	/*===== Fit Histograms =====*/
	TF1* ggIM_coneangle_fits[4][3];
	TF1* ggIM_coneangle_sig [4][3];
	TF1* ggIM_coneangle_bg  [4][3];
	TPaveStats* statspanel; //buffer for TPaneStats obj, to reposition/resize etc.

	cout << "Fitting distributions..." << endl;
	for (int i = 0; i < det.size(); i++){
		for (int j = 0; j < conecut.size(); j++) {
			// cout << "{setup}" << endl;
			namebuff = base + det[i] + conecut[j];

			ggIM_coneangle_fits[j][i] = new TF1(namebuff+"_fit", fitfxn, 0, 0.2);
			ggIM_coneangle_fits[j][i]->SetParNames("Amp.", "Mean", "#sigma", "BG_{1}", "BG_{2}", "BG_{3}", "BG_{4}");

			ggIM_coneangle_fits[j][i]->SetRange(0.08, 0.18);
			ggIM_coneangle_fits[j][i]->SetParameter(0, 1000);
			ggIM_coneangle_fits[j][i]->SetParLimits(0, 0, 10000);
			ggIM_coneangle_fits[j][i]->SetParameter(1, 0.135);
			ggIM_coneangle_fits[j][i]->SetParLimits(1, 0.11, 0.15);
			ggIM_coneangle_fits[j][i]->SetParameter(2, 0.11);
			ggIM_coneangle_fits[j][i]->SetParLimits(2, 0, 1);

			// cout << "{first fit}" << endl;
			ggIM_coneangle_h[j][i]->Fit(ggIM_coneangle_fits[j][i], "NQR"); //N: don't save gfx or draw; Q: Quiet

			/*Broaden range & re-fit*/
			// cout << "{refit}" << endl;
			ggIM_coneangle_fits[j][i]->SetRange(0.07, 0.2);
			ggIM_coneangle_h[j][i]->Fit(ggIM_coneangle_fits[j][i], "RM"); //removed M opt

			// cout << "{read into separate TF1s}" << endl;
			/*Extract signal fit*/
			ggIM_coneangle_sig [j][i] = new TF1(namebuff+"_sig", "gaus", 0, 0.2);
			ggIM_coneangle_sig [j][i]->SetParameter(0, ggIM_coneangle_fits[j][i]->GetParameter(0));
			ggIM_coneangle_sig [j][i]->SetParameter(1, ggIM_coneangle_fits[j][i]->GetParameter(1));
			ggIM_coneangle_sig [j][i]->SetParameter(2, ggIM_coneangle_fits[j][i]->GetParameter(2));

			/*Extract background fit (of N degree polynomial)*/
			int poldeg = ggIM_coneangle_fits[j][i]->GetNpar() - 4; //polynomial degree is N parameters of fit - 3(gaus) -1(const.)
			TString poldegstr = TString::Itoa(poldeg,10);

			ggIM_coneangle_bg  [j][i] = new TF1(namebuff+"_bg" , "pol"+poldegstr, 0, 0.2);
			for (int k = 0; k <= poldeg; k++) {
				ggIM_coneangle_bg  [j][i]->SetParameter(k, ggIM_coneangle_fits[j][i]->GetParameter(k+3));
			}

		}
	}

	cout << "Drawing canvas..." << endl;
	ggIM_conecuts_c->Clear(); //Clear canvas of any fit-drawings.
	ggIM_conecuts_c->Divide(3,4);
	int n=0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++){
			ggIM_conecuts_c->cd(n+1);
			ggIM_coneangle_h[i][j]->Draw();

			gPad->Update(); //allows following TPaveStats retrieval to be done.
			statspanel = (TPaveStats*) ggIM_coneangle_h[i][j]->FindObject("stats");
			statspanel->SetX1NDC(0.11);
			statspanel->SetX2NDC(0.45);
			statspanel->SetY1NDC(0.89);
			statspanel->SetY2NDC(0.35);

			//re-draw to refresh stats panel position on canvas
			ggIM_coneangle_h[i][j]->Draw();

			n++;
		}
	}


	TCanvas* C = new TCanvas("C");
	C->Divide(3);

	for (int i=0; i<3; i++){
		C->cd(i+1);
		ggIM_coneangle_fits[1][i]->SetLineColor(kBlack);
		
		ggIM_coneangle_h   [1][i]->Draw();  //fitted histo

		ggIM_coneangle_sig [1][i]->SetLineColor(kGreen);
		ggIM_coneangle_sig [1][i]->Draw("SAME"); //new signal function
		ggIM_coneangle_bg  [1][i]->SetLineColor(kOrange);
		ggIM_coneangle_bg  [1][i]->SetLineWidth(1);
		ggIM_coneangle_bg  [1][i]->Draw("SAME"); //new background

		double mean = ggIM_coneangle_sig[1][i]->GetParameter(1);
		double sig = ggIM_coneangle_sig[1][i]->GetParameter(2);

		cout << det[i] << "	Mean: " << mean << "  Sig: " << sig << "  Lower: " << mean - 3*sig << "  Upper: " << mean + 3*sig << endl;

	}
	//C->SetWindowSize(1920, 1080);
	//C->SetCanvasSize(1920, 1080);
	// 
	// TCanvas* D = new TCanvas("D");
	//

}//macro
