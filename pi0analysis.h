#ifndef pi0analysis_h
#define pi0analysis_h

/*  Functions  */
void histos();

/*  Counters  */
int n_events;

/*  Histograms   */
TH1F *Q2_h[3];
TH1F *W_h[3];
TH1F *xB_h[3];
TH1F *tneg_h[3];
TH2F *Q2xB_h[3];

TH1F *pi0im_h[3];
TH1F *pi0mm2_h[3];
TH1F *pi0mp_h[3];
TH2F *pi0im_pi0mm2_h[3];
TH2F *pi0im_pi0mp_h[3];
TH2F *pi0mm2_pi0mp_h[3];

TH1F *recprotmm_h[3];
TH1F *spectneutmm_h[3];
TH1F *spectneutmp_h[3];
TH2F *spectneut_mpmm_h[3];

#endif
