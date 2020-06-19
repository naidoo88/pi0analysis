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
#include "pi0analysis.h"

using namespace clas12;
using std::cout;

void pi0analysis(const Char_t in_list[], const TString outfilename){

	TStopwatch time;
	time.Start();

	bool DEBUG = 0;
	cout << "Start" << endl;
	TFile *Out_File = new TFile(outfilename, "recreate"); 
	std::ifstream list_of_files;
	char file_name[200];
	char last_file[200];

	DEG = TMath::RadToDeg(); //rad->deg conversion

	list_of_files.open(in_list);
	cout << "list of files opened" << endl;

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

	n_files           = 0;
	n_events          = 0;
	n_dis_events      = 0;
	n_excl_events     = 0;

	//##############################################################################
	// Set up Tree/Branches
	//##############################################################################

	TTree *data = new TTree("data", "Processed Data.");
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
	//##############################################################################

	counterhistos();

	if (list_of_files.is_open()){
		cout << "Successfully opened list:  " << in_list << endl;

		while (!list_of_files.eof()){
			if (!list_of_files.good()){
				cout << "Error: Issue with list." << endl;
				break;
			}

			list_of_files >> file_name;

			if (strcmp(last_file, file_name) != 0){ // makes sure the last file doesn't get counted twice.

				cout << "File to be read in is: " << file_name << endl;

				std::string file_str(file_name); //string for clas12reader
				clas12reader c12(file_str);		 //read hipo file

				c12.addExactPid(2212, 1); //one proton
				c12.addExactPid(11, 1);	  //one electron
				c12.addAtLeastPid(22, 2); //at least 2 photons
				c12.addZeroOfRestPid();	  //no other particles

				auto db = TDatabasePDG::Instance();
				TLorentzVector beam  (0, 0, 10.6, 10.6);
				TLorentzVector target(0, 0, 0, db->GetParticle(2212)->Mass());

				TLorentzVector q          (0, 0, 0, 0);
				TLorentzVector deut       (0, 0, 0, 1.876);

				TLorentzVector e_scattered(0, 0, 0, 0);
				TLorentzVector recoil     (0, 0, 0, 0);
				TLorentzVector phot1      (0, 0, 0, 0);
				TLorentzVector phot2      (0, 0, 0, 0);

				double em    = db->GetParticle(11)->Mass();
				double protm = db->GetParticle(2212)->Mass();
				double pi0m  = db->GetParticle(111)->Mass();
				double neutm = db->GetParticle(2112)->Mass();
				//double deutm = db -> GetParticle(1000010020) -> Mass();

				while (c12.next()){ //loop over events
					// if (n_events%1000==0)
					// {
					//   cout << "\r" << "Processing event: " << std::setw(10) <<  n_events << std::flush;
					// }

					auto electronbuff = c12.getByID(11);
					auto photonbuff = c12.getByID(22);
					auto protonbuff = c12.getByID(2212);

					recoil.SetXYZM(protonbuff[0]->par()->getPx(),
								   protonbuff[0]->par()->getPy(),
								   protonbuff[0]->par()->getPz(),
								   protm);

					e_scattered.SetXYZM(electronbuff[0]->par()->getPx(),
										electronbuff[0]->par()->getPy(),
										electronbuff[0]->par()->getPz(),
										em);

					q = beam - e_scattered;

					helicity = c12.helonline()->getHelicity();

					TLorentzVector system;        //[e N -> e' R g1 g2]
					TLorentzVector photon_pair;
					TLorentzVector expected_pi0;

					TLorentzVector rec_recoil;	  //[e p -> e' g1 g2 X]
					TLorentzVector rec_spectator; //[e D -> e' R g1 g2 X]

					n_photonpairs_inevent = 0;
					n_pi0_post3sig        = 0;
					n_pi0_goodcandidates  = 0;
					n_pi0_post3sig        = 0;
					n_FD_onlyhits         = 0;
					n_FT_onlyhits         = 0;
					n_FD_doublehits       = 0;

					n_photons_inevent     = photonbuff.size();
					n_photons_inevent_h  -> Fill(n_photons_inevent);

					for (int i = 0; i < n_photons_inevent - 1; i++){
						for (int j = i + 1; j < n_photons_inevent; j++){
							if (DEBUG)	cout << "pair [" << i << "][" << j << "]" << endl;

							phot1.SetXYZM(photonbuff[i]->par()->getPx(),
										  photonbuff[i]->par()->getPy(),
										  photonbuff[i]->par()->getPz(),
										  0);
							phot2.SetXYZM(photonbuff[j]->par()->getPx(),
										  photonbuff[j]->par()->getPy(),
										  photonbuff[j]->par()->getPz(),
										  0);
							
							photon_pair = phot1 + phot2;
							IM_g1g2   = photon_pair.M();

							system    = (beam + target) - (e_scattered + recoil + photon_pair); //[e p -> e' p' g1 g2]
							MM2_total = system.M2();
							MP_total  = system.P();
							ME_total  = system.E();

							expected_pi0 = target + beam - e_scattered - recoil;
							pi0coneangle = (expected_pi0.Angle(photon_pair.Vect())) * DEG;

							rec_recoil        = (beam + target) - (e_scattered + photon_pair); //reconstruct recoil [e p -> e' g1 g2 X]
							MP_rec_recoil     = rec_recoil.P();
							MM_rec_recoil     = rec_recoil.M();
							MM2_rec_recoil    = rec_recoil.M2();
 
							rec_spectator     = (beam + deut) - (e_scattered + recoil + photon_pair); //reconstruct spectator [e D -> e' p' g1 g2 X]
							MP_rec_spectator  = rec_spectator.P();
							MM_rec_spectator  = rec_spectator.M();
							MM2_rec_spectator = rec_spectator.M2();

							//Calc trento-phi and co-planarity angles:
							calc_angles(beam.Vect(), e_scattered.Vect(), recoil.Vect(), photon_pair.Vect());

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
							xB   = Q2 / 2 * (target * q);
							tneg = -1 * (recoil - target).M2(); //**DOUBLE CHECK THIS AGAINST THEORY PAPER**

							if ((Q2 > 1) && (tneg < 1)){
								flag_cuts_dis = 1;
								n_dis_events++;
							}
							else
								flag_cuts_dis = 0;

							W2 = (recoil + photon_pair).M2();

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
						} //photons j-loop
					}//photons i-loop
					n_photonpairs_inevent_h -> Fill(n_photonpairs_inevent);
					n_pi0_post3sig_h        -> Fill(n_pi0_post3sig);
					n_FD_onlyhits_h         -> Fill(n_FD_onlyhits);
					n_FT_onlyhits_h         -> Fill(n_FT_onlyhits);
					n_FD_doublehits_h       -> Fill(n_FD_doublehits);
					if (DEBUG)
						cout << "LOOPED THROUGH " << n_photonpairs_inevent << " PAIRS.\n" << endl;
				//SUBSTITUTE THESE LINES TO PROCESS N-EVENTS
				//if (n_events == 10) break;        }//event loop
				n_events++;
				}//event loop
			n_files++;
			}//duplicate check
			sprintf(last_file, "%s", file_name); // save name of the current file into "last_file"
		}//file list loop
	}//list open-check

	list_of_files.close();
	cout << "\nTotal no of files in list: " << n_files << endl;
	cout << "\nTotal no of events processed: " << n_events << endl;

	Out_File->Write();
	Out_File->Close();

	time.Stop();
	time.Print();
} //pi0analysis fxn

