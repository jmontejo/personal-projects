#include "Optimizer/OptimizationPoint.h"
#include <iostream>

VariablePool *OptimizationPoint::vars;
OptimizationPoint::OptimizationPoint(){}

OptimizationPoint::OptimizationPoint(Double_t *cuts, int n, float metric){
	std::vector<double> vcuts(n);
	for(int i=0;i<n;i++)
		vcuts.at(i) = cuts[i];
	this->cuts = vcuts;
	this->metric = metric;
	this->sig = -1.;
	this->bkg = -1.;
}

OptimizationPoint::OptimizationPoint(std::vector<double> cuts, float metric, float sig, float bkg){
	this->cuts = cuts;
	this->metric = metric;
	this->sig = sig;
	this->bkg = bkg;
}

void OptimizationPoint::SetVariablePool(VariablePool *vars_){
	vars = vars_;
}

