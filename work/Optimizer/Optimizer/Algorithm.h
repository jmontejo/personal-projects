#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "TString.h"
#include "Optimizer/OptimizationPoint.h"
#include "Optimizer/VariablePool.h"
#include "TMinuit.h"
#include "TStopwatch.h"
#include "Optimizer/Result.h"
#include <vector>
#include <map>
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"


class Algorithm {

  public:
    static Algorithm* GetAlgorithm(TString name, TString options, VariablePool *vars);
		virtual void Execute()=0;
		VariablePool *vars;
		TString name;
		void (*theFcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t);
		void SetParameters(TString weight, float lumi, float bkgsys, float bkgweight=1., float sigweight=1.);
		float BinomialExpZ(const Double_t *par);
		ROOT::Math::IMultiGenFunction *functor;
		TGraph* getGraph(){ return perfgraph; };
		TGraph* getGraphRounded(){ return perfgraph_rounded; };
		void PrintBestPoint();

  protected:
    Algorithm(VariablePool *vars);
		bool checkMetric(float val, float sig, float bkg, float bkg_effev, const Double_t *par);
		bool checkMetric(float val, float sig, float bkg, float bkg_effev, std::vector<double> par);
		void setStartingPoint(bool usePrevBest);

		void SoverB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
		void SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
		void getSB(const Double_t *par, Double_t *sig_integral, Double_t *bkg_integral, Double_t *bkg_effev=NULL);
		float DampMC(float bkg_effev);


		float max_value, max_sig, max_bkg;
		std::vector<double> max_pars;
		float bkgsyst, lumi, sigweight, bkgweight;
		TString weight;
		bool debug;
		ROOT::Math::Minimizer* minimizer;
		int N;
		std::vector<OptimizationPoint> opt_points;
		static OptimizationPoint best_point, best_point_rounded;
		TGraph *perfgraph, *perfgraph_rounded;
		TStopwatch timer;
		bool usePrevBest, rounding_on;
		void Round(const OptimizationPoint &p);
		std::vector<bool> BestOffset(const OptimizationPoint &p, std::vector<bool> &offsets, int index, float &metric);

};

#endif
