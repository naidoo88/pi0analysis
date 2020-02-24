#ifndef pi0analysis_h
#define pi0analysis_h

/*  Functions  */
void photonflags(clas12::region_part_ptr p1, clas12::region_part_ptr p2, int &count_ECAL_doublehits);

/*  Flags  */
bool flag_excl;
bool flag_pi0thetadiff;
bool flag_pi0mm2;
bool flag_spectneutmp;

bool flag_photon1_ft;
bool flag_photon1_PCAL;
bool flag_photon1_ECAL;
bool flag_photon1_EIN;
bool flag_photon1_EOUT;
int  flag_photon1_wCAL;

bool flag_photon2_ft;
bool flag_photon2_PCAL;
bool flag_photon2_ECAL;
bool flag_photon2_EIN;
bool flag_photon2_EOUT;
int  flag_photon2_wCAL;

/*  Counters  */
int n_events;
int n_excl_events;
int n_postcut_events;
int n_ECAL_doublehits;

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
