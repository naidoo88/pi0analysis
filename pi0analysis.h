#ifndef pi0analysis_h
#define pi0analysis_h

/*  Functions  */
void histos();

/*  Counters  */
int n_events;

/*  Histograms   */
TH1F *pi0im_pre_h;
TH1F *pi0im_h;
TH1F *pi0mm2_pre_h;
TH1F *pi0mm2_h;
TH1F *pi0mp_pre_h;
TH1F *pi0mp_h;
TH1F *tneg_pre_h;
TH1F *tneg_h;
TH1F *Q2_pre_h;
TH1F *Q2_h;
TH1F *W_pre_h;
TH1F *W_h;
TH1F *xB_pre_h;
TH1F *xB_h;

TH1F *recprotmm_pre_h;
TH1F *recprotmm_h;

TH2F *pi0im_pi0mm2_pre_h;
TH2F *pi0im_pi0mm2_h;
TH2F *pi0im_pi0mp_pre_h;
TH2F *pi0im_pi0mp_h;
TH2F *pi0mm2_pi0mp_pre_h;
TH2F *pi0mm2_pi0mp_h;

TH2F *Q2xB_pre_h;
TH2F *Q2xB_h;

#endif
