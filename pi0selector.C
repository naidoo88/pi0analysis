#define pi0selector_cxx
// The class definition in pi0selector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//


#include "pi0selector.h"
#include <TH2.h>
#include "TCanvas.h"
#include <TSystem.h>

using std::cout; 
using std::endl;

namespace clas12root{
  
  pi0selector::pi0selector(TString filename) : HipoSelector(filename) {
   
  }
  pi0selector::pi0selector(HipoChain* chain) : HipoSelector(chain) {
  
  }

  void pi0selector::Begin(TTree * /*tree*/)
  {
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    HipoSelector::Begin(0); //Do not remove this line!
  }
  
  void pi0selector::SlaveBegin(TTree * /*tree*/)
  {
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).
    cout << "In SlaveBegin" << endl;


    data = new TTree("data", "Processed Data.");
   	data->Branch("IM_g1g2",      &IM_g1g2,      "IM_g1g2/D");

    IM_g1g2_h = new TH1F("IM_g1g2_h", "IM_g1g2", 200, 0, 0.2);

    fOutput->Add(IM_g1g2_h);
    fOutput->Add(data);

    HipoSelector::SlaveBegin(0); //Do not remove this line!
    
  }
  
  void pi0selector::AddFilter(){
    cout << "In AddFilter" << endl << endl;

    //_c12->addExactPid(2112, 1); //one neutron
    _c12->addExactPid(2212, 1); //one proton
    _c12->addExactPid(11, 1);	  //one electron
    _c12->addAtLeastPid(22, 2); //at least 2 photons
    _c12->addZeroOfRestPid();	  //no other particles
   }

  Bool_t pi0selector::ProcessEvent(){
        cout << "In ProcessEvent" << endl << endl;

    //Equivalent to TSelector Process
    //Fill in what you would like to do with
    //the data for each event....

    	auto electronbuff = _c12->getByID(11);
			auto recoilbuff = _c12->getByID(2212);
			//auto recoilbuff   = _c12->getByID(2112);
			auto photonbuff   = _c12->getByID(22);
      
      //if (recoilbuff.empty() == false) return kTRUE; //work around for BAND neutrons which cause segfault.

      recoil.SetXYZM(recoilbuff[0]->par()->getPx(),
			  recoilbuff[0]->par()->getPy(),
			  recoilbuff[0]->par()->getPz(),
			  recoil.M());

      e_scattered.SetXYZM(electronbuff[0]->par()->getPx(),
        electronbuff[0]->par()->getPy(),
        electronbuff[0]->par()->getPz(),
        e_scattered.M());

      q = beam - e_scattered;

      Int_t n_photons_inevent = photonbuff.size();
      cout << n_photons_inevent << "in event" << endl;

      for (int i = 0; i < n_photons_inevent - 1; i++){
			  for (int j = i + 1; j < n_photons_inevent; j++){
        cout << "pair [" << i << "][" << j << "]" << endl;
        phot1.SetXYZM(photonbuff[i]->par()->getPx(),
			    photonbuff[i]->par()->getPy(),
			    photonbuff[i]->par()->getPz(),
			    0);

        phot2.SetXYZM(photonbuff[j]->par()->getPx(),
			    photonbuff[j]->par()->getPy(),
			    photonbuff[j]->par()->getPz(),
			    0);

        rec_pi0 = phot1 + phot2;
        IM_g1g2   = rec_pi0.M();
        IM_g1g2_h->Fill(IM_g1g2);
        }
      }

      data->Fill();
    return kTRUE;
  }
 
 void pi0selector::SlaveTerminate()
  {
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.
      
  }

  void pi0selector::Terminate()
  {
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.
    cout << "In Terminate" << endl << endl;


    TCanvas* can = new TCanvas();
    IM_g1g2_h->Draw();
    // cout << "is this where we die?" << endl << endl;
    // Out_File->Write();
    // cout << "Or is it perhaps?" << endl << endl;

    Out_File = new TFile("pi0prooftest.root", "recreate");
	  TListIter *iter = (TListIter*)GetOutputList()->MakeIterator();
	  for (TObject *obj = (*iter)(); obj != 0; obj = iter->Next()) {
		  obj->Write();
	  }

    Out_File->Close();
  }

  // void SetLorentzVector(TLorentzVector &p4,clas12::region_part_ptr rp){
  // p4.SetXYZM(rp->par()->getPx(),rp->par()->getPy(),
	//      rp->par()->getPz(),p4.M());
    
  // }
} 