void photonflags(clas12::region_part_ptr p1, clas12::region_part_ptr p2)
{
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

} //photonflags fxn

void calc_angles(TVector3 Ebeam_vect, TVector3 Electron_vect, TVector3 Recoil_vect, TVector3 Newpart_vect)
{

	TVector3 Virtual_photon; // virtual photon vector
	TVector3 vect_ee;		 // vector in the leptonic plane
	TVector3 vect_Nnew;		 // three vectors in the hadronic plane
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

void counterhistos()
{
	n_photons_inevent_h     =  new TH1F("n_photons_inevent_h", "Number of photons in one event; N; counts",                  15,0,15);
	n_photonpairs_inevent_h =  new TH1F("n_photonpairs_inevent_h", "Number of photon-pairs in one event; N; counts",         100,0,100);
	n_pi0_post3sig_h        =  new TH1F("n_pi0_post3sig_h", "Number of #pi_{0} candidates post-3#sigma mass-cut; N; counts", 15,0,15);

	n_FD_onlyhits_h         =  new TH1F("n_FD_onlyhits_h", "FD-only photon-pairs; N; counts",                    100,0,100);
	n_FT_onlyhits_h         =  new TH1F("n_FT_onlyhits_h", "FT-only photon-pairs; N; counts",                    20,0,20);
	n_FD_doublehits_h       =  new TH1F("n_FD_doublehits_h", "Photon-pairs with one #gamma in FD/FT; N; counts", 20,0,20);
}//counterhistos fxn
