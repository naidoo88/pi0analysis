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
#include "cutstudyhistos.h"

using namespace clas12;

void fitcutstudyhistos(TString inputFile){
	TString outputFile = "fitted_"+inputFile;

	gStyle->SetOptFit(01111);
	gStyle->SetOptStat(0);
	gStyle->SetOptFit(0);
	// gStyle->SetTitleFont(132,"a");
	// gStyle->SetTitleX(0.6); //title X location
	// gStyle->SetTitleY(1);   //title Y location
	// gStyle->SetTitleW(0.8); //title width
	// gStyle->SetTitleH(0.1); //title height

	TString fitfxn = "gaus(0)+pol2(3)";
	Double_t gausamp  = 0;
	Double_t gausmean = 0;
	Double_t gaussig  = 0;
	Double_t polconst = 0;
	Double_t polx     = 0;
	Double_t polx2    = 0;


  /*=====Read histos from file=====*/
	TFile* infile = new TFile(inputFile,"READ");

	ggIM_coneangle_h[0][0] = (TH1F*) infile->Get("ggIM_bothPCAL_coneangleL30_h");
	ggIM_coneangle_h[0][1] = (TH1F*) infile->Get("ggIM_bothECAL_coneangleL30_h");
	ggIM_coneangle_h[0][2] = (TH1F*) infile->Get("ggIM_bothFCAL_coneangleL30_h");
	ggIM_coneangle_h[0][3] = (TH1F*) infile->Get("ggIM_PCALECAL_coneangleL30_h");
	ggIM_coneangle_h[0][4] = (TH1F*) infile->Get("ggIM_ECALFCAL_coneangleL30_h");
	ggIM_coneangle_h[0][5] = (TH1F*) infile->Get("ggIM_PCALFCAL_coneangleL30_h");

	ggIM_coneangle_h[1][0] = (TH1F*) infile->Get("ggIM_bothPCAL_coneangleL20_h");
	ggIM_coneangle_h[1][1] = (TH1F*) infile->Get("ggIM_bothECAL_coneangleL20_h");
	ggIM_coneangle_h[1][2] = (TH1F*) infile->Get("ggIM_bothFCAL_coneangleL20_h");
	ggIM_coneangle_h[1][3] = (TH1F*) infile->Get("ggIM_PCALECAL_coneangleL20_h");
	ggIM_coneangle_h[1][4] = (TH1F*) infile->Get("ggIM_ECALFCAL_coneangleL20_h");
	ggIM_coneangle_h[1][5] = (TH1F*) infile->Get("ggIM_PCALFCAL_coneangleL20_h");

	ggIM_coneangle_h[2][0] = (TH1F*) infile->Get("ggIM_bothPCAL_coneangleL10_h");
	ggIM_coneangle_h[2][1] = (TH1F*) infile->Get("ggIM_bothECAL_coneangleL10_h");
	ggIM_coneangle_h[2][2] = (TH1F*) infile->Get("ggIM_bothFCAL_coneangleL10_h");
	ggIM_coneangle_h[2][3] = (TH1F*) infile->Get("ggIM_PCALECAL_coneangleL10_h");
	ggIM_coneangle_h[2][4] = (TH1F*) infile->Get("ggIM_ECALFCAL_coneangleL10_h");
	ggIM_coneangle_h[2][5] = (TH1F*) infile->Get("ggIM_PCALFCAL_coneangleL10_h");

	ggIM_coneangle_h[3][0] = (TH1F*) infile->Get("ggIM_bothPCAL_coneangleL8_h");
	ggIM_coneangle_h[3][1] = (TH1F*) infile->Get("ggIM_bothECAL_coneangleL8_h");
	ggIM_coneangle_h[3][2] = (TH1F*) infile->Get("ggIM_bothFCAL_coneangleL8_h");
	ggIM_coneangle_h[3][3] = (TH1F*) infile->Get("ggIM_PCALECAL_coneangleL8_h");
	ggIM_coneangle_h[3][4] = (TH1F*) infile->Get("ggIM_ECALFCAL_coneangleL8_h");
	ggIM_coneangle_h[3][5] = (TH1F*) infile->Get("ggIM_PCALFCAL_coneangleL8_h");

	ggIM_coneangle_h[4][0] = (TH1F*) infile->Get("ggIM_bothPCAL_coneangleL5_h");
	ggIM_coneangle_h[4][1] = (TH1F*) infile->Get("ggIM_bothECAL_coneangleL5_h");
	ggIM_coneangle_h[4][2] = (TH1F*) infile->Get("ggIM_bothFCAL_coneangleL5_h");
	ggIM_coneangle_h[4][3] = (TH1F*) infile->Get("ggIM_PCALECAL_coneangleL5_h");
	ggIM_coneangle_h[4][4] = (TH1F*) infile->Get("ggIM_ECALFCAL_coneangleL5_h");
	ggIM_coneangle_h[4][5] = (TH1F*) infile->Get("ggIM_PCALFCAL_coneangleL5_h");

	TString namebuff;
  TString base         = "ggIM_";
  vector<TString> det  = {"bothPCAL_", "bothECAL_", "bothFCAL_", "PCALECAL_", "ECALFCAL_", "PCALFCAL_"};
  vector<TString> mm2cut  = {"mm2PM0.4_h", "mm2PM0.3_h","mm2PM0.2_h"};

  for (int i = 0; i < det.size(); i++){
    for (int j = 0; j < mm2cut.size(); j++) {
      namebuff = base + det[i] + mm2cut[j];
      ggIM_mm2_h[j][i] = (TH1F*) infile->Get(namebuff);
    }
  }
	ggIM_mm2_h[0][0]->Draw();

	TCanvas *ggIM_conecuts_c = new TCanvas("ggIM_conecuts_c", "#gamma_{1}#gamma_{2}-mass split by region (Cone angle < 30',20',10', 8', 5')");

	TF1* ggIM_coneangle_fits[5][6];
	TF1* ggIM_mm2_fits[3][6];

	ggIM_coneangle_fits[0][0] = new TF1("ggIM_bothPCAL_coneangleL30_fit", fitfxn, 0, 0.2);
	ggIM_coneangle_fits[0][0]->SetRange(0.08, 0.18);
	ggIM_coneangle_fits[0][0]->SetParameter(1, 0.135);
	ggIM_coneangle_fits[0][0]->SetParLimits(1, 0.12, 0.14);
	ggIM_coneangle_fits[0][0]->SetParameter(2, 0.1);
	ggIM_coneangle_fits[0][0]->SetParLimits(2, -1, 1);

	ggIM_coneangle_h[0][0]->Fit(ggIM_coneangle_fits[0][0], "R");

	ggIM_conecuts_c->Clear(); //Clear canvas of any fit-drawings.
	ggIM_conecuts_c->Divide(6,5);
	int n=0;
	for (int i = 0; i <= 4; i++) {
		for (int j = 0; j <= 5; j++){
			ggIM_conecuts_c->cd(n+1);
			ggIM_coneangle_h[i][j]->Draw();
			n++;
		}
	}

}//macro
