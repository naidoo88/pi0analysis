#include <cstdlib>
#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <TString.h>

void DVEPplots(TString infile, TString outfile)
{	
    TChain chain("data");
	chain.AddFile(infile);

    double Q2;
    double xB;
	double IM_g1g2;
	double MM2_total;
    double MM2_rec_recoil;

	bool flag_cuts_3sigIM;
    bool flag_cuts_dis;
    bool flag_cuts_W2;
    bool flag_cuts_broadcone;
    bool flag_cuts_spectMP; 
    bool flag_cuts_3sigMM2;
    bool flag_cuts_cop;
    bool flag_cuts_broadMM2;

	chain.SetBranchAddress("Q2",             &Q2);
	chain.SetBranchAddress("xB",             &xB);
	chain.SetBranchAddress("IM_g1g2",        &IM_g1g2);
	chain.SetBranchAddress("MM2_total",      &MM2_total);
   	chain.SetBranchAddress("MM2_rec_recoil", &MM2_rec_recoil);

	chain.SetBranchAddress("flag_cuts_3sigIM",    &flag_cuts_3sigIM);
    chain.SetBranchAddress("flag_cuts_dis",       &flag_cuts_dis);
    chain.SetBranchAddress("flag_cuts_W2",        &flag_cuts_W2);
    chain.SetBranchAddress("flag_cuts_broadcone", &flag_cuts_broadcone);
    chain.SetBranchAddress("flag_cuts_spectMP",   &flag_cuts_spectMP);
    chain.SetBranchAddress("flag_cuts_3sigMM2",   &flag_cuts_3sigMM2);
    chain.SetBranchAddress("flag_cuts_cop",       &flag_cuts_cop);
    chain.SetBranchAddress("flag_cuts_broadMM2",  &flag_cuts_broadMM2);

    TH1F* ggIM_h[3];
    TH1F* MM2tot_h[4];
    TH1F* MM2eggX_h[3];
    TH2F* Q2xB_h[5];

    Q2xB_h[0] = new TH2F("Q2xB_raw_h",        "Q^{2} vs. x_{B}; x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2xB_h[1] = new TH2F("Q2xB_3sig_h",       "Q^{2} vs. x_{B}; x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2xB_h[2] = new TH2F("Q2xB_3sigExc_h",    "Q^{2} vs. x_{B}; x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2xB_h[3] = new TH2F("Q2xB_3sigExcDis_h", "Q^{2} vs. x_{B}; x_{B}; Q^{2}", 200, 0, 1.6, 200, 0, 12);
    Q2xB_h[4] = new TH2F("Q2xB_broad_h",      "Q^{2} vs. x_{B}; x_{B}; Q^{2}", 200, 0, 1.2, 200, 0, 12);

	ggIM_h[0] = new TH1F("ggIM_3sig_h",        "Invariant mass[#gamma_{1} #gamma_{2}] - 3sig;          Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	ggIM_h[1] = new TH1F("ggIM_3sigExc_h",     "Invariant mass[#gamma_{1} #gamma_{2}] - 3sig+Excl;     Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);
	ggIM_h[2] = new TH1F("ggIM_3sigExcDis_h",  "Invariant mass[#gamma_{1} #gamma_{2}] - 3sig+Excl+DIS; Inv.Mass (GeV/c^{2}); counts", 200, 0, 0.2);

    MM2tot_h[0] = new TH1F("MM2tot_3sig_h",        "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig;          Inv.Mass (GeV/c^{2}); counts", 200, -0.5, 0.5);
	MM2tot_h[1] = new TH1F("MM2tot_3sigExc_h",     "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig+Excl;     Inv.Mass (GeV/c^{2}); counts", 200, -0.5, 0.5);
	MM2tot_h[2] = new TH1F("MM2tot_3sigExcDis_h",  "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig+Excl+DIS; Inv.Mass (GeV/c^{2}); counts", 200, -0.5, 0.5);
	MM2tot_h[3] = new TH1F("MM2tot_broad_h",  "MM^{2}_{ep->e'p'#gamma#gammaX} - 3sig+Excl+DIS; Inv.Mass (GeV/c^{2}); counts", 200, -0.5, 0.5);

	MM2eggX_h[0] = new TH1F("MM2eggX_3sig_h",       "MM^{2}_{e#gamma_{1}#gamma_{2}X} - 3sig;            MM^{2}_{e#gamma_{1}#gamma_{2}X}(GeV^{2}/c^{4}); counts", 200, -5, 5);
	MM2eggX_h[1] = new TH1F("MM2eggX_3sigExc_h",    "MM^{2}_{e#gamma_{1}#gamma_{2}X} - 3sig+Excl;       MM^{2}_{e#gamma_{1}#gamma_{2}X}(GeV^{2}/c^{4}); counts", 200, -5, 5);
	MM2eggX_h[2] = new TH1F("MM2eggX_3sigExcDis_h", "MM^{2}_{e#gamma_{1}#gamma_{2}X} - 3sig+Excl+DIS;   MM^{2}_{e#gamma_{1}#gamma_{2}X}(GeV^{2}/c^{4}); counts", 200, -5, 5);


	for(Int_t i=0;i<chain.GetEntries();i++){
		chain.GetEntry(i);

        Q2xB_h[0] -> Fill(xB, Q2);
        if(flag_cuts_broadMM2 == 1 && flag_cuts_broadcone == 1)
        {
            Q2xB_h[4] -> Fill(xB, Q2);
            MM2tot_h[3] -> Fill(MM2_total);
        }

        if(flag_cuts_3sigIM == 1)
        {
		    ggIM_h[0]->Fill(IM_g1g2);
            MM2tot_h[0]->Fill(MM2_total);
            MM2eggX_h[0]->Fill(MM2_rec_recoil);
            Q2xB_h[1] -> Fill(xB, Q2);

            if(flag_cuts_broadcone == 1 && flag_cuts_spectMP == 1 && flag_cuts_3sigMM2 == 1 && flag_cuts_cop == 1)
            {
                ggIM_h[1]->Fill(IM_g1g2);
                MM2tot_h[1]->Fill(MM2_total);
                MM2eggX_h[1]->Fill(MM2_rec_recoil);
                Q2xB_h[2] -> Fill(xB, Q2);

                if(flag_cuts_dis == 1)
                {
                ggIM_h[2]->Fill(IM_g1g2);
                MM2tot_h[2]->Fill(MM2_total);
                MM2eggX_h[2]->Fill(MM2_rec_recoil);
                Q2xB_h[3] -> Fill(xB, Q2);
                }
            }
        
        }
    }
    
    TFile* Outfile = new TFile(outfile, "recreate");
    
    for (int i = 0; i < 5; i++)
    {
        Q2xB_h[i] ->  Write();
    }

    for (int i = 0; i < 3; i++)
    {
        ggIM_h[i]->Write();
    }
    
    for (int i = 0; i < 4; i++)
    {
        MM2tot_h[i]->Write();
    }
    
    for (int i = 0; i < 3; i++)
    {
        MM2eggX_h[i]->Write();
    }

    Outfile->Close();

}