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
    double pi0coneangle;

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
    chain.SetBranchAddress("pi0coneangle", &pi0coneangle);

    chain.SetBranchAddress("flag_cuts_3sigIM", &flag_cuts_3sigIM);
    chain.SetBranchAddress("flag_cuts_dis", &flag_cuts_dis);
    chain.SetBranchAddress("flag_cuts_W2", &flag_cuts_W2);
    chain.SetBranchAddress("flag_cuts_broadcone", &flag_cuts_broadcone);
    chain.SetBranchAddress("flag_cuts_spectMP", &flag_cuts_spectMP);
    chain.SetBranchAddress("flag_cuts_3sigMM2", &flag_cuts_3sigMM2);
    chain.SetBranchAddress("flag_cuts_cop", &flag_cuts_cop);
    chain.SetBranchAddress("flag_cuts_broadMM2", &flag_cuts_broadMM2);

    TH2F *Q2_v_xB_h[5];
    TH2F *IMgg_v_MM2tot_h[5];
    TH2F *MEtot_v_MM2eggX_h[5];
    TH2F *IMgg_v_pi0cone_h[5];
    TH1F *ggIM_h[5];
    TH1F *MM2tot_h[5];
    TH1F *MEtot_h[5];
    TH1F *MM2eggX_h[5];
    TH1F *pi0cone_h[5];

    Q2_v_xB_h[0] = new TH2F("Q2_v_xB_raw_h",        "Q^{2} vs. x_{B}- raw;           x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2_v_xB_h[1] = new TH2F("Q2_v_xB_broad_h",      "Q^{2} vs. x_{B}- broad;         x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2_v_xB_h[2] = new TH2F("Q2_v_xB_3sig_h",       "Q^{2} vs. x_{B}- 3sig;          x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2_v_xB_h[3] = new TH2F("Q2_v_xB_3sigExc_h",    "Q^{2} vs. x_{B}- 3sig+Excl;     x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2_v_xB_h[4] = new TH2F("Q2_v_xB_3sigExcDis_h", "Q^{2} vs. x_{B}- 3sig+Excl+DIS; x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);

    IMgg_v_MM2tot_h[0] = new TH2F("IMgg_v_MM2tot_raw_h",        "M_{#gamma#gamma} vs. MM^{2}_{ep#gamma#gamma X} - raw;           MM^{2}_{ep#gamma#gamma X} (GeV^{2}/c^{4}); M_{#gamma#gamma} (GeV/c^{2})", 200, -0.5, 0.5, 200, 0.00001, 0.2);
    IMgg_v_MM2tot_h[1] = new TH2F("IMgg_v_MM2tot_broad_h",      "M_{#gamma#gamma} vs. MM^{2}_{ep#gamma#gamma X} - broad;         MM^{2}_{ep#gamma#gamma X} (GeV^{2}/c^{4}); M_{#gamma#gamma} (GeV/c^{2})", 200, -0.5, 0.5, 200, 0.00001, 0.2);
    IMgg_v_MM2tot_h[2] = new TH2F("IMgg_v_MM2tot_3sig_h",       "M_{#gamma#gamma} vs. MM^{2}_{ep#gamma#gamma X} - 3sig;          MM^{2}_{ep#gamma#gamma X} (GeV^{2}/c^{4}); M_{#gamma#gamma} (GeV/c^{2})", 200, -0.5, 0.5, 200, 0.00001, 0.2);
    IMgg_v_MM2tot_h[3] = new TH2F("IMgg_v_MM2tot_3sigExc_h",    "M_{#gamma#gamma} vs. MM^{2}_{ep#gamma#gamma X} - 3sig+Excl;     MM^{2}_{ep#gamma#gamma X} (GeV^{2}/c^{4}); M_{#gamma#gamma} (GeV/c^{2})", 200, -0.5, 0.5, 200, 0.00001, 0.2);
    IMgg_v_MM2tot_h[4] = new TH2F("IMgg_v_MM2tot_3sigExcDis_h", "M_{#gamma#gamma} vs. MM^{2}_{ep#gamma#gamma X} - 3sig+Excl+DIS; MM^{2}_{ep#gamma#gamma X} (GeV^{2}/c^{4}); M_{#gamma#gamma} (GeV/c^{2})", 200, -0.5, 0.5, 200, 0.00001, 0.2);

    MEtot_v_MM2eggX_h[0] = new TH2F("MEtot_v_MM2eggX_h_raw_h",        "ME_{ep#gamma#gamma X} vs. MM^{2}_{e#gamma#gamma X} - raw;           MM^{2}_{e#gamma#gamma X} (GeV^{2}/c^{4}); ME_{ep#gamma#gamma X} (GeV)", 400, -5, 20, 400, -5,  10);
    MEtot_v_MM2eggX_h[1] = new TH2F("MEtot_v_MM2eggX_h_broad_h",      "ME_{ep#gamma#gamma X} vs. MM^{2}_{e#gamma#gamma X} - broad;         MM^{2}_{e#gamma#gamma X} (GeV^{2}/c^{4}); ME_{ep#gamma#gamma X} (GeV)", 400, -5, 20, 400, -5,  10);
    MEtot_v_MM2eggX_h[2] = new TH2F("MEtot_v_MM2eggX_h_3sig_h",       "ME_{ep#gamma#gamma X} vs. MM^{2}_{e#gamma#gamma X} - 3sig;          MM^{2}_{e#gamma#gamma X} (GeV^{2}/c^{4}); ME_{ep#gamma#gamma X} (GeV)", 400, -5, 20, 400, -5,  10);
    MEtot_v_MM2eggX_h[3] = new TH2F("MEtot_v_MM2eggX_h_3sigExc_h",    "ME_{ep#gamma#gamma X} vs. MM^{2}_{e#gamma#gamma X} - 3sig+Excl;     MM^{2}_{e#gamma#gamma X} (GeV^{2}/c^{4}); ME_{ep#gamma#gamma X} (GeV)", 200, -2,  5, 200, -1.5, 2);
    MEtot_v_MM2eggX_h[4] = new TH2F("MEtot_v_MM2eggX_h_3sigExcDis_h", "ME_{ep#gamma#gamma X} vs. MM^{2}_{e#gamma#gamma X} - 3sig+Excl+DIS; MM^{2}_{e#gamma#gamma X} (GeV^{2}/c^{4}); ME_{ep#gamma#gamma X} (GeV)", 200, -2,  5, 200, -1.5, 2);

    IMgg_v_pi0cone_h[0] = new TH2F("IMgg_v_pi0cone_raw_h",        "M_{#gamma#gamma} vs. #pi_{0}-coneangle - raw;           #theta_{#pi_{0}2#gamma} (#circ); M_{#gamma#gamma} (GeV/c^{2})", 300, 0, 40, 200, 0.00001, 0.2);
    IMgg_v_pi0cone_h[1] = new TH2F("IMgg_v_pi0cone_broad_h",      "M_{#gamma#gamma} vs. #pi_{0}-coneangle - broad;         #theta_{#pi_{0}2#gamma} (#circ); M_{#gamma#gamma} (GeV/c^{2})", 300, 0, 40, 200, 0.00001, 0.2);
    IMgg_v_pi0cone_h[2] = new TH2F("IMgg_v_pi0cone_3sig_h",       "M_{#gamma#gamma} vs. #pi_{0}-coneangle - 3sig;          #theta_{#pi_{0}2#gamma} (#circ); M_{#gamma#gamma} (GeV/c^{2})", 300, 0, 40, 200, 0.00001, 0.2);
    IMgg_v_pi0cone_h[3] = new TH2F("IMgg_v_pi0cone_3sigExc_h",    "M_{#gamma#gamma} vs. #pi_{0}-coneangle - 3sig+Excl;     #theta_{#pi_{0}2#gamma} (#circ); M_{#gamma#gamma} (GeV/c^{2})", 300, 0, 40, 200, 0.00001, 0.2);
    IMgg_v_pi0cone_h[4] = new TH2F("IMgg_v_pi0cone_3sigExcDis_h", "M_{#gamma#gamma} vs. #pi_{0}-coneangle - 3sig+Excl+DIS; #theta_{#pi_{0}2#gamma} (#circ); M_{#gamma#gamma} (GeV/c^{2})", 300, 0, 40, 200, 0.00001, 0.2);

    ggIM_h[0] = new TH1F("ggIM_raw_h",        "Invariant mass[#gamma_{1} #gamma_{2}] - raw;           Inv.Mass (GeV/c^{2}); counts", 200, 0.00001, 0.2);
    ggIM_h[1] = new TH1F("ggIM_broad_h",      "Invariant mass[#gamma_{1} #gamma_{2}] - broad;         Inv.Mass (GeV/c^{2}); counts", 200, 0.00001, 0.2);
    ggIM_h[2] = new TH1F("ggIM_3sig_h",       "Invariant mass[#gamma_{1} #gamma_{2}] - 3sig;          Inv.Mass (GeV/c^{2}); counts", 200, 0.00001, 0.2);
    ggIM_h[3] = new TH1F("ggIM_3sigExc_h",    "Invariant mass[#gamma_{1} #gamma_{2}] - 3sig+Excl;     Inv.Mass (GeV/c^{2}); counts", 200, 0.00001, 0.2);
    ggIM_h[4] = new TH1F("ggIM_3sigExcDis_h", "Invariant mass[#gamma_{1} #gamma_{2}] - 3sig+Excl+DIS; Inv.Mass (GeV/c^{2}); counts", 200, 0.00001, 0.2);

    MM2tot_h[0] = new TH1F("MM2tot_raw_h",        "MM^{2}_{ep->e'p'#gamma#gammaX} - raw;           Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);
    MM2tot_h[1] = new TH1F("MM2tot_broad_h",      "MM^{2}_{ep->e'p'#gamma#gammaX} - broad;         Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);
    MM2tot_h[2] = new TH1F("MM2tot_3sig_h",       "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig;          Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);
    MM2tot_h[3] = new TH1F("MM2tot_3sigExc_h",    "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig+Excl;     Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);
    MM2tot_h[4] = new TH1F("MM2tot_3sigExcDis_h", "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig+Excl+DIS; Mis.Mass (GeV^{2}/c^{4}); counts", 200, -0.5, 0.5);

    MEtot_h[0] = new TH1F("MEtot_raw_h",        "Missing Energy [ep->e'p'#gamma#gammaX] - raw;           Mis.Energy (GeV); counts", 400, -5,  10);
    MEtot_h[1] = new TH1F("MEtot_broad_h",      "Missing Energy [ep->e'p'#gamma#gammaX] - broad;         Mis.Energy (GeV); counts", 400, -5,  10);
    MEtot_h[2] = new TH1F("MEtot_3sig_h",       "Missing Energy [ep->e'p'#gamma#gammaX] - 3sig;          Mis.Energy (GeV); counts", 400, -5,  10);
    MEtot_h[3] = new TH1F("MEtot_3sigExc_h",    "Missing Energy [ep->e'p'#gamma#gammaX] - 3sig+Excl;     Mis.Energy (GeV); counts", 200, -1.5, 2);
    MEtot_h[4] = new TH1F("MEtot_3sigExcDis_h", "Missing Energy [ep->e'p'#gamma#gammaX] - 3sig+Excl+DIS; Mis.Energy (GeV); counts", 200, -1.5, 2);

    MM2eggX_h[0] = new TH1F("MM2eggX_raw_h",        "MM^{2}_{e#gamma_{1}#gamma_{2}X} - raw;             MM^{2}_{e#gamma_{1}#gamma_{2}X} (GeV^{2}/c^{4}); counts", 400, -5, 20);
    MM2eggX_h[1] = new TH1F("MM2eggX_broad_h",      "MM^{2}_{e#gamma_{1}#gamma_{2}X} - broad;           MM^{2}_{e#gamma_{1}#gamma_{2}X} (GeV^{2}/c^{4}); counts", 400, -5, 20);
    MM2eggX_h[2] = new TH1F("MM2eggX_3sig_h",       "MM^{2}_{e#gamma_{1}#gamma_{2}X} - 3sig;            MM^{2}_{e#gamma_{1}#gamma_{2}X} (GeV^{2}/c^{4}); counts", 400, -5, 20);
    MM2eggX_h[3] = new TH1F("MM2eggX_3sigExc_h",    "MM^{2}_{e#gamma_{1}#gamma_{2}X} - 3sig+Excl;       MM^{2}_{e#gamma_{1}#gamma_{2}X} (GeV^{2}/c^{4}); counts", 200, -2,  5);
    MM2eggX_h[4] = new TH1F("MM2eggX_3sigExcDis_h", "MM^{2}_{e#gamma_{1}#gamma_{2}X} - 3sig+Excl+DIS;   MM^{2}_{e#gamma_{1}#gamma_{2}X} (GeV^{2}/c^{4}); counts", 200, -2,  5);

    pi0cone_h[0] = new TH1F("pi0cone_raw_h",        "#pi_{0}-coneangle - raw;           #theta_{#pi_{0}2#gamma}(#circ); counts", 300, 0, 40);
    pi0cone_h[1] = new TH1F("pi0cone_broad_h",      "#pi_{0}-coneangle - broad;         #theta_{#pi_{0}2#gamma}(#circ); counts", 300, 0, 40);
    pi0cone_h[2] = new TH1F("pi0cone_3sig_h",       "#pi_{0}-coneangle - 3sig;          #theta_{#pi_{0}2#gamma}(#circ); counts", 300, 0, 40);
    pi0cone_h[3] = new TH1F("pi0cone_3sigExc_h",    "#pi_{0}-coneangle - 3sig+Excl;     #theta_{#pi_{0}2#gamma}(#circ); counts", 300, 0, 40);
    pi0cone_h[4] = new TH1F("pi0cone_3sigExcDis_h", "#pi_{0}-coneangle - 3sig+Excl+DIS; #theta_{#pi_{0}2#gamma}(#circ); counts", 300, 0, 40);

    for (Int_t i = 0; i < chain.GetEntries(); i++)
    {
        chain.GetEntry(i);

        //Raw selected events
        Q2_v_xB_h[0]->Fill(xB, Q2);
        IMgg_v_MM2tot_h[0]->Fill(MM2_total, IM_g1g2);
        MEtot_v_MM2eggX_h[0]->Fill(MM2_rec_recoil, ME_total);
        IMgg_v_pi0cone_h[0]->Fill(pi0coneangle, IM_g1g2);

        ggIM_h[0]->Fill(IM_g1g2);
        MM2tot_h[0]->Fill(MM2_total);
        MEtot_h[0]->Fill(ME_total);
        MM2eggX_h[0]->Fill(MM2_rec_recoil);
        pi0cone_h[0]->Fill(pi0coneangle);
        //"Broad" MM2epggX and cone-angle cuts to clear up some BG
        if (flag_cuts_broadMM2 == 1 && flag_cuts_broadcone == 1)
        {
            Q2_v_xB_h[1]->Fill(xB, Q2);
            IMgg_v_MM2tot_h[1]->Fill(MM2_total, IM_g1g2);
            MEtot_v_MM2eggX_h[1]->Fill(MM2_rec_recoil, ME_total);
            IMgg_v_pi0cone_h[1]->Fill(pi0coneangle, IM_g1g2);

            ggIM_h[1]->Fill(IM_g1g2);
            MM2tot_h[1]->Fill(MM2_total);
            MEtot_h[1]->Fill(ME_total);
            MM2eggX_h[1]->Fill(MM2_rec_recoil);
            pi0cone_h[1]->Fill(pi0coneangle);

        }

        //3-sigma cut around reconstructed pi0-mass
        if (flag_cuts_3sigIM == 1)
        {
            Q2_v_xB_h[2]->Fill(xB, Q2);
            IMgg_v_MM2tot_h[2]->Fill(MM2_total, IM_g1g2);
            MEtot_v_MM2eggX_h[2]->Fill(MM2_rec_recoil, ME_total);
            IMgg_v_pi0cone_h[2]->Fill(pi0coneangle, IM_g1g2);

            ggIM_h[2]->Fill(IM_g1g2);
            MM2tot_h[2]->Fill(MM2_total);
            MEtot_h[2]->Fill(ME_total);
            MM2eggX_h[2]->Fill(MM2_rec_recoil);
            pi0cone_h[2]->Fill(pi0coneangle);

            //Add Exclusivity cuts
            if (flag_cuts_broadcone == 1 && flag_cuts_spectMP == 1 && flag_cuts_3sigMM2 == 1 && flag_cuts_cop == 1)
            {
                Q2_v_xB_h[3]->Fill(xB, Q2);
                IMgg_v_MM2tot_h[3]->Fill(MM2_total, IM_g1g2);
                MEtot_v_MM2eggX_h[3]->Fill(MM2_rec_recoil, ME_total);
                IMgg_v_pi0cone_h[3]->Fill(pi0coneangle, IM_g1g2);

                ggIM_h[3]->Fill(IM_g1g2);
                MM2tot_h[3]->Fill(MM2_total);
                MEtot_h[3]->Fill(ME_total);
                MM2eggX_h[3]->Fill(MM2_rec_recoil);
                pi0cone_h[3]->Fill(pi0coneangle);

                //Add DIS cuts
                if (flag_cuts_dis == 1)
                {
                    Q2_v_xB_h[4]->Fill(xB, Q2);
                    IMgg_v_MM2tot_h[4]->Fill(MM2_total, IM_g1g2);
                    MEtot_v_MM2eggX_h[4]->Fill(MM2_rec_recoil, ME_total);
                    IMgg_v_pi0cone_h[4]->Fill(pi0coneangle, IM_g1g2);

                    ggIM_h[4]->Fill(IM_g1g2);
                    MM2tot_h[4]->Fill(MM2_total);
                    MEtot_h[4]->Fill(ME_total);
                    MM2eggX_h[4]->Fill(MM2_rec_recoil);
                    pi0cone_h[4]->Fill(pi0coneangle);
                }
            }
        }
    }

    TFile *Outfile = new TFile(outfile, "recreate");

    //Loop individually for well formatted output files
    for (int i = 0; i < 5; i++)
    {
        Q2_v_xB_h[i]->Write();
    }

    for (int i = 0; i < 5; i++)
    {
        IMgg_v_MM2tot_h[i]->Write();
    }

    for (int i = 0; i < 5; i++)
    {
        MEtot_v_MM2eggX_h[i]->Write();
    }

    for (int i = 0; i < 5; i++)
    {
        IMgg_v_pi0cone_h[i]->Write();
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
    
    for (int i = 0; i < 5; i++)
    {
        pi0cone_h[i]->Write();
    }

    Outfile->Close();
}