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
using HistArray1D = vector<TH1F>;
using HistArray2D = vector<HistArray1D>;

HistArray1D createHistArray1D(TString, float[3], TString, vector<TString>, vector<TString>);
HistArray2D createHistArray2D(TString, float[3], TString, vector<TString>, vector<TString>, vector<TString>, vector<TString>);
void writeHistos(HistArray1D &);
void writeHistos(HistArray2D &);

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
	chain.SetBranchAddress("flag_2photon_event", &flag_2photon_event);
	
	vector<vector<double>> Eg_bins = {{0.0, 0.5}, {0.5, 1.0}, {1.0, 1.5}, 
									  {1.5, 2.5}, {2.5, 100}};

	vector<TString> Ebins{"E0-0.5","E0.5-1","E1-1.5", "E1.5-2.5", "Eover2.5"};
	vector<TString> Etitles{"E_{#gamma} 0-0.5GeV","E_{#gamma} 0.5-1GeV","E_{#gamma} 1-1.5GeV","E_{#gamma} 1.5-2.5GeV","E_{#gamma} > 2.5GeV"};
	vector<TString> Photon{"g1","g2"};
	vector<TString> Photontitles{"#gamma_{1}","#gamma_{2}"};
	float options[3] = {200,0.001,0.2};
	auto IMgg_Ebins_h = createHistArray2D("IM_gg", options,"IM_{#gamma#gamma}", Ebins, Etitles, Photon, Photontitles);
	auto IMgg_EbinsComb_h = createHistArray1D("IM_gg", options,"IM_{#gamma#gamma}", Ebins, Etitles);

	vector<double> Eg_thresh = {0, 0.5, 1, 1.5, 2, 2.5};
	vector<TString> Ethresh{"E>0","E>0.5","E>1","E>1.5","E>2", "E>2.5"};
	vector<TString> Ethresh_titles{"E_{#gamma}>0","E_{#gamma}>0.5","E_{#gamma}>1","E_{#gamma}>1.5","E_{#gamma}>2", "E_{#gamma}>2.5"};
	auto IMgg_Ethresh_h = createHistArray2D("IM_gg", options, "IM_{#gamma#gamma}", Ethresh, Ethresh_titles, Photon, Photontitles);
	auto IMgg_EthreshComb_h = createHistArray1D("IM_gg", options,"IM_{#gamma#gamma}", Ethresh, Ethresh_titles);

	//vector<vector<double>> egcone_bins = {{0.0, 0.5}, {0.5, 1.0}, {1.0, 1.5}, {1.5, 2.5}, {2.5, 100}};
	
	//vector<double>egcone_thresh = 


	for (u_int i = 0; i < chain.GetEntries(); i++)
	{
		chain.GetEntry(i);
		
		//cout << "Photon1 has energy: " << phot1_E << endl;
		//cout << "Photon2 has energy: " << phot2_E << endl << endl;
		//cout << "E-bin size: " << Eg_bins.size() << endl;

		if(flag_2photon_event == 1)
		{
			for (u_int e = 0; e < Eg_bins.size(); e++)
			{
				//cout << "Current bin is: " << Eg_bins[e][0] << " to " << Eg_bins[e][1] << endl;
				if(phot1_E > Eg_bins[e][0] && phot1_E < Eg_bins[e][1] && IM_g1g2 != 0) IMgg_Ebins_h[e][0].Fill(IM_g1g2);
				if(phot2_E > Eg_bins[e][0] && phot2_E < Eg_bins[e][1] && IM_g1g2 != 0) IMgg_Ebins_h[e][1].Fill(IM_g1g2);

				if((phot1_E > Eg_bins[e][0] && phot1_E < Eg_bins[e][1]) && (phot2_E > Eg_bins[e][0] && phot2_E < Eg_bins[e][1]) && IM_g1g2 != 0)
				{
					IMgg_EbinsComb_h[e].Fill(IM_g1g2);
				}
			}

			for (u_int e = 0; e < Eg_thresh.size(); e++)
			{
				//cout << "Current bin is: " << Eg_thresh[e][0] << " to " << Eg_thresh[e][1] << endl;
				if(phot1_E > Eg_thresh[e] && IM_g1g2 != 0) IMgg_Ethresh_h[e][0].Fill(IM_g1g2);
				if(phot2_E > Eg_thresh[e] && IM_g1g2 != 0) IMgg_Ethresh_h[e][1].Fill(IM_g1g2);

				if(phot1_E > Eg_thresh[e] && phot2_E > Eg_thresh[e] && IM_g1g2 != 0)
				{
					IMgg_EthreshComb_h[e].Fill(IM_g1g2);
				}
			}

		} //if 2g-event
		

	}//entry loop

	TFile* OutFile = new TFile(outfile, "RECREATE");

	writeHistos(IMgg_Ebins_h);
	writeHistos(IMgg_EbinsComb_h);
	writeHistos(IMgg_Ethresh_h);
	writeHistos(IMgg_EthreshComb_h);

	// for (u_int i = 0; i < IMgg_Ebins_h.size(); i++)
	// {
	// 	IMgg_Ebins_h[i][0].Write();
	// }
	// for (u_int i = 0; i < IMgg_Ebins_h.size(); i++)
	// {
	// 	IMgg_Ebins_h[i][1].Write();
	// }
	// for (u_int i = 0; i < IMgg_Ebins_h.size(); i++)
	// {
	// 	IMgg_EbinsComb_h[i].Write();
	// }

	// for (u_int i = 0; i < IMgg_Ethresh_h.size(); i++)
	// {
	// 	IMgg_Ethresh_h[i][0].Write();
	// }
	// for (u_int i = 0; i < IMgg_Ethresh_h.size(); i++)
	// {
	// 	IMgg_Ethresh_h[i][1].Write();
	// }
	// for (u_int i = 0; i < IMgg_Ethresh_h.size(); i++)
	// {
	// 	IMgg_EthreshComb_h[i].Write();
	// }

	OutFile->Close();
}

//Generic function to create 2D array of TH1Fs.
HistArray2D createHistArray2D(TString hname, float op[3], TString htitle, vector<TString> var1, vector<TString> var1_titles, vector<TString> var2, vector<TString> var2_titles){
	HistArray2D h2D;

	for(u_int i = 0; i < var1.size(); i++)
	{
		HistArray1D h1D;
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
HistArray1D createHistArray1D(TString hname, float op[3], TString htitle, vector<TString> var, vector<TString> var_titles){
	HistArray1D h1D;

	for(u_int j=0; j < var.size(); j++)
	{
		auto histName = Form("%s_%s", hname.Data(), var[j].Data());
		auto histTitle = Form("%s: %s", htitle.Data(), var[j].Data());

		h1D.push_back(TH1F{histName,histTitle,(int)op[0],op[1],op[2]});
	}

	return h1D;
}

void writeHistos(HistArray1D &hist){
	for (auto &ihist : hist)
	{
		ihist.Write();
	}
}

void writeHistos(HistArray2D &hist){
	for (auto &histarray : hist)
	{
		for (auto &ihist : histarray)
		{
			ihist.Write();
		}
	}
}

