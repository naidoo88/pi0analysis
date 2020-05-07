#include <cstdlib>
#include <iostream>
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
#include "clas12reader.h"
#include "plothistos.h"

using namespace clas12;

 void plothistos(){
   TFile *Out_File = new TFile("histos.root", "recreate");
   TChain chain("data");
   chain.AddFile("/home/pauln/code/pi0analysis/out.root");

   bool   excl;
   double Q2;
   double tneg;
   double W;
   double xB;
   double IM_g1g2;
   double MM2_total;
   double MM2_total;
   double MM_rec_recoil;
   double MP_rec_spectator;
   double MM_rec_spectator;

   chain.SetBranchAddress("excl", &excl);
   chain.SetBranchAddress("Q2", &Q2);
   chain.SetBranchAddress("tneg", &tneg);
   chain.SetBranchAddress("W", &W);
   chain.SetBranchAddress("xB", &xB);
   chain.SetBranchAddress("IM_g1g2", &IM_g1g2);
   chain.SetBranchAddress("MM2_total", &MM2_total);
   chain.SetBranchAddress("MM2_total", &MM2_total);
   chain.SetBranchAddress("MM_rec_recoil", &MM_rec_recoil);
   chain.SetBranchAddress("MP_rec_spectator", &MP_rec_spectator);
   chain.SetBranchAddress("MM_rec_spectator", &MM_rec_spectator);

   histos();

   for(Int_t i=0;i<chain.GetEntries();i++){
     chain.GetEntry(i);

     /*=====PRE-CUT HISTOS=====*/
     IM_g1g2_h[0]  -> Fill(IM_g1g2);
     MM2_total_h[0] -> Fill(MM2_total);
     MM2_total_h[0]  -> Fill(MM2_total);
     Q2_h[0]     -> Fill(Q2);
     xB_h[0]     -> Fill(xB);
     Q2xB_h[0]   -> Fill(xB, Q2);
     tneg_h[0]   -> Fill(tneg);
     W_h[0]      -> Fill(W);

     pi0_immm2_h[0] -> Fill(MM2_total, IM_g1g2);
     pi0_immp_h[0]  -> Fill(MM2_total, IM_g1g2);
     pi0_mm2mp_h[0] -> Fill(MM2_total, MM2_total);

     MM_rec_recoil_h[0]      -> Fill(MM_rec_recoil);
     spectneutmm_h[0]    -> Fill(MM_rec_spectator);
     spectneutmp_h[0]    -> Fill(MP_rec_spectator);
     spectneut_mpmm_h[0] -> Fill(MM_rec_spectator, MP_rec_spectator);

     /*=====POST-MM2_total-CUT HISTOS=====*/
     if(MM2_total < -0.0853 || MM2_total > 0.0718) continue;
     IM_g1g2_h[1]  -> Fill(IM_g1g2);
     MM2_total_h[1] -> Fill(MM2_total);
     MM2_total_h[1]  -> Fill(MM2_total);
     Q2_h[1]     -> Fill(Q2);
     xB_h[1]     -> Fill(xB);
     Q2xB_h[1]   -> Fill(xB, Q2);
     tneg_h[1]   -> Fill(tneg);
     W_h[1]      -> Fill(W);

     pi0_immm2_h[1] -> Fill(MM2_total, IM_g1g2);
     pi0_immp_h[1]  -> Fill(MM2_total, IM_g1g2);
     pi0_mm2mp_h[1] -> Fill(MM2_total, MM2_total);

     MM_rec_recoil_h[1]      -> Fill(MM_rec_recoil);
     spectneutmm_h[1]    -> Fill(MM_rec_spectator);
     spectneutmp_h[1]    -> Fill(MP_rec_spectator);
     spectneut_mpmm_h[1] -> Fill(MM_rec_spectator, MP_rec_spectator);

     /*=====POST-spectMP-CUT HISTOS=====*/
     if(MP_rec_spectator > 0.3) continue;
     IM_g1g2_h[2]  -> Fill(IM_g1g2);
     MM2_total_h[2] -> Fill(MM2_total);
     MM2_total_h[2]  -> Fill(MM2_total);
     Q2_h[2]     -> Fill(Q2);
     xB_h[2]     -> Fill(xB);
     Q2xB_h[2]   -> Fill(xB, Q2);
     tneg_h[2]   -> Fill(tneg);
     W_h[2]      -> Fill(W);

     pi0_immm2_h[2] -> Fill(MM2_total, IM_g1g2);
     pi0_immp_h[2]  -> Fill(MM2_total, IM_g1g2);
     pi0_mm2mp_h[2] -> Fill(MM2_total, MM2_total);

     MM_rec_recoil_h[2]      -> Fill(MM_rec_recoil);
     spectneutmm_h[2]    -> Fill(MM_rec_spectator);
     spectneutmp_h[2]    -> Fill(MP_rec_spectator);
     spectneut_mpmm_h[2] -> Fill(MM_rec_spectator, MP_rec_spectator);

   }//chain-loop

   Out_File->Write();
   Out_File->Close();

   cout << "seems coo" << endl;
 }//fxn


 void histos(){
   // [0] - raw;
   // [1] - post MM^2 cut
   // [2] - post rec neut mp cut

   //Exclusivity variables
   //---------------------------------------------------------------------------------------------------------------
   tneg_h[0] = new TH1F("tneg_0_h", "-t - Before pi0MM^{2} cut; -t (GeV^{2}); counts",               150, 0, 10);
   tneg_h[1] = new TH1F("tneg_1_h", "-t; -t (GeV^{2}); counts",                                      150, 0, 10);
   tneg_h[2] = new TH1F("tneg_2_h", "-t - Post spectMP cut; -t (GeV^{2}); counts",                   150, 0, 10);

   Q2_h[0]   = new TH1F("Q2_0_h", "Q^{2} - Before pi0MM^{2} cut; Q^{2} (GeV^{2}); counts",           150, 0, 8);
   Q2_h[1]   = new TH1F("Q2_1_h", "Q^{2}; Q^{2} (GeV^{2}); counts",                                  150, 0, 8);
   Q2_h[2]   = new TH1F("Q2_2_h", "Q^{2} - Post spectMP cut; Q^{2} (GeV^{2}); counts",               150, 0, 8);

   xB_h[0]   = new TH1F("xB_0_h", "xB - Before pi0MM^{2} cut; xB; counts",                           100, 0, 20);
   xB_h[1]   = new TH1F("xB_1_h", "xB; xB; counts",                                                  100, 0, 20);
   xB_h[2]   = new TH1F("xB_2_h", "xB - Post spectMP cut; xB; counts",                               100, 0, 20);

   W_h[0]    = new TH1F("W_0_h", "CoM Energy - Before pi0MM^{2} cut; W (GeV^{2}); counts",           120, 0, 30);
   W_h[1]    = new TH1F("W_1_h", "CoM Energy; W (GeV^{2}); counts",                                  120, 0, 30);
   W_h[2]    = new TH1F("W_2_h", "CoM Energy - Post spectMP cut; W (GeV^{2}); counts",               120, 0, 30);

   Q2xB_h[0] = new TH2F("Q2xB_0_h", "Q{2} vs. x_{B} - Before pi0MM^{2} cut; x_{B}; Q^{2} (GeV^{2})", 250, 0, 20, 150, 0, 8);
   Q2xB_h[1] = new TH2F("Q2xB_1_h", "Q{2} vs. x_{B}; x_{B}; Q^{2} (GeV^{2})",                        250, 0, 20, 150, 0, 8);
   Q2xB_h[2] = new TH2F("Q2xB_2_h", "Q{2} vs. x_{B} - Post spectMP cut; x_{B}; Q^{2} (GeV^{2})",     250, 0, 20, 150, 0, 8);

   //Channel masses etc.
   //---------------------------------------------------------------------------------------------------------------
   IM_g1g2_h[0]  = new TH1F("IM_g1g2_0_h", "Invariant mass of paired photons - Before pi0MM^{2} cut; Inv.Mass (GeV); counts", 200, 0, 0.2);
   IM_g1g2_h[1]  = new TH1F("IM_g1g2_1_h", "Invariant mass of paired photons; Inv.Mass (GeV); counts",                        200, 0, 0.2);
   IM_g1g2_h[2]  = new TH1F("IM_g1g2_2_h", "Invariant mass of paired photons - Post spectMP cut; Inv.Mass (GeV); counts",     200, 0, 0.2);

   MM2_total_h[0] = new TH1F("MM2_total_0_h", "Missing Mass-Squared [e p -> e' p' #gamma_{1} #gamma_{2}] - Before pi0MM^{2} cut; MissingMass-Sq (GeV^{2}); counts", 200, -.5, .5);
   MM2_total_h[1] = new TH1F("MM2_total_1_h", "Missing Mass-Squared [e p -> e' p' #gamma_{1} #gamma_{2}]; MissingMass-Sq (GeV^{2}); counts",                        200, -.5, .5);
   MM2_total_h[2] = new TH1F("MM2_total_2_h", "Missing Mass-Squared [e p -> e' p' #gamma_{1} #gamma_{2}] - Post spectMP cut; MissingMass-Sq (GeV^{2}); counts",     200, -.5, .5);

   MM2_total_h[0]  = new TH1F("MM2_total_0_h", "Missing Momentum [e p -> e' p' #gamma_{1} #gamma_{2}] - Before pi0MM^{2} cut; MissingMomentum (GeV); counts",         200, 0, 6);
   MM2_total_h[1]  = new TH1F("MM2_total_1_h", "Missing Momentum [e p -> e' p' #gamma_{1} #gamma_{2}]; MissingMomentum-Sq (GeV); counts",                             200, 0, 6);
   MM2_total_h[2]  = new TH1F("MM2_total_2_h", "Missing Momentum [e p -> e' p' #gamma_{1} #gamma_{2}] - Post spectMP cut; MissingMomentum-Sq (GeV); counts",          200, 0, 6);

   pi0_immm2_h[0] = new TH2F ("pi0_immm2_0_h", "Inv. Mass of photon-pair vs. MM^{2}[e p -> e' p' #gamma_{1} #gamma_{2}] Pre-pi0MM^{2} cut; MissingMass-Sq (GeV^{2}); Inv.Mass (GeV)", 200, -.5, .5, 200, 0, 0.2);
   pi0_immm2_h[1] = new TH2F ("pi0_immm2_1_h", "Inv. Mass of photon-pair vs. MM^{2}[e p -> e' p' #gamma_{1} #gamma_{2}]; MissingMass-Sq (GeV^{2}); Inv.Mass (GeV)",                   200, -.5, .5, 200, 0, 0.2);
   pi0_immm2_h[2] = new TH2F ("pi0_immm2_2_h", "Inv. Mass of photon-pair vs. MM^{2}[e p -> e' p' #gamma_{1} #gamma_{2}]- Post spectMP cut; MissingMass-Sq (GeV^{2}); Inv.Mass (GeV)", 200, -.5, .5, 200, 0, 0.2);

   pi0_immp_h[0]  = new TH2F ("pi0_immp_0_h", "Inv.Mass [#gamma_{1}#gamma_{2}] vs. MP[e p -> e' p' #gamma_{1} #gamma_{2}] - Pre-pi0MM^{2} cut; MissingMomentum (GeV); Inv.Mass (GeV)",   200, 0, 6, 200, 0, 0.2);
   pi0_immp_h[1]  = new TH2F ("pi0_immp_1_h", "Inv.Mass [#gamma_{1}#gamma_{2}] vs. MP[e p -> e' p' #gamma_{1} #gamma_{2}]; MissingMomentum (GeV); Inv.Mass (GeV)",                       200, 0, 6, 200, 0, 0.2);
   pi0_immp_h[2]  = new TH2F ("pi0_immp_2_h", "Inv.Mass [#gamma_{1}#gamma_{2}] vs. MP[e p -> e' p' #gamma_{1} #gamma_{2}] - Post spectMP cut; MissingMomentum (GeV); Inv.Mass (GeV)",    200, 0, 6, 200, 0, 0.2);

   pi0_mm2mp_h[0] = new TH2F ("pi0_mm2mp_0_h", "[e p -> e' p' #gamma_{1} #gamma_{2}]: MM^{2} vs. Mp - Pre-pi0MM^{2} cut; MissingMomentum (GeV); MissingMass-Sq (GeV^{2})",              200, 0, 6, 200, -.5, .5);
   pi0_mm2mp_h[1] = new TH2F ("pi0_mm2mp_1_h", "[e p -> e' p' #gamma_{1} #gamma_{2}]: MM^{2} vs. Mp; MissingMomentum (GeV); MissingMass-Sq (GeV^{2})",                                  200, 0, 6, 200, -.5, .5);
   pi0_mm2mp_h[2] = new TH2F ("pi0_mm2mp_2_h", "[e p -> e' p' #gamma_{1} #gamma_{2}]: MM^{2} vs. Mp - Post spectMP cut; MissingMomentum (GeV); MissingMass-Sq (GeV^{2})",               200, 0, 6, 200, -.5, .5);

   //Reconstructed particles for BG clean-up
   //---------------------------------------------------------------------------------------------------------------
   MM_rec_recoil_h[0]   = new TH1F("MM_rec_recoil_0_h", "MM[e p -> e' #gamma_{1} #gamma_{2}] - Before pi0MM^{2} cut; MM of reconstructed proton (GeV); counts",                180, 0, 3);
   MM_rec_recoil_h[1]   = new TH1F("MM_rec_recoil_1_h", "MM[e p -> e' #gamma_{1} #gamma_{2}]; MM of reconstructed proton (GeV); counts",                                       180, 0, 3);
   MM_rec_recoil_h[2]   = new TH1F("MM_rec_recoil_2_h", "MM[e p -> e' #gamma_{1} #gamma_{2}] - Post spectMP cut; MM of reconstructed proton (GeV); counts",                    180, 0, 3);

   spectneutmp_h[0] = new TH1F("spectneutmp_0_h", "MP[e d -> e' p' #gamma_{1} #gamma_{2}] - Before pi0MM^{2} cut; MP of reconstructed spect. neutron (GeV/c); counts", 200, 0, 6);
   spectneutmp_h[1] = new TH1F("spectneutmp_1_h", "MP[e d -> e' p' #gamma_{1} #gamma_{2}]; MP of reconstructed spect. neutron (GeV/c); counts",                        200, 0, 6);
   spectneutmp_h[2] = new TH1F("spectneutmp_2_h", "MP[e d -> e' p' #gamma_{1} #gamma_{2}]- Post spectMP cut; MP of reconstructed spect. neutron (GeV/c); counts",      200, 0, 6);

   spectneutmm_h[0] = new TH1F("spectneutmm_0_h", "MM[e d -> e' p' #gamma_{1} #gamma_{2}] - Before pi0MM^{2} cut; MM of reconstructed spect. neutron (GeV); counts",   200, 0, 6);
   spectneutmm_h[1] = new TH1F("spectneutmm_1_h", "MM[e d -> e' p' #gamma_{1} #gamma_{2}]; MM of reconstructed spect. neutron (GeV); counts",                          200, 0, 6);
   spectneutmm_h[2] = new TH1F("spectneutmm_2_h", "MM[e d -> e' p' #gamma_{1} #gamma_{2}] - Post spectMP cut; MM of reconstructed spect. neutron (GeV); counts",       200, 0, 6);

   spectneut_mpmm_h[0] = new TH2F("spectneut_mpmm_0_h", "[e d -> e' p' #gamma_{1} #gamma_{2}]: MP vs. MM - Before pi0MM^{2} cut; MM of rec. spect. neutron (GeV); MissingMom. (GeV/c)", 200, 0, 6, 200, 0, 6);
   spectneut_mpmm_h[1] = new TH2F("spectneut_mpmm_1_h", "[e d -> e' p' #gamma_{1} #gamma_{2}]: MP vs. MM; MM of rec. spect. neutron (GeV); MissingMom. (GeV/c)",                        200, 0, 6, 200, 0, 6);
   spectneut_mpmm_h[2] = new TH2F("spectneut_mpmm_2_h", "[e d -> e' p' #gamma_{1} #gamma_{2}]: MP vs. MM - Post spectMP cut; MM of rec. spect. neutron (GeV); MissingMom. (GeV/c)",     200, 0, 6, 200, 0, 6);
 }//histos fxn
