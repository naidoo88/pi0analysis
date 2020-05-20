#ifndef pi0analysis_h
#define pi0analysis_h

/*  Constants  */
double DEG;

/*  Functions  */
void photonflags(clas12::region_part_ptr p1, clas12::region_part_ptr p2, int &count_ECAL_doublehits);
void calc_angles(TVector3 Ebeam_vect, TVector3 Electron_vect, TVector3 Recoil_vect, TVector3 Newpart_vect);

//angle variables for calc_angles function
double phi_Nvg;
double phi_Nnew;
double phi_vgnew;
double cop_Nvg_vgnew;
double cop_Nvg_Nnew;
double cop_Nnew_vgnew;

/*  Flags  */
bool flag_goodpi0;
bool flag_cuts_dis;
bool flag_cuts_excl;
bool flag_cuts_broadMM2;
bool flag_cuts_broadcone;
bool flag_cuts_spectMP; 
bool flag_cuts_3sigMM2;
bool flag_cuts_cop;
//bool flag_MM2_total;


bool flag_photon1_ft;
bool flag_photon1_PCAL;
bool flag_photon1_ECAL;
bool flag_photon1_EIN;
bool flag_photon1_EOUT;
int  flag_photon1_wCAL;//kill when stable

bool flag_photon2_ft;
bool flag_photon2_PCAL;
bool flag_photon2_ECAL;
bool flag_photon2_EIN;
bool flag_photon2_EOUT;
int  flag_photon2_wCAL;//kill when stable

/*  Counters  */
int n_events;
int n_dis_events;
int n_excl_events;
int n_goodpi0_candidates;
int n_postcut_events;
int n_ECAL_doublehits;
int n_photons_in_event;

int helicity;

double Q2;
double tneg;
double W2;
double xB;

double IM_g1g2;
double MM2_total;
double MP_total;
double pi0coneangle;

double MM_rec_recoil;
double MP_rec_recoil;
double MP_rec_spectator;
double MM_rec_spectator;

#endif
