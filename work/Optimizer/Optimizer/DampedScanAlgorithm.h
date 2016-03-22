#ifndef DAMPEDSCAN_ALGORITHM_H
#define DAMPEDSCAN_ALGORITHM_H

#include "TString.h"
#include "Optimizer/Algorithm.h"
#include <vector>
#include <map>

class DampedScanAlgorithm: public Algorithm{

	public:
    DampedScanAlgorithm(std::vector<TString> options, VariablePool *vars);
  	void Execute();
	protected:
		OptimizationPoint optpoint;
		void setStartingPoint(bool usePrevBest);
		float dampfactor;
		int BestBin(TH1F *hsig,TH1F *hbkg, float &best_metric);
		void Cumulative(TH1F *h);

};

#endif
