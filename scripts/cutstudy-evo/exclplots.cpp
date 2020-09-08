//@root
#include <stdio.h>
#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TBenchmark.h"

#include "hist_fxns.h"
//^^^^^^^^^^^^^^^^^^^^^  https://root-forum.cern.ch/t/adding-include-paths-to-cint/8281/5

using std::cout;
using std::endl;

TH1Array1D_Ptr calcAsymmetry(TH1Array2D_Ptr hel_plus_h, TH1Array2D_Ptr hel_min_h, TString asym_name, TString asym_title, vector<TString> &cut, vector<TString> &cut_title);

void exclplots(TString infile, TString outfile)
{
	TStopwatch time;
	time.Start();

	cout << "Begin..." << endl;

 	gStyle->SetOptFit(01111);
	gStyle->SetOptStat(11);

	cout << "Set up read-out..." << endl;

	TChain chain("data");
	chain.AddFile(infile);

	double IM_g1g2;
	double MM2_total;
	double ME_total;
	double MM_rec_recoil;
	double MP_rec_spectator;
	double MM_rec_spectator;

  double pi0coneangle;
	double phot1_E;
	double phot2_E;
	double eg1coneangle;
	double eg2coneangle;

  double phi_Nvg;
	double phi_Nnew;
	double phi_vgnew;
	double cop_Nvg_vgnew;
  double cop_Nvg_Nnew;
  double cop_Nnew_vgnew;
	int helicity;

	bool flag_2photon_event;
  bool flag_cuts_3sigIM; 
	bool flag_cuts_spectMP;

	chain.SetBranchAddress("IM_g1g2", &IM_g1g2);
	chain.SetBranchAddress("MM2_total", &MM2_total);
	chain.SetBranchAddress("ME_total", &ME_total);
	chain.SetBranchAddress("MM_rec_recoil", &MM_rec_recoil);
	chain.SetBranchAddress("MM_rec_spectator", &MM_rec_spectator);
	chain.SetBranchAddress("MP_rec_spectator", &MP_rec_spectator);
	chain.SetBranchAddress("pi0coneangle", &pi0coneangle);
	chain.SetBranchAddress("phot1_E", &phot1_E);
	chain.SetBranchAddress("phot2_E", &phot2_E);
	chain.SetBranchAddress("eg1coneangle", &eg1coneangle);
	chain.SetBranchAddress("eg2coneangle", &eg2coneangle);
	chain.SetBranchAddress("phi_Nvg", &phi_Nvg);
	chain.SetBranchAddress("phi_Nnew", &phi_Nnew);
	chain.SetBranchAddress("phi_vgnew", &phi_vgnew);
	chain.SetBranchAddress("cop_Nvg_vgnew", &cop_Nvg_vgnew);
	chain.SetBranchAddress("cop_Nvg_Nnew", &cop_Nvg_Nnew);
	chain.SetBranchAddress("cop_Nnew_vgnew", &cop_Nnew_vgnew);	
	chain.SetBranchAddress("helicity", &helicity);
	chain.SetBranchAddress("flag_2photon_event", &flag_2photon_event);
	chain.SetBranchAddress("flag_cuts_3sigIM", &flag_cuts_3sigIM);
	chain.SetBranchAddress("flag_cuts_spectMP", &flag_cuts_spectMP);

	cout << "Setting up histograms... " << endl;

	hist_fxns h;

	//--cumulation histos
	vector<TString> cut = {"0", "1", "2", "3", "4"}; 
	vector<TString> cut_title = {"Raw", 
															 "#pi_{0} 3#sigma cut", 
															 "#pi_{0} 3#sigma cut & MP_{eD->en#pi0X}<0.7GeV", 
															 "#pi_{0} 3#sigma cut, MP_{eD->en#pi0X}<0.7GeV & #theta_{#phi_{#pi_{0}}#phi_{n}}<5^{#circ}",
															 "#pi_{0} 3#sigma cut, MP_{eD->en#pi0X}<0.7GeV, #theta_{#phi_{#pi_{0}}#phi_{n}}<5^{#circ} & E_{#gamma} > 1GeV"}; 
	float MMspect_bins[3] = {200, 0, 5.5};
	auto MMspect_h = h.createTH1PtrArray1D("MMspect", MMspect_bins, "MM_{eD->en#pi_{0}X}", cut, cut_title);

	float MMrecoil_bins[3] = {200, 0, 4.5};
	auto MMrecoil_h = h.createTH1PtrArray1D("MMrecoil", MMrecoil_bins, "MM_{en->e#pi_{0}X}", cut, cut_title);

	float MM2totWide_bins[3] = {400, -10, 10};
	float MM2tot_bins[3] = {100, -1, 1};
	auto MM2totWide_h = h.createTH1PtrArray1D("MM2totWide", MM2totWide_bins, "MM2_{en->en#pi_{0}X} - Wide", cut, cut_title);
	auto MM2tot_h = h.createTH1PtrArray1D("MM2tot", MM2tot_bins, "MM2_{en->en#pi_{0}X}", cut, cut_title);

	
	float phi_bins[3] = {30, 0, 360};
	vector<TString> phi_vars = {"R_N", "R_vg", "N_vg"};
	vector<TString> phi_titles = {"Recoil/NewPart", "Recoil/VirtualPhot", "NewPart/VirtualPhot"};
	auto phi_h = h.createTH1PtrArray2D("phi", phi_bins, "#phi_{Trento}", cut, cut_title, phi_vars, phi_titles);

	float hel_bins[3] = {6, 0, 360};
	vector<TString> hel_vars = {"plus", "min"};
	vector<TString> hel_titles = {"Positive", "Negative"};
	auto hel_RN_h  = h.createTH1PtrArray2D("hel_RN",  hel_bins, "Helicity - R-N",  cut, cut_title, hel_vars, hel_titles);
	auto hel_Rvg_h = h.createTH1PtrArray2D("hel_Rvg", hel_bins, "Helicity - R-vg", cut, cut_title, hel_vars, hel_titles);
	auto hel_Nvg_h = h.createTH1PtrArray2D("hel_Nvg", hel_bins, "Helicity - N-vg", cut, cut_title, hel_vars, hel_titles);

	cout << "Looping through file... " << endl;

	for(Int_t i=0;i<chain.GetEntries();i++){
		chain.GetEntry(i);

		//if (i % 1000 == 0) cout << "Event: " << i << endl; 

		//if(flag_cuts_3sigIM  != 1) continue;  //corresponds to 3sig cut on pi0-mass
		//if(flag_cuts_dis != 1) continue;
		//if(flag_cuts_broadMM2  != 1) continue; //} 
		//if(flag_cuts_broadcone != 1) continue; //} correspond to very loose excl cuts  
		//if(flag_cuts_spectMP   != 1) continue; //}
		//if(!(MM2_total > -0.05155 && MM2_total < 0.03455)) continue; //can be changed to flag_cuts_3sigMM2 soon
		//if(!(std::abs(cop_Nvg_vgnew) < 5)) continue; //coplanarity between Recoil/virtPhot & newPart/virtPhot

		MMspect_h[0]->Fill(MM_rec_spectator);
		MMrecoil_h[0]->Fill(MM_rec_recoil);
		MM2totWide_h[0]->Fill(MM2_total);
		MM2tot_h[0]->Fill(MM2_total);

		phi_h[0][0] ->Fill(phi_Nnew);
		phi_h[0][1] ->Fill(phi_Nvg);
		phi_h[0][2] ->Fill(phi_vgnew);
		
		if (helicity == 1){
			hel_RN_h[0][0] ->Fill(phi_Nnew); 
			hel_Rvg_h[0][0]->Fill(phi_Nvg);
			hel_Nvg_h[0][0]->Fill(phi_vgnew);
		}
		else if (helicity == -1){
			hel_RN_h[0][1] ->Fill(phi_Nnew); 
			hel_Rvg_h[0][1]->Fill(phi_Nvg);
			hel_Nvg_h[0][1]->Fill(phi_vgnew);	
		}


		if(flag_cuts_3sigIM  == 1)
		{
			MMspect_h[1]->Fill(MM_rec_spectator);
			MMrecoil_h[1]->Fill(MM_rec_recoil);
			MM2totWide_h[1]->Fill(MM2_total);
			MM2tot_h[1]->Fill(MM2_total);

			phi_h[1][0] ->Fill(phi_Nnew);
			phi_h[1][1] ->Fill(phi_Nvg);
			phi_h[1][2] ->Fill(phi_vgnew);

			if (helicity == 1){
				hel_RN_h[1][0] ->Fill(phi_Nnew); 
				hel_Rvg_h[1][0]->Fill(phi_Nvg);
				hel_Nvg_h[1][0]->Fill(phi_vgnew);
			}
			else if (helicity == -1){
				hel_RN_h[1][1] ->Fill(phi_Nnew); 
				hel_Rvg_h[1][1]->Fill(phi_Nvg);
				hel_Nvg_h[1][1]->Fill(phi_vgnew);	
			}


			if(flag_cuts_spectMP   == 1)
			{
				MMspect_h[2]->Fill(MM_rec_spectator);
				MMrecoil_h[2]->Fill(MM_rec_recoil);
				MM2totWide_h[2]->Fill(MM2_total);
				MM2tot_h[2]->Fill(MM2_total);

				phi_h[2][0] ->Fill(phi_Nnew);
				phi_h[2][1] ->Fill(phi_Nvg);
				phi_h[2][2] ->Fill(phi_vgnew);

				if (helicity == 1){
					hel_RN_h[2][0] ->Fill(phi_Nnew); 
					hel_Rvg_h[2][0]->Fill(phi_Nvg);
					hel_Nvg_h[2][0]->Fill(phi_vgnew);
				}
				else if (helicity == -1){
					hel_RN_h[2][1] ->Fill(phi_Nnew); 
					hel_Rvg_h[2][1]->Fill(phi_Nvg);
					hel_Nvg_h[2][1]->Fill(phi_vgnew);	
				}

				
				if((std::abs(cop_Nvg_vgnew) < 5)){
					MMspect_h[3]->Fill(MM_rec_spectator);
					MMrecoil_h[3]->Fill(MM_rec_recoil);
					MM2totWide_h[3]->Fill(MM2_total);
					MM2tot_h[3]->Fill(MM2_total);

					phi_h[3][0] ->Fill(phi_Nnew);
					phi_h[3][1] ->Fill(phi_Nvg);
					phi_h[3][2] ->Fill(phi_vgnew);

					if (helicity == 1){
						hel_RN_h[3][0] ->Fill(phi_Nnew); 
						hel_Rvg_h[3][0]->Fill(phi_Nvg);
						hel_Nvg_h[3][0]->Fill(phi_vgnew);
					}
					else if (helicity == -1){
						hel_RN_h[3][1] ->Fill(phi_Nnew); 
						hel_Rvg_h[3][1]->Fill(phi_Nvg);
						hel_Nvg_h[3][1]->Fill(phi_vgnew);	
					}


					if(phot1_E > 1 && phot2_E > 1)
					{
						MMspect_h[4]->Fill(MM_rec_spectator);
						MMrecoil_h[4]->Fill(MM_rec_recoil);
						MM2totWide_h[4]->Fill(MM2_total);
						MM2tot_h[4]->Fill(MM2_total);

						phi_h[4][0] ->Fill(phi_Nnew);
						phi_h[4][1] ->Fill(phi_Nvg);
						phi_h[4][2] ->Fill(phi_vgnew);

						if (helicity == 1){
							hel_RN_h[4][0] ->Fill(phi_Nnew); 
							hel_Rvg_h[4][0]->Fill(phi_Nvg);
							hel_Nvg_h[4][0]->Fill(phi_vgnew);
						}
						else if (helicity == -1){
							hel_RN_h[4][1] ->Fill(phi_Nnew); 
							hel_Rvg_h[4][1]->Fill(phi_Nvg);
							hel_Nvg_h[4][1]->Fill(phi_vgnew);	
						}

					}
				}//coplanarity
			}//spectMP
		}//3sigpi0IM

	}//chain loop

	cout << "Processing asymmetries..." << endl;
	TH1Array1D_Ptr asym_RN_h 	= calcAsymmetry(hel_RN_h,  hel_RN_h,  "asym_RN",  "BSA_{RN}",  cut, cut_title);
	TH1Array1D_Ptr asym_Rvg_h	= calcAsymmetry(hel_Rvg_h, hel_Rvg_h, "asym_Rvg", "BSA_{Rvg}", cut, cut_title);
	TH1Array1D_Ptr asym_Nvg_h	= calcAsymmetry(hel_Nvg_h, hel_Nvg_h, "asym_Nvg", "BSA_{Nvg}", cut, cut_title);

	for (u_int i = 0; i < asym_RN_h.size(); i++){
		cout << "post asym: asym_RN_h[" << i << "] is -- " << asym_RN_h[i]->GetName() << endl; 
	} 

	cout << "Writting out to file..." << endl;
	TFile* OutFile = new TFile(outfile, "RECREATE");
	h.writeHistos(MMspect_h);
	h.writeHistos(MMrecoil_h);
	h.writeHistos(MM2totWide_h);
	h.writeHistos(MM2tot_h);
	h.writeHistos(phi_h);
	h.writeHistos(hel_RN_h);
	h.writeHistos(hel_Rvg_h);
	h.writeHistos(hel_Nvg_h);



	h.writeHistos(asym_RN_h);
	h.writeHistos(asym_Rvg_h);
	h.writeHistos(asym_Nvg_h);

	// h.writeHistos(asym_RN_h);
	// h.writeHistos(asym_Rvg_h);
	// h.writeHistos(asym_Nvg_h);

	OutFile->Close();

}//fxn

