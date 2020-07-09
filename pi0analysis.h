#ifndef pi0analysis_h
#define pi0analysis_h

/*  Constants  */
double DEG;

/*  Functions  */
void photonflags(clas12::region_part_ptr p1, clas12::region_part_ptr p2);
void calc_angles(TVector3 Ebeam_vect, TVector3 Electron_vect, TVector3 Recoil_vect, TVector3 Newpart_vect);
void counterhistos();

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

/*  Flags  */
bool flag_2photon_event;

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

TH1F *n_photons_inevent_h;
TH1F *n_photonpairs_inevent_h;
TH1F *n_pi0_post3sig_h;
TH1F *n_FD_onlyhits_h;
TH1F *n_FT_onlyhits_h;
TH1F *n_FD_doublehits_h;
TH1F *photonE_h;
#endif
