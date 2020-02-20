#ifndef pi0analysis_h
#define pi0analysis_h

/*  Functions  */

/*  Counters  */
int n_events;
int n_excl_events;
int n_postcut_events;
int n_FTreg_flags;
int n_FTCAL_flags;

/* Branch Variables */
bool flag_excl;
bool flag_pi0thetadiff;
bool flag_pi0mm2;
bool flag_spectneutmp;
bool flag_photon1_FT;
bool flag_photon1_ft;
bool flag_photon1_PCAL;
bool flag_photon1_ECAL;
int  flag_photon1_wCAL;
bool flag_photon2_FT;
bool flag_photon2_ft;
bool flag_photon2_PCAL;
bool flag_photon2_ECAL;
int  flag_photon2_wCAL;

double Q2;
double tneg;
double W;
double xB;

double pi0thetadiff;
double pi0coneangle;
double pi0im;
double pi0mm2;
double pi0mp;

double recprotmm;
double recprotmp;
double specneutmp;
double specneutmm;

#endif
