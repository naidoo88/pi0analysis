#ifndef plothistos_h
#define plothistos_h

/*  Functions  */
void histos();

/*  Histograms   */
TH1F *Q2_h[3];
TH1F *W_h[3];
TH1F *xB_h[3];
TH1F *tneg_h[3];
TH2F *Q2xB_h[3];

TH1F *pi0im_h[3];
TH1F *pi0mm2_h[3];
TH1F *pi0mp_h[3];
TH2F *pi0_immm2_h[3];
TH2F *pi0_immp_h[3];
TH2F *pi0_mm2mp_h[3];

TH1F *recprotmm_h[3];
TH1F *spectneutmm_h[3];
TH1F *spectneutmp_h[3];
TH2F *spectneut_mpmm_h[3];

#endif
