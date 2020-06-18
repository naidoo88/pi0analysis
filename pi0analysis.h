#ifndef pi0analysis_h
#define pi0analysis_h

/*  Constants  */
double DEG;

/*  Functions  */
void photonflags(clas12::region_part_ptr p1, clas12::region_part_ptr p2, int &count_ECAL_doublehits);
void calc_angles(TVector3 Ebeam_vect, TVector3 Electron_vect, TVector3 Recoil_vect, TVector3 Newpart_vect);

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

bool flag_photon1_FD;
bool flag_photon1_FT;
bool flag_photon1_PCAL;
bool flag_photon1_ECAL;
bool flag_photon1_EIN;
bool flag_photon1_EOUT;
bool flag_photon2_FD;
bool flag_photon2_FT;
bool flag_photon2_PCAL;
bool flag_photon2_ECAL;
bool flag_photon2_EIN;
bool flag_photon2_EOUT;


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

#endif
