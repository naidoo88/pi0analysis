// @(#)root
#include <cstdlib>
#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <TString.h>

#include "clas12reader.h"
#include "cutstudyhistosFTFD.h"

using namespace clas12;

void cutstudyhistosFTFD(TString datafile, TString outfile)
{
    TChain chain("data");
    chain.AddFile(datafile);

    double IM_g1g2;
    double MM2_total;
    double pi0coneangle;
    bool flag_cuts_dis;
    bool flag_photon1_FD;
    bool flag_photon2_FD;
    bool flag_photon1_FT;
    bool flag_photon2_FT;

    chain.SetBranchAddress("IM_g1g2",         &IM_g1g2);
    chain.SetBranchAddress("MM2_total",       &MM2_total);
    chain.SetBranchAddress("pi0coneangle",    &pi0coneangle);
    chain.SetBranchAddress("flag_cuts_dis",   &flag_cuts_dis);
    chain.SetBranchAddress("flag_photon1_FD", &flag_photon1_FD);
    chain.SetBranchAddress("flag_photon2_FD", &flag_photon2_FD);
    chain.SetBranchAddress("flag_photon1_FT", &flag_photon1_FT);
    chain.SetBranchAddress("flag_photon2_FT", &flag_photon2_FT);

    cuthistos();

    std::cout << "Filling Histograms..." << '\n';
    for (Int_t i = 0; i < chain.GetEntries(); i++)
    {
        chain.GetEntry(i);

        /*=====PRE-CUT HISTOS=====*/
        if (flag_photon1_FD == 1 && flag_photon2_FD == 1) ggIM_h[0]->Fill(IM_g1g2);
        if (flag_photon1_FT == 1 && flag_photon2_FT == 1) ggIM_h[1]->Fill(IM_g1g2);
        if ((flag_photon1_FD == 1 && flag_photon2_FT == 1) || (flag_photon2_FD == 1 && flag_photon1_FT == 1)) ggIM_h[2]->Fill(IM_g1g2);

        // /*=====Cone angle histos=====*/
        if ((MM2_total>-0.5 && MM2_total<0.5) && (flag_cuts_dis == 1)){ //Apply very broad MM2 cut to kill some background.
            if (pi0coneangle < 30)
            {
                if (flag_photon1_FD == 1 && flag_photon2_FD == 1) ggIM_coneangle_h[0][0]  -> Fill(IM_g1g2);
                if (flag_photon1_FT == 1 && flag_photon2_FT == 1) ggIM_coneangle_h[0][1]  -> Fill(IM_g1g2);
                if ((flag_photon1_FD == 1 && flag_photon2_FT == 1) || (flag_photon2_FD == 1 && flag_photon1_FT == 1)) ggIM_coneangle_h[0][2]  -> Fill(IM_g1g2);
            }    
            if (pi0coneangle < 20)
            {
                if (flag_photon1_FD == 1 && flag_photon2_FD == 1) ggIM_coneangle_h[1][0]  -> Fill(IM_g1g2);
                if (flag_photon1_FT == 1 && flag_photon2_FT == 1) ggIM_coneangle_h[1][1]  -> Fill(IM_g1g2);
                if ((flag_photon1_FD == 1 && flag_photon2_FT == 1) || (flag_photon2_FD == 1 && flag_photon1_FT == 1)) ggIM_coneangle_h[1][2]  -> Fill(IM_g1g2);
            }
            if (pi0coneangle < 10)
            {
                if (flag_photon1_FD == 1 && flag_photon2_FD == 1) ggIM_coneangle_h[2][0]  -> Fill(IM_g1g2);
                if (flag_photon1_FT == 1 && flag_photon2_FT == 1) ggIM_coneangle_h[2][1]  -> Fill(IM_g1g2);
                if ((flag_photon1_FD == 1 && flag_photon2_FT == 1) || (flag_photon2_FD == 1 && flag_photon1_FT == 1)) ggIM_coneangle_h[2][2]  -> Fill(IM_g1g2);    
            }
            if (pi0coneangle < 8)
            {
                if (flag_photon1_FD == 1 && flag_photon2_FD == 1) ggIM_coneangle_h[3][0]  -> Fill(IM_g1g2);
                if (flag_photon1_FT == 1 && flag_photon2_FT == 1) ggIM_coneangle_h[3][1]  -> Fill(IM_g1g2);
                if ((flag_photon1_FD == 1 && flag_photon2_FT == 1) || (flag_photon2_FD == 1 && flag_photon1_FT == 1)) ggIM_coneangle_h[3][2]  -> Fill(IM_g1g2);    
            }
        }//broad MM2 cut (+/- 0.5)
    } //chain-loop

    /*=====Plot some canvases=====*/
    
    std::cout << "Drawing plots..." << endl;
    TCanvas *ggIM_conecuts_c = new TCanvas("ggIM_conecuts_c", "#gamma_{1}#gamma_{2}-mass split by region (Cone angle < 30',20',10', 8')");
    ggIM_conecuts_c->Divide(4,3); 
    int n = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << "i: " << i << "  j: " << j << endl; 
            ggIM_conecuts_c->cd(n+1); 
            ggIM_coneangle_h[j][i]->Draw();
            n++;
        }
    }


