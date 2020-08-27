#ifndef FXNS_H
#define FXNS_H

#include <vector>
#include "TH1.h"
#include "TH2F.h"
#include "TString.h"

using std::vector;
using std::cout;
using std::endl;
using HistArray1D = vector<TH1F>;
using HistArray2D = vector<HistArray1D>;

HistArray1D createHistArray1D(TString, float[3], TString, vector<TString>, vector<TString>);
HistArray2D createHistArray2D(TString, float[3], TString, vector<TString>, vector<TString>, vector<TString>, vector<TString>);
void writeHistos(HistArray1D &);
void writeHistos(HistArray2D &);

#endif