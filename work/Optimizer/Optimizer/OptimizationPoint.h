#ifndef OPTIMIZATIONPOINT_H
#define OPTIMIZATIONPOINT_H

#include "TString.h"
#include <vector>
#include <map>

class OptimizationPoint{

  public:
    OptimizationPoint(std::vector<float> cuts, float metric);

  private:
		std::vector<float> cuts;
		float metric;

};

#endif
