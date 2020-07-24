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
 
  private:

      TFile *Out_File;
      TTree *data;

    	TLorentzVector beam{0, 0, 10.6, 10.6};  //TODO - access directly from DST
			TLorentzVector target{0, 0, 0, 0.93827200};
			TLorentzVector e_scattered{0, 0, 0, 0.00051099891};
			TLorentzVector recoil{0, 0, 0, 0.93827200};
			TLorentzVector phot1{0, 0, 0, 0};
			TLorentzVector phot2{0, 0, 0, 0};

			TLorentzVector deut{0, 0, 0, 1.876};
      TLorentzVector q{0, 0, 0, 0};

      TLorentzVector rec_pi0{0,0,0,0};

      double IM_g1g2;

      TH1F* IM_g1g2_h;
      //TH1F *n_photons_inevent_h;
      //TH1F *n_photonpairs_inevent_h;
      //TH1F *n_pi0_post3sig_h;
      //TH1F *n_FD_onlyhits_h;
      //TH1F *n_FT_onlyhits_h;
      //TH1F *n_FD_doublehits_h;
      //TH1F *photonE_h;
      ClassDefOverride(clas12root::pi0selector,0);

    };
}


#endif // #ifdef pi0selector_cxx
