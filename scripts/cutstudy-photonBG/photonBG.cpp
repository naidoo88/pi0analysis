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
using HistArray1D = vector<TH1F> ;
using HistArray2D = vector<HistArray1D>;

HistArray2D create2DHistos(TString, float[3], TString, vector<TString>, vector<TString>, vector<TString>, vector<TString>);

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
    double g1E;
    double g2E;

    chain.SetBranchAddress("Q2", &Q2);
    chain.SetBranchAddress("xB", &xB);
    chain.SetBranchAddress("IM_g1g2", &IM_g1g2);
    chain.SetBranchAddress("MM2_total", &MM2_total);
    chain.SetBranchAddress("ME_total", &ME_total);
    chain.SetBranchAddress("MM2_rec_recoil", &MM2_rec_recoil);
    chain.SetBranchAddress("MP_rec_spectator", &MP_rec_spectator);
    chain.SetBranchAddress("pi0coneangle", &pi0coneangle);
    chain.SetBranchAddress("g1E", &g1E);
    chain.SetBranchAddress("g2E", &g2E);
    
    vector<vector<double>> Eg_bins = {{0.0, 0.5, 1.0, 1.5, 2.5}, //lower lim
                                    {0.5, 1.0, 1.5, 2.5, 100}}; //upper lim


    vector<TString> Ebins{"E0-0.5","E0.5-1","E1-1.5", "E1.5-2.5", "Eover2.5"};
    vector<TString> Etitles{"E_{#gamma} 0-0.5GeV","E_{#gamma} 0.5-1GeV","E_{#gamma} 1-1.5GeV","E_{#gamma} 1.5-2.5GeV","E_{#gamma} > 2.5GeV"};
    vector<TString> Photon{"g1","g2"};
    vector<TString> Photontitles{"#gamma_{1}","#gamma_{2}"};
    float options[3] = {200,0,0.2};
    auto IMgg_Ebins_h = create2DHistos("IM_gg", options,"IM_{#gamma#gamma}", Ebins, Etitles, Photon, Photontitles);

    for (Int_t i = 0; i < chain.GetEntries(); i++)
    {
        chain.GetEntry(i);

        for (u_int e; e < Eg_bins.size(); e++){
            if(g1E > Eg_bins[e][0] && g1E < Eg_bins[e][1]) IMgg_Ebins_h[e][0].Fill(IM_g1g2);
            if(g2E > Eg_bins[e][0] && g2E < Eg_bins[e][1]) IMgg_Ebins_h[e][1].Fill(IM_g1g2);
        }
    }


    TFile* OutFile = new TFile(outfile, "RECREATE");

    for (u_int i; i < IMgg_Ebins_h.size(); i++){
        IMgg_Ebins_h[i][0].Write();
        IMgg_Ebins_h[i][1].Write();
    }
    
    OutFile->Close();
}

//Generic function to create 2D array of TH1Fs.
HistArray2D create2DHistos(TString hname, float op[3], TString htitle, vector<TString> var1, vector<TString> var1_titles, vector<TString> var2, vector<TString> var2_titles){
    auto n_v1 = var1.size();
    auto n_v2 = var2.size();

    HistArray2D h2D;

    for(u_int i=0; i<n_v1; i++){
        HistArray1D h1D;
        for(u_int j=0; j<n_v2; j++){
            auto histName = Form("%s_%s_%s", hname.Data(), var2[j].Data(), var1[i].Data());
            auto histTitle = Form("%s - %s: %s", htitle.Data(), var2[j].Data() , var1[i].Data());

            h1D.push_back(TH1F{histName,histTitle,(int)op[0],op[1],op[2]});
        }
        h2D.push_back(h1D);
    }

    return h2D;
}

