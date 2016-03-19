#include "Optimizer/Variable.h"
#include <iostream>
//#include <math>

Variable::Variable(TString name, TH1F *hsig, TH1F *hbkg, float step, int total){
	this->name = name;
	this->hsig = hsig;
	this->hbkg = hbkg;

	for(int i=0;i<hsig->GetNbinsX();i++)
		if(hsig->GetBinContent(i)!=0){
			fmin = hsig->GetBinLowEdge(i);
			break;
		}
	fmax = hbkg->GetBinLowEdge(hbkg->GetNbinsX()+1);
	for(int i=0;i<hbkg->GetNbinsX();i++)
		if(hbkg->Integral(i,-1)/hbkg->Integral(0,-1)<0.05){
			fmax = hbkg->GetBinLowEdge(i);
			break;
		}
	fmean = hbkg->GetMean();
	start = (fmean+(total-1)*fmin)/total;

	float range = fmax-fmin;
	this->step = getStep(range,step);
	imax = ceil(range/this->step);
}

float Variable::getStep(float range, float step){
	float div = pow(10,floor(log10(range)));
	float mostsignif = range/div;
	if      (mostsignif<2) mostsignif = 2;
	else if (mostsignif<5) mostsignif = 5;
	else 									 mostsignif = 10;
	float newstep = mostsignif*div/100;
	if (step < newstep)
		step = newstep;
	return step;

}

int Variable::i_getMax()		{ return imax; }
int Variable::i_getStart()	{ return f2i(start); }
int Variable::f2i(float f)	{ return floor(std::min(std::max(f-fmin,float(0.)),fmax)/step); }
float Variable::f_getMax()	{ return fmax; }
float Variable::f_getMin()	{ return fmin; }
float Variable::f_getStart(){ return start; }
float Variable::i2f(int i)	{	return std::min(fmin+i*step,fmax); }

void Variable::Print(){
    float modulo = fmax>1000 ? 1000.: 1.;
    TString gev  = fmax>1000 ? " 1e3": "    ";
    std::cout << std::setw(22) << std::setfill(' ') << name << gev;
    std::cout << std::setw(9) << std::setfill(' ') << std::setprecision(3) << fmin/modulo;
    std::cout << std::setw(9) << std::setfill(' ') << std::setprecision(3) << fmax/modulo;
    std::cout << std::setw(9) << std::setfill(' ') << std::setprecision(3) << step/modulo;
    std::cout << std::setw(9) << std::setfill(' ') << std::setprecision(3) << imax;
    std::cout << std::endl;
}
