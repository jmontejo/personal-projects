#include "Optimizer/Algorithm.h"
#include "Optimizer/MinimizerAlgorithm.h"
#include "Optimizer/DampedScanAlgorithm.h"
#include "RooStats/NumberCountingUtils.h"
#include "TObjArray.h"
#include "TROOT.h"
#include <vector>
#include <iostream>
#include <algorithm>

OptimizationPoint Algorithm::best_point;
OptimizationPoint Algorithm::best_point_rounded;

Algorithm::Algorithm(VariablePool *vars){
	debug = false;
	this->vars = vars;
	N = vars->GetN();
	functor =  new ROOT::Math::Functor(this,&Algorithm::BinomialExpZ,N); 
	perfgraph = new TGraph();
	perfgraph_rounded = new TGraph();
	max_value = 0;
	rounding_on = false;
}

Algorithm* Algorithm::GetAlgorithm(TString name, TString options, VariablePool *vars){

	auto tokens = options.Tokenize(":");
	std::vector<TString> voptions;
	for(int i=0; i<tokens->GetEntries(); i++){
		TString token = ((TObjString*)tokens->At(i))->String();
		if (token==".") token = "";
		voptions.push_back(token);
	}
	if(name=="Minimizer")
		return new MinimizerAlgorithm(voptions,vars);
	else if(name=="DampedScan")
		return new DampedScanAlgorithm(voptions,vars);
	else{
		std::cout << "Algorithm not defined: " << name <<std::endl;
		exit(1);
	}
}

float Algorithm::DampMC(float bkg_effev){
	return erf(bkg_effev-100)*0.5 + 0.5; //Veto less than 100 MC events
}

bool Algorithm::checkMetric(float val, float sig, float bkg, float bkg_effev, const Double_t *par){
	//std::cout << "checking metric: " << val << " " << max_value <<std::endl;
	if (val <= max_value) return false;
	std::vector<double> max_pars(par, par + sizeof(Double_t)*N);
	return checkMetric(val,sig,bkg,bkg_effev,max_pars);
}
bool Algorithm::checkMetric(float val, float sig, float bkg, float bkg_effev, std::vector<double> max_pars){
	if (val <= max_value) return false;
	max_value = val;
	max_sig = sig;
	max_bkg = bkg;
	OptimizationPoint opt(max_pars, val, sig, bkg);
	opt_points.push_back(opt);
	if (val > best_point.metric)
		best_point = opt;
	double time = timer.CpuTime();
	timer.Continue();
	perfgraph->SetPoint(perfgraph->GetN(),time,val);
	std::cout << perfgraph->GetN() << " " << time << " "<<val <<std::endl;
	float realZ = RooStats::NumberCountingUtils::BinomialExpZ(sig, bkg, bkgsyst);
	std::cout << "Improvement: 1+BinomialZ with MC stat and damping: (sig/bkg) " << sig<<"/"<<bkg<< " => " << val << std::endl;
	std::cout << "Improvement: real BinomialZ: " << realZ << std::endl;
	std::cout << "Improvement: effective events: " << bkg_effev << std::endl;
	return true;
}

// --- Figures of merit
void Algorithm::SoverB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

	Double_t sig_integral, bkg_integral;
	getSB(par,&sig_integral,&bkg_integral);
	f = -sig_integral/(bkg_integral+0.00001); 
}

void Algorithm::SoverSqrtB(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

	Double_t sig_integral, bkg_integral;
	getSB(par,&sig_integral,&bkg_integral);
	f = -sig_integral/sqrt(bkg_integral+pow(bkg_integral*bkgsyst,2)+1);

}

float Algorithm::BinomialExpZ(const Double_t *par){
	float f = 1;
	Double_t sig_integral, bkg_integral, bkg_effev;
	getSB(par,&sig_integral,&bkg_integral,&bkg_effev);
	if(bkg_integral>0)
		f = -	(1+RooStats::NumberCountingUtils::BinomialExpZ(sig_integral, bkg_integral, sqrt(bkgsyst*bkgsyst+1./bkg_effev)))*DampMC(bkg_effev);
	else
		f = 9999;
	if(!rounding_on)
		checkMetric(-f,sig_integral, bkg_integral, bkg_effev, par);
	return f;

}

// --- Get integrals
void Algorithm::getSB(const Double_t *par, Double_t *sig_integral, Double_t *bkg_integral , Double_t *bkg_effev){

	TString evcut    = "1";
	for(unsigned int i=0;i<vars->GetN(); i++){
		auto var = vars->variables.at(i);
		evcut += Form("*(%s > %g)",var.name.Data(),par[i]);
	}
	evcut += "*"+weight;
	if(debug)
		std::cout << evcut << std::endl;

	TTree *sigtree = vars->getSigTree();
	TTree *bkgtree = vars->getBkgTree();

	TString signame = "hsig";
	signame = signame.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","");
	sigtree->Draw("1 >>"+signame,evcut,"goff");
	TH1F *hsig = (TH1F *) gDirectory->Get(signame);
	*sig_integral = lumi*sigweight*hsig->Integral();
	TString bkgname = "hbkg";
	bkgname = bkgname.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","");
	bkgtree->Draw("1 >>"+bkgname,evcut,"goff");
	TH1F *hbkg = (TH1F *) gDirectory->Get(bkgname);
	*bkg_integral = lumi*bkgweight*hbkg->Integral(); //FIXME IntegralAndError

	if(bkg_effev)
		*bkg_effev = hbkg->GetEffectiveEntries();
	if(*bkg_integral == 0){
		*bkg_effev = 1;
	}
	delete hbkg;
	delete hsig;

}

