#include "hist_fxns.h"

//hist_fxns::hist_fxns() { }

//hist_fxns::~hist_fxns() { }


//Generic function to create 2D array of TH1Fs.
TH1Array2D hist_fxns::createTH1Array2D(TString hname, float op[3], TString htitle, vector<TString> var1, vector<TString> var1_titles, vector<TString> var2, vector<TString> var2_titles){
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

// HistArray2D_Ptr hist_fxns::createHistPtrArray2D(TString hname, float op[3], TString htitle, vector<TString> var1, vector<TString> var1_titles, vector<TString> var2, vector<TString> var2_titles){
// 	HistArray2D h2D;

// 	for(u_int i = 0; i < var1.size(); i++)
// 	{
// 		HistArray1D h1D;
// 		for(u_int j=0; j < var2.size(); j++)
// 		{
// 			auto histName = Form("%s_%s_%s", hname.Data(), var2[j].Data(), var1[i].Data());
// 			auto histTitle = Form("%s - %s: %s", htitle.Data(), var2[j].Data() , var1[i].Data());

// 			TH1F *h = new TH1F(histName,histTitle,(int)op[0],op[1],op[2])
// 			h1D.push_back(h);
// 		}
// 		h2D.push_back(h1D);
// 	}

// 	return h2D;
// }

//Generic function to create 1D array of TH1Fs.
TH1Array1D hist_fxns::createTH1Array1D(TString hname, float op[3], TString htitle, vector<TString> var, vector<TString> var_titles){
	TH1Array1D h1D;

	for(u_int j=0; j < var.size(); j++)
	{
		auto histName = Form("%s_%s", hname.Data(), var[j].Data());
		auto histTitle = Form("%s: %s", htitle.Data(), var[j].Data());

		h1D.push_back(TH1F{histName,histTitle,(int)op[0],op[1],op[2]});
	}

	return h1D;
}

void hist_fxns::writeHistos(TH1Array1D &hist){
	for (auto &ihist : hist)
	{
		ihist.Write();
	}
}

void hist_fxns::writeHistos(TH1Array2D &hist){
	for (auto &histarray : hist)
	{
		for (auto &ihist : histarray)
		{
			ihist.Write();
		}
	}
}