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
#include "TCanvas.h"
#include <TSystem.h>
#include "clas12reader.h"

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
      data->Branch("helicity", &helicity, "helicity/I");

      data->Branch("Q2",   &Q2,   "Q2/D");
      data->Branch("tneg", &tneg, "tneg/D");
      data->Branch("W2",   &W2,   "W2/D");
      data->Branch("xB",   &xB,   "xB/D");

      data->Branch("IM_g1g2",      &IM_g1g2,      "IM_g1g2/D");
      data->Branch("MM2_total",    &MM2_total,    "MM2_total/D");
      data->Branch("MP_total",     &MP_total,     "MP_total/D");
      data->Branch("ME_total",     &ME_total,     "ME_total/D");
      data->Branch("pi0coneangle", &pi0coneangle, "pi0coneangle/D");
      data->Branch("eg1coneangle", &eg1coneangle, "eg1coneangle/D");
      data->Branch("eg2coneangle", &eg2coneangle, "eg2coneangle/D");

      data->Branch("MM_rec_recoil",     &MM_rec_recoil,     "MM_rec_recoil/D");
      data->Branch("MM2_rec_recoil",    &MM2_rec_recoil,    "MM2_rec_recoil/D");
      data->Branch("MP_rec_recoil",     &MP_rec_recoil,     "MP_rec_recoil/D");

      data->Branch("MM_rec_spectator",  &MM_rec_spectator,  "MM_rec_spectator/D");
      data->Branch("MM2_rec_spectator", &MM2_rec_spectator, "MM2_rec_spectator/D");
      data->Branch("MP_rec_spectator",  &MP_rec_spectator,  "MP_rec_spectator/D");

      data->Branch("phi_Nvg",        &phi_Nvg,        "phi_Nvg/D");
      data->Branch("phi_Nnew",       &phi_Nnew,       "phi_Nnew/D");
      data->Branch("phi_vgnew",      &phi_vgnew,      "phi_vgnew/D");
      data->Branch("cop_Nvg_vgnew",  &cop_Nvg_vgnew,  "cop_Nvg_vgnew/D");
      data->Branch("cop_Nvg_Nnew",   &cop_Nvg_Nnew,   "cop_Nvg_Nnew/D");
      data->Branch("cop_Nnew_vgnew", &cop_Nnew_vgnew, "cop_Nnew_vgnew/D");

      data->Branch("flag_2photon_event",   &flag_2photon_event,   "flag_2photon_event/O");

      data->Branch("flag_photon1_FD",   &flag_photon1_FD,   "flag_photon1_FD/O");
      data->Branch("flag_photon1_FT",   &flag_photon1_FT,   "flag_photon1_FT/O");
      data->Branch("flag_photon1_PCAL", &flag_photon1_PCAL, "flag_photon1_PCAL/O");
      data->Branch("flag_photon1_ECAL", &flag_photon1_ECAL, "flag_photon1_ECAL/O");
      data->Branch("flag_photon1_EIN",  &flag_photon1_EIN,  "flag_photon1_EIN/O");
      data->Branch("flag_photon1_EOUT", &flag_photon1_EOUT, "flag_photon1_EOUT/O");

      data->Branch("flag_photon2_FD",   &flag_photon2_FD,   "flag_photon2_FD/O");
      data->Branch("flag_photon2_FT",   &flag_photon2_FT,   "flag_photon2_FT/O");
      data->Branch("flag_photon2_PCAL", &flag_photon2_PCAL, "flag_photon2_PCAL/O");
      data->Branch("flag_photon2_ECAL", &flag_photon2_ECAL, "flag_photon2_ECAL/O");
      data->Branch("flag_photon2_EIN",  &flag_photon2_EIN,  "flag_photon2_EIN/O");
      data->Branch("flag_photon2_EOUT", &flag_photon2_EOUT, "flag_photon2_EOUT/O");

      data->Branch("flag_photon1_onlyPCAL", &flag_photon1_onlyPCAL, "flag_photon1_onlyPCAL/O");
      data->Branch("flag_photon1_onlyECAL", &flag_photon1_onlyECAL, "flag_photon1_onlyECAL/O");
      data->Branch("flag_photon2_onlyPCAL", &flag_photon2_onlyPCAL, "flag_photon2_onlyPCAL/O");
      data->Branch("flag_photon2_onlyECAL", &flag_photon2_onlyECAL, "flag_photon2_onlyECAL/O");

      data->Branch("flag_cuts_3sigIM",     &flag_cuts_3sigIM,     "flag_cuts_3sigIM/O");
      data->Branch("flag_cuts_3sigIMfull", &flag_cuts_3sigIMfull, "flag_cuts_3sigIMfull/O");
      data->Branch("flag_cuts_bestpi0",    &flag_cuts_bestpi0,    "flag_cuts_bestpi0/O");
      data->Branch("flag_cuts_goodpi0",    &flag_cuts_goodpi0,    "flag_cuts_goodpi0/O");
      data->Branch("flag_cuts_dis",        &flag_cuts_dis,        "flag_cuts_dis/O");
      data->Branch("flag_cuts_excl",       &flag_cuts_excl,       "flag_cuts_excl/O");
      data->Branch("flag_cuts_W2",         &flag_cuts_W2,         "flag_cuts_W2/O");
      data->Branch("flag_cuts_broadMM2",   &flag_cuts_broadMM2,   "flag_cuts_broadMM2/O");
      data->Branch("flag_cuts_broadcone",  &flag_cuts_broadcone,  "flag_cuts_broadcone/O");
      data->Branch("flag_cuts_spectMP",    &flag_cuts_spectMP,    "flag_cuts_spectMP/O");
      data->Branch("flag_cuts_3sigMM2",    &flag_cuts_3sigMM2,    "flag_cuts_3sigMM2/O");
      data->Branch("flag_cuts_cop",        &flag_cuts_cop,        "flag_cuts_cop/O");
      data->Branch("flag_cuts_photonE",    &flag_cuts_photonE,    "flag_cuts_photonE/O");
      data->Branch("flag_cuts_egcone",     &flag_cuts_egcone,     "flag_cuts_egcone/O");
      
      data->Branch("n_photons_inevent",     &n_photons_inevent,     "n_photons_inevent/O");
      //data->Branch("n_photonpairs_inevent", &n_photonpairs_inevent, "n_photonpairs_inevent/O");  <-- needs to be written out as a histogram if needed
      data->Branch("n_pi0_post3sig",        &n_pi0_post3sig,        "n_pi0_post3sig/O");
      data->Branch("n_pi0_goodcandidates",  &n_pi0_goodcandidates,  "n_pi0_goodcandidates/O");

      data->Branch("n_FD_onlyhits",         &n_FD_onlyhits,         "n_FD_onlyhits/O");
      data->Branch("n_FT_onlyhits",         &n_FT_onlyhits,         "n_FT_onlyhits/O");
      data->Branch("n_FD_doublehits",       &n_FD_doublehits,       "n_FD_doublehits/O");
      data->Branch("n_ECAL_doublehits",     &n_ECAL_doublehits,     "n_ECAL_doublehits/O");

      data->Branch("n_dis_events",          &n_dis_events,          "n_dis_events/O");
      data->Branch("n_excl_events",         &n_excl_events,         "n_excl_events/O");

      data->Branch("e_px",     &e_px,     "e_px/D");
      data->Branch("e_py",     &e_py,     "e_py/D");
      data->Branch("e_pz",     &e_pz,     "e_pz/D");
      data->Branch("e_E",      &e_E,      "e_E/D");
      data->Branch("rec_px",   &rec_px,   "rec_px/D");
      data->Branch("rec_py",   &rec_py,   "rec_py/D");
      data->Branch("rec_pz",   &rec_pz,   "rec_pz/D");
      data->Branch("rec_E",    &rec_E,    "rec_E/D");
      data->Branch("phot1_px", &phot1_px, "phot1_px/D");
      data->Branch("phot1_py", &phot1_py, "phot1_py/D");
      data->Branch("phot1_pz", &phot1_pz, "phot1_pz/D");
      data->Branch("phot1_E",  &phot1_E,  "phot1_E/D");
      data->Branch("phot2_px", &phot2_px, "phot2_px/D");
      data->Branch("phot2_py", &phot2_py, "phot2_py/D");
      data->Branch("phot2_pz", &phot2_pz, "phot2_pz/D");
      data->Branch("phot2_E",  &phot2_E,  "phot2_E/D");
    
    //data->SetDirectory(Out_File);
    GetOutputList()->Add(data);

    IM_g1g2_h = new TH1F("IM_g1g2_h", "IM_g1g2", 200, 0, 0.2);
    //IM_g1g2_h->SetDirectory(Out_File);
    GetOutputList()->Add(IM_g1g2_h);

    counterhistos();

    HipoSelector::SlaveBegin(0); //Do not remove this line!
    
  }
  
  void pi0selector::AddFilter(){
    cout << "In AddFilter" << endl << endl;

    _c12->addExactPid(2112, 1); //one neutron
    //_c12->addExactPid(2212, 1); //one proton
    _c12->addExactPid(11, 1);	  //one electron
    _c12->addAtLeastPid(22, 2); //at least 2 photons
    _c12->addZeroOfRestPid();	  //no other particles
   }

  Bool_t pi0selector::ProcessEvent(){
    //Equivalent to TSelector Process
    //Fill in what you would like to do with
    //the data for each event....
      //cout << "In ProcessEvent" << endl << endl;

    	auto electronbuff = _c12->getByID(11);
			//auto recoilbuff = _c12->getByID(2212);
			auto recoilbuff   = _c12->getByID(2112);
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

      rec_px = recoil.Px();
      rec_py = recoil.Py();
      rec_pz = recoil.Pz();
      rec_E  = recoil.E();

      e_px = e_scattered.Px();
      e_py = e_scattered.Py();
      e_pz = e_scattered.Pz();
      e_E  = e_scattered.E();

      q = beam - e_scattered;

      //helicity = c12.helonline()->getHelicity();
      helicity = _c12->event()->getHelicity();

      n_photonpairs_inevent = 0;
      n_pi0_post3sig        = 0;
      n_pi0_goodcandidates  = 0;
      n_pi0_post3sig        = 0;
      n_FD_onlyhits         = 0;
      n_FT_onlyhits         = 0;
      n_FD_doublehits       = 0;

      n_photons_inevent     = photonbuff.size();
      n_photons_inevent_h  -> Fill(n_photons_inevent);

      double Eg_threshold = 1.5;  //Reduce BG by flagging photons with energy higher than

      if (photonbuff.size() == 2)
        flag_2photon_event = 1;
      else
        flag_2photon_event = 0;
      

      for (int i = 0; i < n_photons_inevent - 1; i++){
			  for (int j = i + 1; j < n_photons_inevent; j++){
        //cout << "pair [" << i << "][" << j << "]" << endl;
        phot1.SetXYZM(photonbuff[i]->par()->getPx(),
			    photonbuff[i]->par()->getPy(),
			    photonbuff[i]->par()->getPz(),
			    0);

        phot2.SetXYZM(photonbuff[j]->par()->getPx(),
			    photonbuff[j]->par()->getPy(),
			    photonbuff[j]->par()->getPz(),
			    0);

        phot1_px = phot1.Px();
        phot1_py = phot1.Py();
        phot1_pz = phot1.Pz();
        phot1_E  = phot1.E();

        phot2_px = phot2.Px();
        phot2_py = phot2.Py();
        phot2_pz = phot2.Pz();
        phot2_E  = phot2.E();

        photonE_h -> Fill(phot1.E());
        photonE_h -> Fill(phot2.E());

        eg1coneangle = (e_scattered.Angle(phot1.Vect())) * DEG;
        eg2coneangle = (e_scattered.Angle(phot2.Vect())) * DEG;

        if ((eg1coneangle > 5) || (eg2coneangle > 5))
          flag_cuts_egcone = 1;
        else
          flag_cuts_egcone = 0;

        rec_pi0 = phot1 + phot2;
        IM_g1g2   = rec_pi0.M();
        IM_g1g2_h->Fill(IM_g1g2);

        system    = (beam + target) - (e_scattered + recoil + rec_pi0); //[e p -> e' p' g1 g2]
        MM2_total = system.M2();
        MP_total  = system.P();
        ME_total  = system.E();

        expected_pi0 = target + beam - e_scattered - recoil;
        pi0coneangle = (expected_pi0.Angle(rec_pi0.Vect())) * DEG;

        rec_recoil        = (beam + target) - (e_scattered + rec_pi0); //reconstruct recoil [e p -> e' g1 g2 X]
        MP_rec_recoil     = rec_recoil.P();
        MM_rec_recoil     = rec_recoil.M();
        MM2_rec_recoil    = rec_recoil.M2();

        rec_spectator     = (beam + deut) - (e_scattered + recoil + rec_pi0); //reconstruct spectator [e D -> e' p' g1 g2 X]
        MP_rec_spectator  = rec_spectator.P();
        MM_rec_spectator  = rec_spectator.M();
        MM2_rec_spectator = rec_spectator.M2();

        //Calc trento-phi and co-planarity angles:
        calc_angles(beam.Vect(), e_scattered.Vect(), recoil.Vect(), rec_pi0.Vect());

        //Set Photon flags:
        photonflags(photonbuff[i], photonbuff[j]);

        //TODO -- these are always 1 because they're written within this pairing loop
        if (flag_photon1_EIN == 1 && flag_photon1_EOUT == 1)
          n_ECAL_doublehits++;
        if (flag_photon2_EIN == 1 && flag_photon2_EOUT == 1)
          n_ECAL_doublehits++;

        if (flag_photon1_FD == 1 && flag_photon2_FD == 1)
          n_FD_onlyhits++;
        if (flag_photon1_FT == 1 && flag_photon2_FT == 1)
          n_FT_onlyhits++;
        if ((flag_photon1_FD == 1 && flag_photon2_FT == 1) || (flag_photon2_FD == 1 && flag_photon1_FT == 1))
          n_FD_doublehits++;							

        //###################################################################################
        // Set 3sigma IM_gg cut flags.  ("full" PCAL/ECAL/FT split or FD/FT split)
        // [IM_gg fitted with cuts: MM2 +/- 0.5GeV && pi0coneangle < 20deg (somewhat arb.)]
        //###################################################################################

        /* FD/FT split (linear BG) 3 sigma cut gives the following:
                // ---- both FD: Mean: 0.13100  Sig: 0.01406 =>  Lower: 0.08882   Upper: 0.17318
                // ---- both FT: Mean: 0.13110  Sig: 0.00642 =>  Lower: 0.11184   Upper: 0.15036
                // ---- 1FD/1FT: Mean: 0.12610  Sig: 0.01483 =>  Lower: 0.08161   Upper: 0.17059
        */
        if ((flag_photon1_FD == 1 && flag_photon2_FD == 1) && (IM_g1g2 > 0.08882 && IM_g1g2 < 0.17318)){
          flag_cuts_3sigIM = 1;
          n_pi0_post3sig++;
        }
        else if ((flag_photon1_FT == 1 && flag_photon2_FT == 1) && (IM_g1g2 > 0.11184 && IM_g1g2 < 0.15036)){
          flag_cuts_3sigIM = 1;
          n_pi0_post3sig++;
        }
        else if (((flag_photon1_FD == 1 && flag_photon2_FT == 1) || (flag_photon2_FD == 1 && flag_photon1_FT == 1)) && (IM_g1g2 > 0.08161 && IM_g1g2 < 0.17059)){
          flag_cuts_3sigIM = 1;
          n_pi0_post3sig++;
        }
        else
          flag_cuts_3sigIM = 0;							

        /* "full" split 3 sigma cut gives the following:
                // ---- both PCAL: Mean: 0.131183  Sig: 0.0132358 => Lower: 0.091476   Upper: 0.170891
                // ---- both ECAL: Mean: 0.134668  Sig: 0.0110543 => Lower: 0.101505   Upper: 0.167831
                // ---- both FCAL: Mean: 0.131060  Sig: 0.0060639 => Lower: 0.112868   Upper: 0.149252
                // ---- PCAL/ECAL: Mean: 0.132082  Sig: 0.0127805 => Lower: 0.0937409  Upper: 0.170424
                // ---- ECAL/FCAL: Mean: 0.127126  Sig: 0.0107954 => Lower: 0.0947396  Upper: 0.159512
                // ---- PCAL/FCAL: Mean: 0.126721  Sig: 0.0119468 => Lower: 0.0908801  Upper: 0.162561
                */
        if ((flag_photon1_PCAL == 1 && flag_photon2_PCAL == 1) && (IM_g1g2 > 0.091476 && IM_g1g2 < 0.170891)){
          flag_cuts_3sigIMfull = 1;
        }
        else if ((flag_photon1_ECAL == 1 && flag_photon2_ECAL == 1) && (IM_g1g2 > 0.101505 && IM_g1g2 < 0.167831)){
          flag_cuts_3sigIMfull = 1;
        }
        else if ((flag_photon1_FT == 1 && flag_photon2_FT == 1) && (IM_g1g2 > 0.112868 && IM_g1g2 < 0.149252)){
          flag_cuts_3sigIMfull = 1;
        }
        else if (((flag_photon1_PCAL == 1 && flag_photon2_ECAL == 1) || (flag_photon2_PCAL == 1 && flag_photon1_ECAL == 1)) && (IM_g1g2 > 0.0937409 && IM_g1g2 < 0.170424)){
          flag_cuts_3sigIMfull = 1;
        }
        else if (((flag_photon1_ECAL == 1 && flag_photon2_FT == 1) || (flag_photon2_ECAL == 1 && flag_photon1_FT == 1)) && (IM_g1g2 > 0.0947396 && IM_g1g2 < 0.159512)){
          flag_cuts_3sigIMfull = 1;
        }
        else if (((flag_photon1_PCAL == 1 && flag_photon2_FT == 1) || (flag_photon2_PCAL == 1 && flag_photon1_FT == 1)) && (IM_g1g2 > 0.0908801 && IM_g1g2 < 0.162561)){
          flag_cuts_3sigIMfull = 1;
        }
        else
          flag_cuts_3sigIMfull = 0;

        //###################################################################################
        // Set Cut Flags -- DIS cuts, excl' cuts.
        //###################################################################################

        /*======= DIS CUTS =======*/
        Q2   = -1 * q.M2();
        xB   = Q2 / (2 * (target.Dot(q)));
        tneg = -1 * (recoil - target).M2(); //**DOUBLE CHECK THIS AGAINST THEORY PAPER**

        if ((Q2 > 1) && (tneg < 1)){
          flag_cuts_dis = 1;
          n_dis_events++;
        }
        else
          flag_cuts_dis = 0;

        W2 = (recoil + rec_pi0).M2();

        if (W2 > 4){
          flag_cuts_W2 = 1;
        }
        else
          flag_cuts_W2 = 0;

        /*======= (WIP) EXCL. CUTS =======*/
        //TODO
        if (MM2_total > -0.5 && MM2_total < 0.5){
          flag_cuts_broadMM2 = 1;
        }
        else
          flag_cuts_broadMM2 = 0;

        if (pi0coneangle < 20){
          flag_cuts_broadcone = 1;
        }
        else
          flag_cuts_broadcone = 0;

        if (MP_rec_spectator < 0.7){ //700MeV cut on spectator missing momentum -- previously discussed 300MeV
          flag_cuts_spectMP = 1;
        }
        else
          flag_cuts_spectMP = 0;

        if (MM2_total > -0.05155 && MM2_total < 0.03455){ //3sigma cut where: mu = -0.0085, sig = 0.01435
          flag_cuts_3sigMM2 = 1;
        }
        else
          flag_cuts_3sigMM2 = 0;

        if (std::abs(cop_Nvg_vgnew) < 5){
          flag_cuts_cop = 1;
        }
        else
          flag_cuts_cop = 0;
        //###################################################################################

        data->Fill();
        n_photonpairs_inevent++;
        }//photons j-loop
      }//photons i-loop

      n_photonpairs_inevent_h -> Fill(n_photonpairs_inevent);
      n_pi0_post3sig_h        -> Fill(n_pi0_post3sig);
      n_FD_onlyhits_h         -> Fill(n_FD_onlyhits);
      n_FT_onlyhits_h         -> Fill(n_FT_onlyhits);
      n_FD_doublehits_h       -> Fill(n_FD_doublehits);

      n_events++;

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

    //ProofOut_File = new TProofOutputFile(Out_File_Name, "L");
    //Out_File = ProofOut_File->OpenFile("RECREATE");
    TFile Out_File(".outbuff/" + Out_File_Name, "RECREATE");

	  TListIter *iter = (TListIter*)GetOutputList()->MakeIterator();
	  for (TObject *obj = (*iter)(); obj != 0; obj = iter->Next()) {
		  obj->Write();
	  }

    Out_File.Close();
    cout << "In Terminate: " << Out_File.GetName() << " closed and we're done." << endl << endl;

  }

  // void SetLorentzVector(TLorentzVector &p4,clas12::region_part_ptr rp){
  // p4.SetXYZM(rp->par()->getPx(),rp->par()->getPy(),
	//      rp->par()->getPz(),p4.M());
    
  // }

  void pi0selector::counterhistos()
  {
    n_photons_inevent_h     =  new TH1F("n_photons_inevent_h", "Number of photons in one event; N; counts", 15,0,15);
    n_photonpairs_inevent_h =  new TH1F("n_photonpairs_inevent_h", "Number of photon-pairs in one event; N; counts", 100,0,100);
    n_pi0_post3sig_h        =  new TH1F("n_pi0_post3sig_h", "Number of #pi_{0} candidates post-3#sigma mass-cut; N; counts", 15,0,15);
    n_FD_onlyhits_h         =  new TH1F("n_FD_onlyhits_h", "FD-only photon-pairs; N; counts", 100,0,100);
    n_FT_onlyhits_h         =  new TH1F("n_FT_onlyhits_h", "FT-only photon-pairs; N; counts", 20,0,20);
    n_FD_doublehits_h       =  new TH1F("n_FD_doublehits_h", "Photon-pairs with one #gamma in FD/FT; N; counts", 20,0,20);
    photonE_h               =  new TH1F("photonE_h", "Photon Energy; E_{#gamma} (GeV); counts", 300, 0, 10);

    // n_photons_inevent_h     ->SetDirectory(Out_File);
    // n_photonpairs_inevent_h ->SetDirectory(Out_File);
    // n_pi0_post3sig_h        ->SetDirectory(Out_File);
    // n_FD_onlyhits_h         ->SetDirectory(Out_File);
    // n_FT_onlyhits_h         ->SetDirectory(Out_File);
    // n_FD_doublehits_h       ->SetDirectory(Out_File);
    // photonE_h               ->SetDirectory(Out_File);
  
    GetOutputList()->Add(n_photons_inevent_h);
    GetOutputList()->Add(n_photonpairs_inevent_h);
    GetOutputList()->Add(n_pi0_post3sig_h);
    GetOutputList()->Add(n_FD_onlyhits_h);
    GetOutputList()->Add(n_FT_onlyhits_h);
    GetOutputList()->Add(n_FD_doublehits_h);
    GetOutputList()->Add(photonE_h);
  }//counterhistos fxn

  void pi0selector::calc_angles(TVector3 Ebeam_vect, TVector3 Electron_vect, TVector3 Recoil_vect, TVector3 Newpart_vect)
  {

    TVector3 Virtual_photon; // virtual photon vector
    TVector3 vect_ee;		     // vector in the leptonic plane
    TVector3 vect_Nnew;		   // three vectors in the hadronic plane
    TVector3 vect_Nvg;
    TVector3 vect_vgnew;

    Virtual_photon = Ebeam_vect - Electron_vect; // vector of the virtual photon

    // Leptonic plane:
    vect_ee = Ebeam_vect.Cross(Electron_vect); // cross product of e and e', gives normal to leptonic plane

    // Hadronic plane:
    vect_Nnew = Recoil_vect.Cross(Newpart_vect);	 // normal to hadronic plane formed by recoiling nucleon and new particle (photon, meson)
    vect_Nvg = Recoil_vect.Cross(Virtual_photon);	 // normal to hadronic plane formed by recoiling nucleon and virtual photon
    vect_vgnew = Virtual_photon.Cross(Newpart_vect); // gives normal to hadronic plane formed by virtual photon and new particle (photon, meson)

    // Phi angles (between leptonic and hadronic planes):
    // Phi convention here:
    //- e from bottom left to center, e' from center to top left (so vect_ee is up out of page)
    //- gamma* from center left to center right
    //- N' from center to bottom right, new particle (gamma or pi0) from center to top right (so vect_Nnew, vect_Nvg, vect_vgnew all up out of page)
    //
    //- phi is 0 when both leptopnic and hadronic planes are parallel
    //- phi increases as you rotate hadronic plane clockwise, if looking along trajectory of virtual photon
    //- phi is 180 deg when the two planes are again parallel but the N' and the new particle (photon or meson) have swapped directions
    //- phi continues to increase to 360 deg as you continue to rotate hadronic plane clockwise, from point of view of gamma*

    phi_Nvg = (vect_ee.Angle(vect_Nvg)) * DEG; // angle between (e,e') and (N,gamma*) planes, in degrees.
    if ((vect_ee.Dot(Recoil_vect)) > 0.)
      phi_Nvg = 360. - phi_Nvg; // To make sure it runs from 0 - 360 degrees.

    phi_Nnew = (vect_ee.Angle(vect_Nnew)) * DEG; // angle between (e,e') and (N,gamma) or (N,pi0) planes, in degrees.
    if ((vect_ee.Dot(Newpart_vect)) < 0.)
      phi_Nnew = 360. - phi_Nnew; // Note: the pi0 or gamma are called "New" for simplicity.

    phi_vgnew = (vect_ee.Angle(vect_vgnew)) * DEG; // angle between (e,e') and (gamma*,gamma) or (gamma*,pi0) planes, in degrees.
    if ((vect_ee.Dot(Newpart_vect)) < 0.)
      phi_vgnew = 360. - phi_vgnew; // Note: the pi0 or gamma are called "New" for simplicity.

    // Coplanarity angles (to make sure the recoil nucleon, the virtual photon and the photon/meson are in the same plane):
    // In the naming convention, "new" refers to produced gamma or pi0.

    cop_Nvg_vgnew = (vect_Nvg.Angle(vect_vgnew)) * DEG; // angle between (N,gamma*) and (gamma*,gamma) or (gamma*,pi0) planes, in degrees.
    if ((vect_Nvg.Dot(Newpart_vect)) < 0)
      cop_Nvg_vgnew = -1 * cop_Nvg_vgnew; // sort of arbitrary, but makes sure there's a nice peak at zero

    cop_Nvg_Nnew = (vect_Nvg.Angle(vect_Nnew)) * DEG; // angle between (N,gamma*) and (N,gamma) or (N,pi0) planes, in degrees.
    if ((vect_Nvg.Dot(Newpart_vect)) < 0)
      cop_Nvg_Nnew = -1 * cop_Nvg_Nnew; // sort of arbitrary, but makes sure there's a nice peak at zero

    cop_Nnew_vgnew = (vect_Nnew.Angle(vect_vgnew)) * DEG; // angle between (N,gamma) and (gamma*,gamma) or between (N,pi0) and (gamma*,pi0) planes.
    if ((vect_vgnew.Dot(Recoil_vect)) < 0)
      cop_Nnew_vgnew = -1 * cop_Nnew_vgnew; // sort of arbitrary, but makes sure there's a nice peak at zero

  } //calc_angles fxn

  void pi0selector::photonflags(clas12::region_part_ptr p1, clas12::region_part_ptr p2)
  {
    using namespace clas12;
    //first photon:
    if (p1->ft(FTCAL)->getDetector() == 10)
      flag_photon1_FT = 1;
    else
      flag_photon1_FT = 0;

    if (p1->cal(PCAL)->getDetector() == 7)
      flag_photon1_PCAL = 1;
    else
      flag_photon1_PCAL = 0;

    if (p1->cal(ECIN)->getDetector() == 7)
      flag_photon1_EIN = 1;
    else
      flag_photon1_EIN = 0;
    if (p1->cal(ECOUT)->getDetector() == 7)
      flag_photon1_EOUT = 1;
    else
      flag_photon1_EOUT = 0;
    if (flag_photon1_EIN == 1 || flag_photon1_EOUT == 1)
      flag_photon1_ECAL = 1;
    else
      flag_photon1_ECAL = 0;

    if (flag_photon1_PCAL == 1 && flag_photon1_ECAL == 0)
      flag_photon1_onlyPCAL = 1;
    else
      flag_photon1_onlyPCAL = 0;

    if (flag_photon1_PCAL == 0 && flag_photon1_ECAL == 1)
      flag_photon1_onlyECAL = 1;
    else
      flag_photon1_onlyECAL = 0;	


    //second photon:
    if (p2->ft(FTCAL)->getDetector() == 10)
      flag_photon2_FT = 1;
    else
      flag_photon2_FT = 0;

    if (p2->cal(PCAL)->getDetector() == 7)
      flag_photon2_PCAL = 1;
    else
      flag_photon2_PCAL = 0;

    if (p2->cal(ECIN)->getDetector() == 7)
      flag_photon2_EIN = 1;
    else
      flag_photon2_EIN = 0;
    if (p2->cal(ECOUT)->getDetector() == 7)
      flag_photon2_EOUT = 1;
    else
      flag_photon2_EOUT = 0;
    if (flag_photon2_EIN == 1 || flag_photon2_EOUT == 1)
      flag_photon2_ECAL = 1;
    else
      flag_photon2_ECAL = 0;

    if ((flag_photon1_PCAL == 1) || (flag_photon1_ECAL == 1 ))
      flag_photon1_FD = 1;
    else 
      flag_photon1_FD = 0;

    if ((flag_photon2_PCAL == 1) || (flag_photon2_ECAL == 1 ))
      flag_photon2_FD = 1;
    else 
      flag_photon2_FD = 0;

    if (flag_photon2_PCAL == 1 && flag_photon2_ECAL == 0)
      flag_photon2_onlyPCAL = 1;
    else
      flag_photon2_onlyPCAL = 0;

    if (flag_photon2_PCAL == 0 && flag_photon2_ECAL == 1)
      flag_photon2_onlyECAL = 1;
    else
      flag_photon2_onlyECAL = 0;		
  } //photonflags fxn


  void pi0selector::SetOutFileName(TString Out_Name){
    Out_File_Name = Out_Name;
  }
} 
