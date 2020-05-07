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
bool flag_dis_cuts;
bool flag_excl_cuts;
//bool flag_MM2_total;
//bool flag_spectneutmp;

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
int n_excl_events;
int n_postcut_events;
int n_ECAL_doublehits;
int n_photons_in_event;

double Q2;
double tneg;
double W;
double xB;

double pi0coneangle;
double IM_g1g2;
double MM2_total;
double MM2_total;

double MM_rec_recoil;
double MM_rec_recoil;
double MP_rec_spectator;
double MM_rec_spectator;

#endif
