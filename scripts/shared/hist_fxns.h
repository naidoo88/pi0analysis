#ifndef FXNS_H
#define FXNS_H

#include <vector>
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"

using std::vector;
using TH1Array1D = vector<TH1F>;
using TH1Array2D = vector<TH1Array1D>;
using TH2Array1D = vector<TH2F>;
using TH2Array2D = vector<TH2Array1D>;
using TH1Array1D_Ptr = vector<TH1F*>;
using TH1Array2D_Ptr = vector<TH1Array1D_Ptr>;

class hist_fxns {
  public:

  hist_fxns()=default;
  ~hist_fxns()=default;

  TH1Array1D createTH1Array1D(TString, float[3], TString, vector<TString>, vector<TString>);
  TH1Array2D createTH1Array2D(TString, float[3], TString, vector<TString>, vector<TString>, vector<TString>, vector<TString>);
  TH2Array1D createTH2Array1D(TString, float[6], TString, vector<TString>, vector<TString>);
  TH2Array2D createTH2Array2D(TString, float[6], TString, vector<TString>, vector<TString>, vector<TString>, vector<TString>);

  TH1Array1D_Ptr createTH1PtrArray1D(TString, float[3], TString, vector<TString>, vector<TString>);
  TH1Array2D_Ptr createTH1PtrArray2D(TString, float[3], TString, vector<TString>, vector<TString>, vector<TString>, vector<TString>);
  
  void writeHistos(TH1Array1D &);
  void writeHistos(TH1Array2D &);
  void writeHistos(TH1Array1D_Ptr);
  void writeHistos(TH1Array2D_Ptr);

};
#endif