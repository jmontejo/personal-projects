#ifndef Variable_H
#define Variable_H

#include "TH1F.h"

class Variable{

public:
  Variable(TString name, TH1F *hsig, TH1F *hbkg, float step=-1., int total=10);
	TString name;
	int 	i_getMax();
//	int 	i_getMean();
	int 	i_getStart();
	int 	f2i(float f, bool ceil=false);
	float f_getMin();
	float f_getMinFi();
	float f_getMax();
//	float f_getMean();
	float f_getStart();
	float i2f(int i);
	void Print();
	float step;

private:
	TH1F *hsig, *hbkg;
	float fmin, fmax, fmean, start, fimin;
	int imax;
	float getStep(float range, float step);

};
#endif