/*=====Write out to file=====*/

    std::cout << "Writting out to file..." << '\n';
    TFile *Out_File = new TFile(outfile, "recreate");

    for (int i = 0; i < 3; i++)
    {
        ggIM_h[i]->Write();
                for (int j = 0; j < 4; j++)
        {
            cout << "i: " << i << "  j: " << j << endl;  
            ggIM_coneangle_h[j][i]->Write();
        }
    }

    Out_File->Close();

} //macro

void cuthistos()
{
    std::cout << "Generating histograms..." << '\n';

    /* For reg-splits:
    // [*][0] - both photons in FD
    // [*][1] - both photons in FT
    // [*][2] - photon in FD/FT
    */

    //Raw g1g2-inv.mass split by region.
    //---------------------------------------------------------------------------------------------------------------
    ggIM_h[0] = new TH1F("ggIM_bothFD_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FD; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_h[1] = new TH1F("ggIM_bothFT_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FT; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_h[2] = new TH1F("ggIM_FDFT_h", "Invariant mass[#gamma_{1} #gamma_{2}] - FD/FT;   Inv.Mass (GeV/c^{2}); counts",   200, 0, 0.2);

    //Cone angle cuts - g1g2-inv.mass split by region.
    //---------------------------------------------------------------------------------------------------------------
    ggIM_coneangle_h[0][0] = new TH1F("ggIM_bothFD_coneangleL30_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FD - ConeAngle<30^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_coneangle_h[0][1] = new TH1F("ggIM_bothFT_coneangleL30_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FT - ConeAngle<30^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_coneangle_h[0][2] = new TH1F("ggIM_FDFT_coneangleL30_h", "Invariant mass[#gamma_{1} #gamma_{2}] - FD/FT - ConeAngle<30^{o}; Inv.Mass (GeV/c^{2}); counts",     200, 0, 0.2);

    ggIM_coneangle_h[1][0] = new TH1F("ggIM_bothFD_coneangleL20_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FD - ConeAngle<20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_coneangle_h[1][1] = new TH1F("ggIM_bothFT_coneangleL20_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FT - ConeAngle<20^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_coneangle_h[1][2] = new TH1F("ggIM_FDFT_coneangleL20_h", "Invariant mass[#gamma_{1} #gamma_{2}] - FD/FT - ConeAngle<20^{o}; Inv.Mass (GeV/c^{2}); counts",     200, 0, 0.2);

    ggIM_coneangle_h[2][0] = new TH1F("ggIM_bothFD_coneangleL10_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FD - ConeAngle<10^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_coneangle_h[2][1] = new TH1F("ggIM_bothFT_coneangleL10_h", "Invariant mass[#gamma_{1} #gamma_{2}] - both FT - ConeAngle<10^{o}; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_coneangle_h[2][2] = new TH1F("ggIM_FDFT_coneangleL10_h", "Invariant mass[#gamma_{1} #gamma_{2}] - FD/FT - ConeAngle<10^{o}; Inv.Mass (GeV/c^{2}); counts",     200, 0, 0.2);

    ggIM_coneangle_h[3][0] = new TH1F("ggIM_bothFD_coneangleL8_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - both FD - ConeAngle<8^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_coneangle_h[3][1] = new TH1F("ggIM_bothFT_coneangleL8_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - both FT - ConeAngle<8^{o};  Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_coneangle_h[3][2] = new TH1F("ggIM_FDFT_coneangleL8_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - FD/FT - ConeAngle<8^{o};  Inv.Mass (GeV/c^{2}); counts",     200, 0, 0.2);

} //histos fxn
