#include <vector>

#include "TFile.h"
#include "TChain.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TString.h"

using std::vector;
using std::cout;
using std::endl;
using TH1Array1D = vector<TH1F>;
using TH1Array2D = vector<TH1Array1D>;

TH1Array1D createTH1Array1D(TString, float[3], TString, vector<TString>, vector<TString>);
TH1Array2D createTH1Array2D(TString, float[3], TString, vector<TString>, vector<TString>, vector<TString>, vector<TString>);
void writeHistos(TH1Array1D &);
void writeHistos(TH1Array2D &);

void photonBG(TString infile, TString outfile)
{
	TChain chain("data");
	chain.AddFile(infile);

	double Q2;
	double xB;
	double IM_g1g2;
	double MM2_total;
	double ME_total;
	double MM2_rec_recoil;
	double MP_rec_spectator;
	double pi0coneangle;
	double phot1_E;
	double phot2_E;
	double eg1coneangle;
	double eg2coneangle;
	bool flag_2photon_event;

	chain.SetBranchAddress("Q2", &Q2);
	chain.SetBranchAddress("xB", &xB);
	chain.SetBranchAddress("IM_g1g2", &IM_g1g2);
	chain.SetBranchAddress("MM2_total", &MM2_total);
	chain.SetBranchAddress("ME_total", &ME_total);
	chain.SetBranchAddress("MM2_rec_recoil", &MM2_rec_recoil);
	chain.SetBranchAddress("MP_rec_spectator", &MP_rec_spectator);
	chain.SetBranchAddress("pi0coneangle", &pi0coneangle);
	chain.SetBranchAddress("phot1_E", &phot1_E);
	chain.SetBranchAddress("phot2_E", &phot2_E);
	chain.SetBranchAddress("eg1coneangle", &eg1coneangle);
	chain.SetBranchAddress("eg2coneangle", &eg2coneangle);
	chain.SetBranchAddress("flag_2photon_event", &flag_2photon_event);
	
	vector<vector<double>> Eg_bins = {{0.0, 0.5}, {0.5, 1.0}, {1.0, 1.5}, 
									  {1.5, 2.5}, {2.5, 100}};

	vector<TString> Ebins{"E0-0.5","E0.5-1","E1-1.5", "E1.5-2.5", "Eover2.5"};
	vector<TString> Etitles{"E_{#gamma} 0-0.5GeV","E_{#gamma} 0.5-1GeV","E_{#gamma} 1-1.5GeV","E_{#gamma} 1.5-2.5GeV","E_{#gamma} > 2.5GeV"};
	vector<TString> Photon{"g1","g2"};
	vector<TString> Photontitles{"#gamma_{1}","#gamma_{2}"};
	float options[3] = {200,0.001,0.2};
	auto IMgg_Ebins_h = createTH1Array2D("IM_gg", options,"IM_{#gamma#gamma}", Ebins, Etitles, Photon, Photontitles);
	auto IMgg_EbinsComb_h = createTH1Array1D("IM_gg", options,"IM_{#gamma#gamma}", Ebins, Etitles);

	vector<double> Eg_thresh = {0, 0.5, 1, 1.5, 2, 2.5};
	vector<TString> Ethresh{"E>0","E>0.5","E>1","E>1.5","E>2", "E>2.5"};
	vector<TString> Ethresh_titles{"E_{#gamma}>0","E_{#gamma}>0.5","E_{#gamma}>1","E_{#gamma}>1.5","E_{#gamma}>2", "E_{#gamma}>2.5"};
	auto IMgg_Ethresh_h = createTH1Array2D("IM_gg", options, "IM_{#gamma#gamma}", Ethresh, Ethresh_titles, Photon, Photontitles);
	auto IMgg_EthreshComb_h = createTH1Array1D("IM_gg", options,"IM_{#gamma#gamma}", Ethresh, Ethresh_titles);

	vector<vector<double>> egcone_bins = {{0.0, 2.5}, {2.5, 7}, {7, 15}, {15, 25}, {25, 35},{35, 45}, {45, 360}};
	vector<TString> egconebins = {"eg_cone_0-2.5", "eg_cone_2.5-7", "eg_cone_7-15", "eg_cone_15-25", "eg_cone_25-35", "eg_cone_35-45", "eg_cone_>45"};
	vector<TString> egconebins_titles = {"#theta_{e#gamma} 0-2.5", "#theta_{e#gamma} 2.5-7", "#theta_{e#gamma} 7-15", "#theta_{e#gamma} 15-25", "#theta_{e#gamma} 25-35", "#theta_{e#gamma} 35-45", "#theta_{e#gamma} > 45"};
	auto IMgg_egconebins_h = createTH1Array2D("IM_gg", options, "IM_{#gamma#gamma}", egconebins, egconebins_titles, Photon, Photontitles);
	auto IMgg_egconebinsComb_h = createTH1Array1D("IM_gg", options, "IM_{#gamma#gamma}", egconebins, egconebins_titles);

	vector<double> egcone_thresh = {0, 2.5, 7, 15, 25, 35, 45};
	vector<TString> egconethresh = {"eg_cone>0", "eg_cone>2.5", "eg_cone>7", "eg_cone>15", "eg_cone>25", "eg_cone>35", "eg_cone>45"};
	vector<TString> egconethresh_titles = {"#theta_{e#gamma}>0", "#theta_{e#gamma}>2.5", "#theta_{e#gamma}>7", "#theta_{e#gamma}>15", "#theta_{e#gamma}>25", "#theta_{e#gamma}>35", "#theta_{e#gamma}>45"};
	auto IMgg_egconethresh_h = createTH1Array2D("IM_gg", options, "IM_{#gamma#gamma}", egconethresh, egconethresh_titles, Photon, Photontitles);
	auto IMgg_egconethreshComb_h = createTH1Array1D("IM_gg", options, "IM_{#gamma#gamma}", egconethresh, egconethresh_titles);

	TH2F Eg_egcone_corr_h = TH2F("Eg_egcone_corr_h", "E_{#gamma} vs. #theta_{e#gamma}", 60, 0, 60, 100, 0, 10);

	for (u_int i = 0; i < chain.GetEntries(); i++)
	{
		chain.GetEntry(i);
		
		if(flag_2photon_event == 1)
		{
			for (u_int e = 0; e < Eg_bins.size(); e++)
			{
				if(phot1_E > Eg_bins[e][0] && phot1_E < Eg_bins[e][1] && IM_g1g2 != 0) IMgg_Ebins_h[e][0].Fill(IM_g1g2);
				if(phot2_E > Eg_bins[e][0] && phot2_E < Eg_bins[e][1] && IM_g1g2 != 0) IMgg_Ebins_h[e][1].Fill(IM_g1g2);

				if((phot1_E > Eg_bins[e][0] && phot1_E < Eg_bins[e][1]) && (phot2_E > Eg_bins[e][0] && phot2_E < Eg_bins[e][1]) && IM_g1g2 != 0)
				{
					IMgg_EbinsComb_h[e].Fill(IM_g1g2);
				}
			}

			for (u_int e = 0; e < Eg_thresh.size(); e++)
			{
				if(phot1_E > Eg_thresh[e] && IM_g1g2 != 0) IMgg_Ethresh_h[e][0].Fill(IM_g1g2);
				if(phot2_E > Eg_thresh[e] && IM_g1g2 != 0) IMgg_Ethresh_h[e][1].Fill(IM_g1g2);

				if(phot1_E > Eg_thresh[e] && phot2_E > Eg_thresh[e] && IM_g1g2 != 0)
				{
					IMgg_EthreshComb_h[e].Fill(IM_g1g2);
				}
			}

			for (u_int e = 0; e < egcone_bins.size(); e++)
			{
				if(eg1coneangle > egcone_bins[e][0] && eg1coneangle < egcone_bins[e][1] && IM_g1g2 != 0) IMgg_egconebins_h[e][0].Fill(IM_g1g2);
				if(eg2coneangle > egcone_bins[e][0] && eg2coneangle < egcone_bins[e][1] && IM_g1g2 != 0) IMgg_egconebins_h[e][1].Fill(IM_g1g2);

				if((eg1coneangle > egcone_bins[e][0] && eg1coneangle < egcone_bins[e][1]) && (eg2coneangle > egcone_bins[e][0] && eg2coneangle < egcone_bins[e][1]) && IM_g1g2 != 0)
				{
					IMgg_egconebinsComb_h[e].Fill(IM_g1g2);
				}
			}

			for (u_int e = 0; e < egcone_thresh.size(); e++)
			{
				if(eg1coneangle > egcone_thresh[e] && IM_g1g2 != 0) IMgg_egconethresh_h[e][0].Fill(IM_g1g2);
				if(eg2coneangle > egcone_thresh[e] && IM_g1g2 != 0) IMgg_egconethresh_h[e][1].Fill(IM_g1g2);

				if(eg1coneangle > egcone_thresh[e] && eg2coneangle > egcone_thresh[e] && IM_g1g2 != 0)
				{
					IMgg_egconethreshComb_h[e].Fill(IM_g1g2);
				}
			}

			Eg_egcone_corr_h.Fill(eg1coneangle, phot1_E);
		} //if 2g-event
		

	}//entry loop

	TFile* OutFile = new TFile(outfile, "RECREATE");

	writeHistos(IMgg_Ebins_h);
	writeHistos(IMgg_EbinsComb_h);
	writeHistos(IMgg_Ethresh_h);
	writeHistos(IMgg_EthreshComb_h);
	writeHistos(IMgg_egconebins_h);
	writeHistos(IMgg_egconebinsComb_h);
	writeHistos(IMgg_egconethresh_h);
	writeHistos(IMgg_egconethreshComb_h);
	Eg_egcone_corr_h.Write();

	OutFile->Close();
}

