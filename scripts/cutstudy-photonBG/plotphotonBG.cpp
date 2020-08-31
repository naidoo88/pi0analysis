// @(#)root
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "TFile.h"
#include "TMath.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TText.h"
#include "TPaveStats.h"
#include "TSystem.h"

using std::cout;
using std::endl;

//#include "/home/pauln/code/pi0analysis/scripts/hist_fxns.h"
//#include "/home/pauln/code/pi0analysis/scripts/hist_fxns.C"

void plotphotonBG(TString inputFile){
  cout << "TOP" << endl << endl;
  //hist_fxns h;

  TString outputFile = "plotted_"+inputFile;

  TH1F* IMgg_Ebins_h[5][2];
  TH1F* IMgg_EbinsComb_h[5];
  TH1F* IMgg_EthreshComb_h[6];
  TH1F* IMgg_egconethreshComb_h[7];

  /*=====Read histos from file=====*/
	cout << "Reading out histograms..." << endl;
	TFile* infile = new TFile(inputFile,"READ");

  IMgg_Ebins_h[0][0] = (TH1F*) infile -> Get("IM_gg_g1_E0-0.5");
  IMgg_Ebins_h[0][1] = (TH1F*) infile -> Get("IM_gg_g2_E0-0.5");
  IMgg_Ebins_h[1][0] = (TH1F*) infile -> Get("IM_gg_g1_E0.5-1");
  IMgg_Ebins_h[1][1] = (TH1F*) infile -> Get("IM_gg_g2_E0.5-1");
  IMgg_Ebins_h[2][0] = (TH1F*) infile -> Get("IM_gg_g1_E1-1.5");
  IMgg_Ebins_h[2][1] = (TH1F*) infile -> Get("IM_gg_g2_E1-1.5");
  IMgg_Ebins_h[3][0] = (TH1F*) infile -> Get("IM_gg_g1_E1.5-2.5");
  IMgg_Ebins_h[3][1] = (TH1F*) infile -> Get("IM_gg_g2_E1.5-2.5");
  IMgg_Ebins_h[4][0] = (TH1F*) infile -> Get("IM_gg_g1_Eover2.5");
  IMgg_Ebins_h[4][1] = (TH1F*) infile -> Get("IM_gg_g2_Eover2.5");

  IMgg_EbinsComb_h[0] = (TH1F*) infile -> Get("IM_gg_E0-0.5");
  IMgg_EbinsComb_h[1] = (TH1F*) infile -> Get("IM_gg_E0.5-1");
  IMgg_EbinsComb_h[2] = (TH1F*) infile -> Get("IM_gg_E1-1.5");
  IMgg_EbinsComb_h[3] = (TH1F*) infile -> Get("IM_gg_E1.5-2.5");
  IMgg_EbinsComb_h[4] = (TH1F*) infile -> Get("IM_gg_Eover2.5");

  IMgg_EthreshComb_h[0] = (TH1F*) infile -> Get("IM_gg_E>0");
  IMgg_EthreshComb_h[1] = (TH1F*) infile -> Get("IM_gg_E>0.5");
  IMgg_EthreshComb_h[2] = (TH1F*) infile -> Get("IM_gg_E>1");
  IMgg_EthreshComb_h[3] = (TH1F*) infile -> Get("IM_gg_E>1.5");
  IMgg_EthreshComb_h[4] = (TH1F*) infile -> Get("IM_gg_E>2");
  IMgg_EthreshComb_h[5] = (TH1F*) infile -> Get("IM_gg_E>2.5");

  IMgg_egconethreshComb_h[0] = (TH1F*) infile -> Get("IM_gg_eg_cone>0");
  IMgg_egconethreshComb_h[1] = (TH1F*) infile -> Get("IM_gg_eg_cone>2.5");
  IMgg_egconethreshComb_h[2] = (TH1F*) infile -> Get("IM_gg_eg_cone>7");
  IMgg_egconethreshComb_h[3] = (TH1F*) infile -> Get("IM_gg_eg_cone>15");
  IMgg_egconethreshComb_h[4] = (TH1F*) infile -> Get("IM_gg_eg_cone>25");
  IMgg_egconethreshComb_h[5] = (TH1F*) infile -> Get("IM_gg_eg_cone>35");
  IMgg_egconethreshComb_h[6] = (TH1F*) infile -> Get("IM_gg_eg_cone>45");


  TCanvas *Ebins_C = new TCanvas("Ebins_C", "IM_{#gamma#gamma} for bins of E_{#gamma}");
  Ebins_C->Divide(5,2);
  int panel = 1;
  for (u_int i = 0; i < 5; i++){
    Ebins_C->cd(panel);
    IMgg_Ebins_h[i][0]->Draw();

    Ebins_C->cd(panel+5);
    IMgg_Ebins_h[i][1]->Draw();

    panel++;
  }

  TCanvas *EbinsComb_C = new TCanvas("EbinsComb_C", "IM_{#gamma#gamma} for bins of E_{#gamma}");
  EbinsComb_C->Divide(5);
  panel = 1;
  for (u_int i = 0; i < 5; i++){
    EbinsComb_C->cd(panel);
    IMgg_EbinsComb_h[i]->Draw();

    panel++;
  }

  TCanvas *EthreshComb_C = new TCanvas("EthreshComb_C", "IM_{#gamma#gamma} for thresholds of E_{#gamma}");
  EthreshComb_C->Divide(5);
  panel = 1;
  for (u_int i = 0; i < 5; i++){
    EthreshComb_C->cd(panel);
    IMgg_EthreshComb_h[i]->Draw();

    panel++;
  }

  TCanvas *egconethreshComb_C = new TCanvas("egconethreshComb_C", "IM_{#gamma#gamma} for thresholds of eg-coneangle");
  egconethreshComb_C->Divide(5);
  panel = 1;
  for (u_int i = 0; i < 5; i++){
    egconethreshComb_C->cd(panel);
    IMgg_egconethreshComb_h[i]->Draw();

    panel++;
  }

cout << "END" << endl << endl;

}//fxn



      
       


  // vector<vector<double>> Eg_bins = {{0.0, 0.5}, {0.5, 1.0}, {1.0, 1.5}, {1.5, 2.5}, {2.5, 100}};
  // vector<TString> Ebins{"E0-0.5","E0.5-1","E1-1.5", "E1.5-2.5", "Eover2.5"};
  // vector<TString> Etitles{"E_{#gamma} 0-0.5GeV","E_{#gamma} 0.5-1GeV","E_{#gamma} 1-1.5GeV","E_{#gamma} 1.5-2.5GeV","E_{#gamma} > 2.5GeV"};
  // vector<TString> Photon{"g1","g2"};
  // vector<TString> Photontitles{"#gamma_{1}","#gamma_{2}"};
  // float options[3] = {200,0.001,0.2};
  // cout << "PREGEN" << endl << endl;
  // auto IMgg_Ebins_h = h.createTH1Array2D("IM_gg", options,"IM_{#gamma#gamma}", Ebins, Etitles, Photon, Photontitles);
  // auto IMgg_EbinsComb_h = h.createTH1Array1D("IM_gg", options,"IM_{#gamma#gamma}", Ebins, Etitles);
  // cout << "POSTGEN" << endl << endl;


  // // ROOT INCLUDE PATH????
  // for (auto hist_array : IMgg_Ebins_h){
  //   for (auto hist : hist_array){
  //     cout << hist.GetName() << endl;
  //     //hist = (TH1F) infile->Get(hist.GetName());
  //   }
  // }