// --- Set starting values and step sizes for parameters
void Algorithm::setStartingPoint(bool usePrevBest){
	for(unsigned int i=0;i<vars->GetN(); i++){
		auto var = vars->variables.at(i);
		if(debug)
			std::cout << Form("%d %s %f %f %f %f",i , var.name.Data(), var.f_getStart(), ( var.f_getStart()-var.f_getMin())/1.01, var.f_getMinFi(), var.f_getMax()) <<std::endl;
		if (usePrevBest)
			minimizer->SetLimitedVariable(i , var.name.Data(), std::max<double>(var.f_getMin(),best_point.cuts.at(i)), std::max<double>(fabs( best_point.cuts.at(i)-var.f_getMin())/1.01,1e-3), var.f_getMinFi(), var.f_getMax());
		else
			minimizer->SetLimitedVariable(i , var.name.Data(), var.f_getMin(), ( var.f_getStart()-var.f_getMin())/1.01, var.f_getMinFi(), var.f_getMax());

	}
}

void Algorithm::SetParameters(TString weight, float lumi, float bkgsyst, float bkgweight, float sigweight){
	this->weight		= weight;	
	this->lumi 			= lumi;	
	this->bkgsyst 	= bkgsyst;	
	this->bkgweight = bkgweight;	
	this->sigweight = sigweight;	
}

void Algorithm::PrintBestPoint(){
	std::cout << "Results ---------- : " <<name << std::endl;
	std::cout << Form("metric/sig/bkg     : %f %f %f",best_point.metric,best_point.sig,best_point.bkg) <<std::endl;
	for(int i=0; i<N; i++)
		std::cout << Form("%s\t\t%f",vars->variables.at(i).name.Data(),best_point.cuts.at(i)) <<std::endl;
	std::cout << "Rounded ---------- : " <<name << std::endl;
	std::cout << Form("metric/sig/bkg     : %f %f %f",best_point_rounded.metric,best_point_rounded.sig,best_point_rounded.bkg) <<std::endl;
	for(int i=0; i<N; i++)
		std::cout << Form("%s\t\t%f",vars->variables.at(i).name.Data(),best_point_rounded.cuts.at(i)) <<std::endl;
}

void Algorithm::Round(const OptimizationPoint &p){
	rounding_on = true;
	std::cout << "Start Round" <<std::endl;
	float best_rounded = 0;
	float metric;
	int nTemp = (int)pow(2, vars->GetN());
	bool offset;
	Double_t pars[vars->GetN()];
	for(int i=0;i<nTemp;i++){
		for (int k = 0; k < vars->GetN(); k++){
			offset = ((i >> k) & 0x1);
			int vari = vars->variables.at(k).f2i(p.cuts.at(k),offset);
			float varf = vars->variables.at(k).i2f(vari);
			pars[k] = varf;
		}
		metric = -BinomialExpZ(pars);
		if(metric > best_rounded){
			best_rounded = metric;
			best_point_rounded = OptimizationPoint(pars,vars->GetN(),metric);
		}
	}
	double time = timer.CpuTime();
	timer.Continue();
	perfgraph_rounded->SetPoint(perfgraph_rounded->GetN(),time,best_rounded);
	perfgraph_rounded->SetPoint(perfgraph_rounded->GetN(),time,best_rounded);
	std::cout << "Rounded: " << best_point_rounded.metric << std::endl;
	if (best_point_rounded.metric > best_point.metric){
		best_point = best_point_rounded;
		perfgraph->SetPoint(perfgraph->GetN(),time,best_point_rounded.metric);
	}
	rounding_on = false;
	std::cout << "End Round" <<std::endl;
}

std::vector<bool> Algorithm::BestOffset(const OptimizationPoint &p, std::vector<bool> &offsets0, int index, float &metric){
	if(index < vars->GetN()){
		float metric0, metric1;
		std::vector<bool> offsets1 = offsets0;
		offsets1.at(index) = true;
		offsets0 = BestOffset(p, offsets0, index+1, metric0);
		offsets1 = BestOffset(p, offsets1, index+1, metric1);
		if(metric0>=metric1){
			metric = metric0;
			return offsets0;
		}
		metric = metric1;
		return offsets1;
	}
	const int N =vars->GetN();
	Double_t pars[N];
	for(int i=0;i<vars->GetN();i++){
		int vari = vars->variables.at(i).f2i(p.cuts.at(i),offsets0.at(i));
		float varf = vars->variables.at(i).i2f(vari);
		pars[i] = varf;
	}
	metric = -BinomialExpZ(pars);
	return offsets0;

}