//Generic function to create 2D array of TH1Fs.
TH1Array2D createTH1Array2D(TString hname, float op[3], TString htitle, vector<TString> var1, vector<TString> var1_titles, vector<TString> var2, vector<TString> var2_titles){
	TH1Array2D h2D;

	for(u_int i = 0; i < var1.size(); i++)
	{
		TH1Array1D h1D;
		for(u_int j=0; j < var2.size(); j++)
		{
			auto histName = Form("%s_%s_%s", hname.Data(), var2[j].Data(), var1[i].Data());
			auto histTitle = Form("%s - %s: %s", htitle.Data(), var2[j].Data() , var1[i].Data());

			h1D.push_back(TH1F{histName,histTitle,(int)op[0],op[1],op[2]});
		}
		h2D.push_back(h1D);
	}

	return h2D;
}

//Generic function to create 1D array of TH1Fs.
TH1Array1D createTH1Array1D(TString hname, float op[3], TString htitle, vector<TString> var, vector<TString> var_titles){
	TH1Array1D h1D;

	for(u_int j=0; j < var.size(); j++)
	{
		auto histName = Form("%s_%s", hname.Data(), var[j].Data());
		auto histTitle = Form("%s: %s", htitle.Data(), var[j].Data());

		h1D.push_back(TH1F{histName,histTitle,(int)op[0],op[1],op[2]});
	}

	return h1D;
}

void writeHistos(TH1Array1D &hist){
	for (auto &ihist : hist)
	{
		ihist.Write();
	}
}

void writeHistos(TH1Array2D &hist){
	for (auto &histarray : hist)
	{
		for (auto &ihist : histarray)
		{
			ihist.Write();
		}
	}
}

