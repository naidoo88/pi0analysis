// @(#)root

/* 
//Script reads helicity and Trento-phi with the hadronic plane defined three ways: 
// 1. Recoil x New Particle (ie. Meson or Photon)
// 2. Recoil x Virtual-Photon
// 3. New Particle x Virtual-Photon
// It then plots helicity asymmetries as a function of Trento-phi
*/

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
#include <TF1.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <TString.h>
#include <TMath.h>
#include <TFile.h>

#include "clas12reader.h"

//void plotasym(TString datafile)//, TString outfile);

using namespace clas12;

void plotasym(TString datafile, TString outfile){
	TChain chain("data");
	chain.AddFile(datafile);

	int    helicity;
	double phi_Nnew;
    double phi_Nvg;  
    double phi_vgnew;
    bool   flag_goodpi0;
    bool   flag_cuts_dis;
    bool   flag_cuts_broadMM2;
    bool   flag_cuts_broadcone;
    bool   flag_cuts_spectMP;
    double MM2_total;
    double cop_Nvg_vgnew;  


	int n_zerohel = 0;

    TH1F *R_N_phi_h  = new TH1F("R_N_phi_h",  "Trento #phi# - Recoil/NewPart; phi; counts",      30, 0, 360);
	TH1F *R_vg_phi_h = new TH1F("R_vg_phi_h", "Trento #phi# - Recoil/VirtualPhot; phi; counts",  30, 0, 360);
    TH1F *N_vg_phi_h = new TH1F("N_vg_phi_h", "Trento #phi# - NewPart/VirtualPhot; phi; counts", 30, 0, 360);


	TH1F *R_N_helplus_h  = new TH1F("R_N_helplus_h",  "helplus; phi; counts", 30, 0, 360);
	TH1F *R_N_helmin_h   = new TH1F("R_N_helmin_h ",  "helmin;  phi; counts", 30, 0, 360);
	TH1F *R_N_helasym_h  ;

	TH1F *R_vg_helplus_h = new TH1F("R_vg_helplus_h", "helplus; phi; counts", 30, 0, 360);
	TH1F *R_vg_helmin_h  = new TH1F("R_vg_helmin_h",  "helmin;  phi; counts", 30, 0, 360);
	TH1F *R_vg_helasym_h ;

    TH1F *N_vg_helplus_h = new TH1F("N_vg_helplus_h", "helplus; phi; counts", 30, 0, 360);
	TH1F *N_vg_helmin_h  = new TH1F("N_vg_helmin_h",  "helmin;  phi; counts", 30, 0, 360);
	TH1F *N_vg_helasym_h ;

	chain.SetBranchAddress("helicity",  &helicity); 
	chain.SetBranchAddress("phi_Nnew",  &phi_Nnew);
    chain.SetBranchAddress("phi_Nvg",   &phi_Nvg);  
    chain.SetBranchAddress("phi_vgnew", &phi_vgnew);
    chain.SetBranchAddress("flag_goodpi0",   &flag_goodpi0);
    chain.SetBranchAddress("flag_cuts_dis",  &flag_cuts_dis);
    chain.SetBranchAddress("flag_cuts_broadMM2",  &flag_cuts_broadMM2);
    chain.SetBranchAddress("flag_cuts_broadcone", &flag_cuts_broadcone);
    chain.SetBranchAddress("flag_cuts_spectMP",   &flag_cuts_spectMP);  
    chain.SetBranchAddress("MM2_total",           &MM2_total);
    chain.SetBranchAddress("cop_Nvg_vgnew",       &cop_Nvg_vgnew);  


	for(Int_t i=0;i<chain.GetEntries();i++){
        chain.GetEntry(i);
        
        if(flag_goodpi0  != 1) continue;  //corresponds to 3sig cut on pi0-mass
        if(flag_cuts_dis != 1) continue;
        if(flag_cuts_broadMM2  != 1) continue; //} 
        if(flag_cuts_broadcone != 1) continue; //} correspond to very loose excl cuts  
        if(flag_cuts_spectMP   != 1) continue; //}
        if(!(MM2_total > -0.05155 && MM2_total < 0.03455)) continue; //can be changed to flag_cuts_3sigMM2 soon
        if(!(std::abs(cop_Nvg_vgnew) < 5)) continue; //coplanarity between Recoil/virtPhot & newPart/virtPhot

        R_N_phi_h  ->Fill(phi_Nnew);
        R_vg_phi_h ->Fill(phi_Nvg);
        N_vg_phi_h ->Fill(phi_vgnew);

		if(helicity == 1){
			R_N_helplus_h->Fill(phi_Nnew);
            R_vg_helplus_h->Fill(phi_Nvg);
            N_vg_helplus_h->Fill(phi_vgnew);
        }
        else if (helicity == -1){
            R_N_helmin_h->Fill(phi_Nnew);
            R_vg_helmin_h->Fill(phi_Nvg);
            N_vg_helmin_h->Fill(phi_vgnew);
        }
        else if (helicity == 0){
		n_zerohel++;
		}
	}
    
    //Fill asymmetries
   	R_vg_helasym_h = (TH1F*) R_vg_helplus_h->GetAsymmetry(R_vg_helmin_h);
    R_vg_helasym_h->SetName("Asym_R_vg_h");

	R_N_helasym_h  = (TH1F*) R_N_helplus_h ->GetAsymmetry(R_N_helmin_h);
    R_N_helasym_h->SetName("Asym_R_N_h");

	N_vg_helasym_h = (TH1F*) N_vg_helplus_h->GetAsymmetry(N_vg_helmin_h);
    N_vg_helasym_h->SetName("Asym_N_vg_h");

    //Fit the asymmetries
    TString fit_fxn = "[0] + [1]*sin(x)";
    TF1 *asymm_fits[3];
    asymm_fits[0] = new TF1("Asym_R_vg_fit", fit_fxn, 0, 360);
    asymm_fits[1] = new TF1("Asym_R_N_fit",  fit_fxn, 0, 360);
    asymm_fits[2] = new TF1("Asym_N_vg_fit", fit_fxn, 0, 360);

    R_N_helasym_h  -> Fit(asymm_fits[0], "RM");
    R_vg_helasym_h -> Fit(asymm_fits[1], "RM");
    N_vg_helasym_h -> Fit(asymm_fits[2], "RM");

    //Write out histograms
    TFile *Out_File = new TFile(outfile, "recreate"); 

    R_N_phi_h ->Write();
    R_vg_phi_h->Write();
    N_vg_phi_h->Write();

    R_N_helplus_h ->Write();
    R_N_helmin_h  ->Write();
    R_N_helasym_h ->Write();

    R_vg_helplus_h->Write();
    R_vg_helmin_h ->Write();
    R_vg_helasym_h->Write();

    N_vg_helplus_h->Write();
    N_vg_helmin_h ->Write();
    N_vg_helasym_h->Write();

    Out_File->Close();

    cout << n_zerohel << " events with zero helicity" << endl;
}