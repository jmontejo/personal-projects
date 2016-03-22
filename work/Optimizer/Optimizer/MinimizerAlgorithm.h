#ifndef SEEK_ALGORITHM_H
#define SEEK_ALGORITHM_H

#include "TString.h"
#include "Optimizer/Algorithm.h"
#include <vector>
#include <map>

class MinimizerAlgorithm: public Algorithm{

	public:
    MinimizerAlgorithm(std::vector<TString> options, VariablePool *vars);
  	void Execute();

};

#endif
