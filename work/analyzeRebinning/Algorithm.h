#include "TH1D.h"
#include "TString.h"
#include "TColor.h"

class Algorithm{

  public:
    Algorithm(TString name, TH1D * (*function)(TH1D *, TH1D *),int color, int smooth=0, bool usebinwidth=false);
    TH1D *apply (TH1D *h1, TH1D *h2);
    TH1D * (*function)(TH1D *, TH1D *);
    int color;
    int smooth;
    TString name;
    float score[5];
    float totalscore[5];
    bool usebinwidth;
};
