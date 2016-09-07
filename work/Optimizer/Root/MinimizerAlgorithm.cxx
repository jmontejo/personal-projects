#include "Optimizer/MinimizerAlgorithm.h"
#include "Math/IOptions.h"
#include <iostream>

MinimizerAlgorithm::MinimizerAlgorithm(std::vector<TString> options, VariablePool *vars):Algorithm(vars){
	name=options.at(0)+"_"+options.at(1);
	minimizer = ROOT::Math::Factory::CreateMinimizer(options.at(0).Data(),options.at(1).Data());
//  if (options.at(0)=="Genetic"){
//		ROOT::Math::IOptions & geneticOpt = ROOT::Math::MinimizerOptions::Default("Genetic");
//   	geneticOpt.SetValue("PopSize",600);
//   geneticOpt.SetValue("Steps", 60);
//   geneticOpt.SetValue("Cycles", 10);
//   geneticOpt.SetValue("SC_steps", SC_steps);
//   geneticOpt.SetValue("SC_rate", SC_rate);
//   geneticOpt.SetValue("SC_factor", SC_factor);
//   geneticOpt.SetValue("ConvCrit", ConvCrit);
//  }

	minimizer->SetFunction(*functor);
	int ncalls = options.at(2).Atoi();
	minimizer->SetMaxFunctionCalls(ncalls); // for Minuit/Minuit2 
  minimizer->SetMaxIterations(ncalls);  // for GSL 
  minimizer->SetTolerance(0.001);
  minimizer->SetPrintLevel(1);
	usePrevBest = (options.size()>=4 && options.at(3)=="usePrevBest");
	round = (options.size()>=5 && options.at(4)=="round");
}

void MinimizerAlgorithm::Execute(){

  setStartingPoint(usePrevBest);
	timer.Start();
	minimizer->Minimize();
	double time = timer.CpuTime();
	perfgraph->SetPoint(perfgraph->GetN(),time,best_point.metric);
	timer.Stop();
	if(round)
	Round(best_point);
	minimizer->PrintResults();
	PrintBestPoint();

}
