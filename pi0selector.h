#ifndef pi0selector_h
#define pi0selector_h

#include "HipoSelector.h"
#include <TLorentzVector.h>
#include <TH1F.h>

namespace clas12root{
   
  class pi0selector : public HipoSelector {
      public :


      pi0selector() =default;
      pi0selector(TString filename) ;
      pi0selector(HipoChain *chain) ;
      virtual ~pi0selector() { }
      void    Begin(TTree *tree) final;
      void    SlaveBegin(TTree *tree) final;
      
      Bool_t ProcessEvent() final;
      
      void    SlaveTerminate() final;
      void    Terminate() final;

      void AddFilter() override;

      void counterhistos();
      void calc_angles(TVector3 Ebeam_vect, TVector3 Electron_vect, TVector3 Recoil_vect, TVector3 Newpart_vect);
      void photonflags(clas12::region_part_ptr p1, clas12::region_part_ptr p2);

      void SetOutFileName(TString Out_Name);

 
  private:

      TString Out_File_Name;
      TFile *Out_File;
      TTree *data;

      double DEG = TMath::RadToDeg(); //rad->deg conversion

      /*  Variables  */
      int helicity;
      double Q2;
      double tneg;
      double W2;
      double xB;

      double IM_g1g2;
      double MM2_total;
      double MP_total;
      double ME_total;
      double pi0coneangle;
      double eg1coneangle;
      double eg2coneangle;

      double MM_rec_recoil;
      double MM2_rec_recoil;
      double MP_rec_recoil;

      double MM_rec_spectator;
      double MM2_rec_spectator;
      double MP_rec_spectator;

      double phi_Nvg;
      double phi_Nnew;
      double phi_vgnew;
      double cop_Nvg_vgnew;
      double cop_Nvg_Nnew;
      double cop_Nnew_vgnew;

      /*  LorentzVectors  */
      TLorentzVector beam{0, 0, 10.6, 10.6};  //TODO - access directly from DST
      TLorentzVector target{0, 0, 0, 0.939565420};
      TLorentzVector e_scattered{0, 0, 0, 0.00051099891};
      TLorentzVector recoil{0, 0, 0, 0.939565420};
      TLorentzVector phot1{0, 0, 0, 0};
      TLorentzVector phot2{0, 0, 0, 0};

      TLorentzVector deut{0, 0, 0, 1.876};
      TLorentzVector q{0, 0, 0, 0};

      TLorentzVector system{0,0,0,0};        //[e N -> e' R g1 g2]
      TLorentzVector rec_pi0{0,0,0,0};
      TLorentzVector rec_recoil{0,0,0,0};	   //[e p -> e' g1 g2 X]
      TLorentzVector rec_spectator{0,0,0,0}; //[e D -> e' R g1 g2 X]
      TLorentzVector expected_pi0{0,0,0,0};


      /*  Flags  */
      bool flag_2photon_event;
      bool flag_photon1_FD;
      bool flag_photon1_FT;
      bool flag_photon1_PCAL;
      bool flag_photon1_ECAL;
      bool flag_photon1_EIN;
      bool flag_photon1_EOUT;
      bool flag_photon1_onlyPCAL;
      bool flag_photon1_onlyECAL;
      bool flag_photon2_FD;
      bool flag_photon2_FT;
      bool flag_photon2_PCAL;
      bool flag_photon2_ECAL;
      bool flag_photon2_EIN;
      bool flag_photon2_EOUT;
      bool flag_photon2_onlyPCAL;
      bool flag_photon2_onlyECAL;

      bool flag_cuts_3sigIM; 
      bool flag_cuts_3sigIMfull;
      bool flag_cuts_bestpi0;
      bool flag_cuts_goodpi0;
      bool flag_cuts_dis;
      bool flag_cuts_excl;
      bool flag_cuts_W2;
      bool flag_cuts_broadMM2;
      bool flag_cuts_broadcone;
      bool flag_cuts_spectMP; 
      bool flag_cuts_3sigMM2;
      bool flag_cuts_cop;
      bool flag_cuts_photonE;
      bool flag_cuts_egcone;

      /*  Counters  */
      int n_files;
      int n_events;
      int n_photons_inevent;
      int n_photonpairs_inevent;
      int n_pi0_post3sig;
      int n_pi0_goodcandidates;

      int n_FD_onlyhits;
      int n_FT_onlyhits;
      int n_FD_doublehits;
      int n_ECAL_doublehits;

      int n_dis_events;
      int n_excl_events;

      /* Dip-Check Histos */
      TH1F *IM_g1g2_h;
      
      /*  Counter Histos  */
      TH1F *n_photons_inevent_h;
      TH1F *n_photonpairs_inevent_h;
      TH1F *n_pi0_post3sig_h;
      TH1F *n_FD_onlyhits_h;
      TH1F *n_FT_onlyhits_h;
      TH1F *n_FD_doublehits_h;
      TH1F *photonE_h;

      /* Particle 4vects */
      double e_px;
      double e_py;
      double e_pz;
      double e_E;
      double rec_px;
      double rec_py;
      double rec_pz;
      double rec_E;
      double phot1_px;
      double phot1_py;
      double phot1_pz;
      double phot1_E;
      double phot2_px;
      double phot2_py;
      double phot2_pz;
      double phot2_E;
      
      ClassDefOverride(clas12root::pi0selector,0);

    };
}


#endif // #ifdef pi0selector_cxx
