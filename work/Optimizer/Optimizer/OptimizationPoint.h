#ifndef OPTIMIZATIONPOINT_H
#define OPTIMIZATIONPOINT_H

#include <vector>
#include "Optimizer/VariablePool.h"

class OptimizationPoint{

  public:
    OptimizationPoint();
    OptimizationPoint(std::vector<double> cuts, float metric, float sig, float bkg);
    OptimizationPoint(Double_t *pars, int n, float metric);
		std::vector<double> cuts;
		float metric, sig, bkg;
		static void SetVariablePool(VariablePool *vars);
	private:
		static VariablePool *vars;
		std::vector<bool> BestOffset(std::vector<bool> offsets, int index, float &metric);

};

#endif