TH1Array1D_Ptr calcAsymmetry(TH1Array2D_Ptr hel_plus_h, TH1Array2D_Ptr hel_min_h, TString asym_name, TString asym_title, vector<TString> &cut, vector<TString> &cut_title){
	    
  //Fill asymmetries
	//cout << "   In calcAsym." << endl;
	
	TH1Array1D_Ptr asym_h;

	for(u_int j=0; j < cut.size(); j++)
	{
		//cout << "   Hel+: " << hel_plus_h[j][0]->GetName() << " Hel-: " << hel_min_h[j][1]->GetName() << endl; 
		
		TH1F* buff_h = (TH1F*) hel_plus_h[j][0]->GetAsymmetry(hel_min_h[j][1]);
		//cout << "   Created asym: " << buff_h->GetName() << endl;

		auto histName = Form("%s_%s_h", asym_name.Data(), cut[j].Data());
		auto histTitle = Form("%s: %s", asym_title.Data(), cut_title[j].Data());
		buff_h->SetNameTitle(histName, histTitle);

		//cout << "   Renamed it: " << buff_h->GetName() << endl << endl;

		asym_h.push_back(buff_h);
	}


  // //Fit the asymmetries
  TString fit_fxn = "[0] + [1]*sin(TMath::DegToRad()*(x))";
  TF1 *asym_fit[asym_h.size()];

	for (u_int j = 0; j < asym_h.size(); j++)
	{
		auto fitName = Form("%s_fit", asym_h[j]->GetName());
		asym_fit[j] = new TF1(fitName, fit_fxn, 0, 360);
		asym_fit[j]->SetParNames("const", "P_{b} A_{LU}^{sin#phi}");

		asym_h[j]->Fit(asym_fit[j], "RM");
	}


	return asym_h;
  // for (int i = 0; i < 3; i++)
  // {
  //     asymm_fits[i]->SetParNames("const", "P_{b} A_{LU}^{sin#phi}");
  // }
  
  // R_N_helasym_h  -> Fit(asymm_fits[0], "RM");
  // R_vg_helasym_h -> Fit(asymm_fits[1], "RM");
  // N_vg_helasym_h -> Fit(asymm_fits[2], "RM");
}