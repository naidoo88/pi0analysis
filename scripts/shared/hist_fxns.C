#include "hist_fxns.h"

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

//Generic function to create 1D array of TH1F pointers.
TH1Array1D_Ptr hist_fxns::createTH1PtrArray1D(TString hname, float op[3], TString htitle, vector<TString> var, vector<TString> var_titles){
	TH1Array1D_Ptr h1D;

	for(u_int j=0; j < var.size(); j++)
	{
		auto histName = Form("%s_%s", hname.Data(), var[j].Data());
		auto histTitle = Form("%s: %s", htitle.Data(), var[j].Data());

		h1D.push_back(new TH1F{histName,histTitle,(int)op[0],op[1],op[2]});
	}

	return h1D;
}

TH1Array2D_Ptr hist_fxns::createTH1PtrArray2D(TString hname, float op[3], TString htitle, vector<TString> var1, vector<TString> var1_titles, vector<TString> var2, vector<TString> var2_titles){
	TH1Array2D_Ptr h2D;

	for(u_int i = 0; i < var1.size(); i++)
	{
		TH1Array1D_Ptr h1D;
		for(u_int j=0; j < var2.size(); j++)
		{
			auto histName = Form("%s_%s_%s", hname.Data(), var2[j].Data(), var1[i].Data());
			auto histTitle = Form("%s - %s: %s", htitle.Data(), var2[j].Data() , var1[i].Data());

			h1D.push_back(new TH1F{histName,histTitle,(int)op[0],op[1],op[2]});
		}
		h2D.push_back(h1D);
	}

	return h2D;
}

TH2Array2D hist_fxns::createTH2Array2D(TString hname, float op[6], TString htitle, vector<TString> var1, vector<TString> var1_titles, vector<TString> var2, vector<TString> var2_titles){
	TH2Array2D h2D;

	for(u_int i = 0; i < var1.size(); i++)
	{
		TH2Array1D h1D;
		for(u_int j=0; j < var2.size(); j++)
		{
			auto histName = Form("%s_%s_%s", hname.Data(), var2[j].Data(), var1[i].Data());
			auto histTitle = Form("%s - %s: %s", htitle.Data(), var2[j].Data() , var1[i].Data());

			h1D.push_back(TH2F{histName,histTitle,(int)op[0],op[1],op[2],(int)op[3],op[4],op[5]});
		}
		h2D.push_back(h1D);
	}

	return h2D;
}


//Generic function to create 1D array of TH2Fs.
TH2Array1D hist_fxns::createTH2Array1D(TString hname, float op[6], TString htitle, vector<TString> var, vector<TString> var_titles){
	TH2Array1D h1D;

	for(u_int j=0; j < var.size(); j++)
	{
		auto histName = Form("%s_%s", hname.Data(), var[j].Data());
		auto histTitle = Form("%s: %s", htitle.Data(), var[j].Data());

		h1D.push_back(TH2F{histName,histTitle,(int)op[0],op[1],op[2],(int)op[3],op[4],op[5]});
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

void hist_fxns::writeHistos(TH1Array1D_Ptr hist){
	for (auto ihist : hist)
	{
		ihist->Write();
	}
}

void hist_fxns::writeHistos(TH1Array2D_Ptr hist){
	for (auto histarray : hist)
	{
		for (auto ihist : histarray)
		{
			ihist->Write();
		}
	}
}