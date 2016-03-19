#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "TString.h"
//#include "Optimizer/OptimizationPoint.h"
#include "Optimizer/VariablePool.h"
#include "TMinuit.h"
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
		void SetBkgSyst(double s);
		TString name;
		void (*theFcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t);
		void SetParameters(TString weight, float lumi, float bkgsys, float bkgweight=1., float sigweight=1.);
		float BinomialExpZ(const Double_t *par);
		float BinomialExpZ_forMinimizer(const Double_t *par);
		static void BinomialExpZ_forMinuit(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
		Double_t operator()(const Double_t * pars);
		ROOT::Math::IMultiGenFunction *functor;

  protected:
    Algorithm(VariablePool *vars);
		//void SetFunction(	void(* fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t));
//		std::vector<OptimizationPoint> bestPoints;
		void checkMetric(float val, float sig, float bkg, std::vector<double> max_pars);
		void setStartingPoint();

		void SoverB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
		void SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
		void getSB(const Double_t *par, Double_t *sig_integral, Double_t *bkg_integral, Double_t *bkg_effev=NULL, Double_t *bkg_ev=NULL);
		float DampMC(float bkg_effev);


		float max_value, max_sig, max_bkg;
		std::vector<double> max_pars;
		//Double_t sig_integral, bkg_integral, bkg_effev, bkg_ev;
		float bkgsyst, lumi, sigweight, bkgweight;
		TString weight;
		bool debug;
		TMinuit *gMinuit;
		ROOT::Math::Minimizer* minimizer;
		int N;
		Result result;
		Result tmp_result;

};

#endif
