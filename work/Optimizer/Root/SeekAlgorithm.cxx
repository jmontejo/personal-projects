#include "Optimizer/SeekAlgorithm.h"
#include <iostream>

SeekAlgorithm::SeekAlgorithm(std::vector<TString> options, VariablePool *vars):Algorithm(vars){
	name="Seek";
}

void SeekAlgorithm::Execute(){

	minimizer->Minimize();
	minimizer->PrintResults();

}
