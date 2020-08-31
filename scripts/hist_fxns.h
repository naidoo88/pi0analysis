#ifndef FXNS_H
#define FXNS_H

#include <vector>
#include "TH1.h"
#include "TH2F.h"
#include "TString.h"

using std::vector;
using TH1Array1D = vector<TH1F>;
using TH1Array2D = vector<TH1Array1D>;
// using HistArray1D_Ptr = vector<TH1F*>;
// using HistArray2D_Ptr = vector<HistArray1D_Ptr*>:

class hist_fxns {
  public:

  hist_fxns()=default;
  ~hist_fxns()=default;

  TH1Array1D createTH1Array1D(TString, float[3], TString, vector<TString>, vector<TString>);
  TH1Array2D createTH1Array2D(TString, float[3], TString, vector<TString>, vector<TString>, vector<TString>, vector<TString>);
  // HistArray1D_Ptr createHistArray1D(TString, float[3], TString, vector<TString>, vector<TString>);
  // HistArray2D_Ptr createHistArray2D(TString, float[3], TString, vector<TString>, vector<TString>, vector<TString>, vector<TString>);
  void writeHistos(TH1Array1D &);
  void writeHistos(TH1Array2D &);

};
#endif