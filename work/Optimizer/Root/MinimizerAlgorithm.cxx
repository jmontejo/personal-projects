#include "Optimizer/MinimizerAlgorithm.h"
#include <iostream>

MinimizerAlgorithm::MinimizerAlgorithm(std::vector<TString> options, VariablePool *vars):Algorithm(vars){
	name=options.at(0)+"_"+options.at(1);
	minimizer = ROOT::Math::Factory::CreateMinimizer(options.at(0).Data(),options.at(1).Data());
	minimizer->SetFunction(*functor);
	int ncalls = options.at(2).Atoi();
	minimizer->SetMaxFunctionCalls(ncalls); // for Minuit/Minuit2 
  minimizer->SetMaxIterations(ncalls);  // for GSL 
  minimizer->SetTolerance(0.001);
  minimizer->SetPrintLevel(1);
	usePrevBest = (options.size()>=4 && options.at(3)=="usePrevBest");
}

void MinimizerAlgorithm::Execute(){

  setStartingPoint(usePrevBest);
	timer.Start();
	minimizer->Minimize();
	double time = timer.CpuTime();
	perfgraph->SetPoint(perfgraph->GetN(),time,best_point.metric);
	timer.Stop();
	Round(best_point);
	minimizer->PrintResults();
	PrintBestPoint();

}
