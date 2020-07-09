// @(#)root
/* Script to plot general dipcheck plots and interesting physics from processed file. */
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include <TFile.h>
#include <TChain.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TText.h>
#include <TPaveStats.h>
#include <TString.h>

void plots(TString infile, TString outfile)
{
    TChain chain("data");
    chain.AddFile(infile);

    double Q2;
    double xB;
    double IM_g1g2;
    double MM2_total;
    double ME_total;
    double MM2_rec_recoil;

    bool flag_cuts_3sigIM;
    bool flag_cuts_dis;
    bool flag_cuts_W2;
    bool flag_cuts_broadcone;
    bool flag_cuts_spectMP;
    bool flag_cuts_3sigMM2;
    bool flag_cuts_cop;
    bool flag_cuts_broadMM2;

    chain.SetBranchAddress("Q2", &Q2);
    chain.SetBranchAddress("xB", &xB);
    chain.SetBranchAddress("IM_g1g2", &IM_g1g2);
    chain.SetBranchAddress("MM2_total", &MM2_total);
    chain.SetBranchAddress("ME_total", &ME_total);
    chain.SetBranchAddress("MM2_rec_recoil", &MM2_rec_recoil);

    chain.SetBranchAddress("flag_cuts_3sigIM", &flag_cuts_3sigIM);
    chain.SetBranchAddress("flag_cuts_dis", &flag_cuts_dis);
    chain.SetBranchAddress("flag_cuts_W2", &flag_cuts_W2);
    chain.SetBranchAddress("flag_cuts_broadcone", &flag_cuts_broadcone);
    chain.SetBranchAddress("flag_cuts_spectMP", &flag_cuts_spectMP);
    chain.SetBranchAddress("flag_cuts_3sigMM2", &flag_cuts_3sigMM2);
    chain.SetBranchAddress("flag_cuts_cop", &flag_cuts_cop);
    chain.SetBranchAddress("flag_cuts_broadMM2", &flag_cuts_broadMM2);

    TH2F *Q2xB_h[5];
    TH1F *ggIM_h[5];
    TH1F *MM2tot_h[5];
    TH1F *MEtot_h[5];
    TH1F *MM2eggX_h[5];

    Q2xB_h[0] = new TH2F("Q2xB_raw_h", "Q^{2} vs. x_{B}- raw;           x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2xB_h[1] = new TH2F("Q2xB_broad_h", "Q^{2} vs. x_{B}- broad;         x_{B}; Q^{2}", 200, 0, 1.2, 200, 0, 12);
    Q2xB_h[2] = new TH2F("Q2xB_3sig_h", "Q^{2} vs. x_{B}- 3sig;          x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2xB_h[3] = new TH2F("Q2xB_3sigExc_h", "Q^{2} vs. x_{B}- 3sig+Excl;     x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2xB_h[4] = new TH2F("Q2xB_3sigExcDis_h", "Q^{2} vs. x_{B}- 3sig+Excl+DIS; x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);

    ggIM_h[0] = new TH1F("ggIM_raw_h", "Invariant mass[#gamma_{1} #gamma_{2}] - raw;           Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_h[1] = new TH1F("ggIM_broad_h", "Invariant mass[#gamma_{1} #gamma_{2}] - broad;         Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_h[2] = new TH1F("ggIM_3sig_h", "Invariant mass[#gamma_{1} #gamma_{2}] - 3sig;          Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_h[3] = new TH1F("ggIM_3sigExc_h", "Invariant mass[#gamma_{1} #gamma_{2}] - 3sig+Excl;     Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
    ggIM_h[4] = new TH1F("ggIM_3sigExcDis_h", "Invariant mass[#gamma_{1} #gamma_{2}] - 3sig+Excl+DIS; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

    MM2tot_h[0] = new TH1F("MM2tot_raw_h", "MM^{2}_{ep->e'p'#gamma#gammaX} - raw;           Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);
    MM2tot_h[1] = new TH1F("MM2tot_broad_h", "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig+Excl+DIS; Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);
    MM2tot_h[2] = new TH1F("MM2tot_3sig_h", "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig;          Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);
    MM2tot_h[3] = new TH1F("MM2tot_3sigExc_h", "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig+Excl;     Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);
    MM2tot_h[4] = new TH1F("MM2tot_3sigExcDis_h", "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig+Excl+DIS; Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);

    MEtot_h[0] = new TH1F("MEtot_raw_h", "Missing Energy [ep->e'p'#gamma#gammaX] - raw;           Mis.Energy (GeV); counts", 300, -5, 10);
    MEtot_h[1] = new TH1F("MEtot_broad_h", "Missing Energy [ep->e'p'#gamma#gammaX] - broad;         Mis.Energy (GeV); counts", 300, -5, 10);
    MEtot_h[2] = new TH1F("MEtot_3sig_h", "Missing Energy [ep->e'p'#gamma#gammaX] - 3sig;          Mis.Energy (GeV); counts", 300, -5, 10);
    MEtot_h[3] = new TH1F("MEtot_3sigExc_h", "Missing Energy [ep->e'p'#gamma#gammaX] - 3sig+Excl;     Mis.Energy (GeV); counts", 300, -5, 10);
    MEtot_h[4] = new TH1F("MEtot_3sigExcDis_h", "Missing Energy [ep->e'p'#gamma#gammaX] - 3sig+Excl+DIS; Mis.Energy (GeV); counts", 300, -5, 10);

    MM2eggX_h[0] = new TH1F("MM2eggX_raw_h", "MM^{2}_{e#gamma_{1}#gamma_{2}X} - raw;             MM^{2}_{e#gamma_{1}#gamma_{2}X}(GeV^{2}/c^{4}); counts", 200, -5, 5);
    MM2eggX_h[1] = new TH1F("MM2eggX_broad_h", "MM^{2}_{e#gamma_{1}#gamma_{2}X} - broad;           MM^{2}_{e#gamma_{1}#gamma_{2}X}(GeV^{2}/c^{4}); counts", 200, -5, 5);
    MM2eggX_h[2] = new TH1F("MM2eggX_3sig_h", "MM^{2}_{e#gamma_{1}#gamma_{2}X} - 3sig;            MM^{2}_{e#gamma_{1}#gamma_{2}X}(GeV^{2}/c^{4}); counts", 200, -5, 5);
    MM2eggX_h[3] = new TH1F("MM2eggX_3sigExc_h", "MM^{2}_{e#gamma_{1}#gamma_{2}X} - 3sig+Excl;       MM^{2}_{e#gamma_{1}#gamma_{2}X}(GeV^{2}/c^{4}); counts", 200, -5, 5);
    MM2eggX_h[4] = new TH1F("MM2eggX_3sigExcDis_h", "MM^{2}_{e#gamma_{1}#gamma_{2}X} - 3sig+Excl+DIS;   MM^{2}_{e#gamma_{1}#gamma_{2}X}(GeV^{2}/c^{4}); counts", 200, -5, 5);

    for (Int_t i = 0; i < chain.GetEntries(); i++)
    {
        chain.GetEntry(i);

        //Raw selected events
        Q2xB_h[0]->Fill(xB, Q2);
        ggIM_h[0]->Fill(IM_g1g2);
        MM2tot_h[0]->Fill(MM2_total);
        MEtot_h[0]->Fill(ME_total);
        MM2eggX_h[0]->Fill(MM2_rec_recoil);
        //"Broad" MM2epggX and cone-angle cuts to clear up some BG
        if (flag_cuts_broadMM2 == 1 && flag_cuts_broadcone == 1)
        {
            Q2xB_h[1]->Fill(xB, Q2);
            ggIM_h[1]->Fill(IM_g1g2);
            MM2tot_h[1]->Fill(MM2_total);
            MEtot_h[1]->Fill(ME_total);
            MM2eggX_h[1]->Fill(MM2_rec_recoil);
        }

        //3-sigma cut around reconstructed pi0-mass
        if (flag_cuts_3sigIM == 1)
        {
            Q2xB_h[2]->Fill(xB, Q2);
            ggIM_h[2]->Fill(IM_g1g2);
            MM2tot_h[2]->Fill(MM2_total);
            MEtot_h[2]->Fill(ME_total);
            MM2eggX_h[2]->Fill(MM2_rec_recoil);

            //Add Exclusivity cuts
            if (flag_cuts_broadcone == 1 && flag_cuts_spectMP == 1 && flag_cuts_3sigMM2 == 1 && flag_cuts_cop == 1)
            {
                Q2xB_h[3]->Fill(xB, Q2);
                ggIM_h[3]->Fill(IM_g1g2);
                MM2tot_h[3]->Fill(MM2_total);
                MEtot_h[3]->Fill(ME_total);
                MM2eggX_h[3]->Fill(MM2_rec_recoil);

                //Add DIS cuts
                if (flag_cuts_dis == 1)
                {
                    Q2xB_h[4]->Fill(xB, Q2);
                    ggIM_h[4]->Fill(IM_g1g2);
                    MM2tot_h[4]->Fill(MM2_total);
                    MEtot_h[4]->Fill(ME_total);
                    MM2eggX_h[4]->Fill(MM2_rec_recoil);
                }
            }
        }
    }

    TFile *Outfile = new TFile(outfile, "recreate");

    //Loop individually for well formatted output files
    for (int i = 0; i < 5; i++)
    {
        Q2xB_h[i]->Write();
    }

    for (int i = 0; i < 5; i++)
    {
        ggIM_h[i]->Write();
    }

    for (int i = 0; i < 5; i++)
    {
        MM2tot_h[i]->Write();
    }

    for (int i = 0; i < 5; i++)
    {
        MEtot_h[i]->Write();
    }
    
    for (int i = 0; i < 5; i++)
    {
        MM2eggX_h[i]->Write();
    }

    Outfile->Close();
}