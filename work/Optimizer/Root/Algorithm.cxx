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
	round = false;
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
	return erf(bkg_effev/100.-1)*0.5 + 0.5; //Veto less than 100 MC events, starts to be notizable at 200 (x 0.92)
}

bool Algorithm::checkMetric(float val, float sig, float bkg, float bkg_effev, const Double_t *par){
	//std::cout << "checking metric: " << val << " " << max_value <<std::endl;
	if (val <= max_value) return false;
	std::vector<double> max_pars(par, par + sizeof(Double_t)*N);
	return checkMetric(val,sig,bkg,bkg_effev,max_pars,par);
}
bool Algorithm::checkMetric(float val, float sig, float bkg, float bkg_effev, std::vector<double> max_pars, const Double_t *par){
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
	float realZ = RooStats::NumberCountingUtils::BinomialExpZ(sig, bkg, sqrt(bkgsyst*bkgsyst+1./bkg_effev));
	float displayZ = realZ;
	if(displayweight!="" && par){
		Double_t dissig, disbkg, disbkg_effev;
		getSB(par,&dissig,&disbkg,&disbkg_effev,true);
		displayZ = RooStats::NumberCountingUtils::BinomialExpZ(dissig, disbkg, sqrt(bkgsyst*bkgsyst+1./bkg_effev));
	}
	std::cout << "Improvement: 1+BinomialZ with MC stat damping: (sig/bkg) " << sig<<"/"<<bkg<< " => " << val << std::endl;
	std::cout << "Improvement: real BinomialZ: " << realZ << std::endl;
	std::cout << "Improvement: display BinomialZ: " << displayZ << std::endl;
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

float Algorithm::BinomialExpZdisplay(const Double_t *par){
	float f = 1;
	Double_t sig_integral, bkg_integral, bkg_effev;
	getSB(par,&sig_integral,&bkg_integral,&bkg_effev,true);
	if(bkg_integral>0)
		f = RooStats::NumberCountingUtils::BinomialExpZ(sig_integral, bkg_integral, sqrt(bkgsyst*bkgsyst+1./bkg_effev));
	return f;

}

// --- Get integrals
void Algorithm::getSB(const Double_t *par, Double_t *sig_integral, Double_t *bkg_integral , Double_t *bkg_effev, bool do_displayweight){

	TString evcut    = "1";
	for(unsigned int i=0;i<vars->GetN(); i++){
		auto var = vars->variables.at(i);
		evcut += Form("*(%s > %g)",var.name.Data(),par[i]);
	}
	if (do_displayweight)
		evcut += "*"+displayweight;
	else
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

void Algorithm::SetParameters(TString weight, float lumi, float bkgsyst, float bkgweight, float sigweight, TString displayweight){
	this->weight		= weight;	
	this->lumi 			= lumi;	
	this->bkgsyst 	= bkgsyst;	
	this->bkgweight = bkgweight;	
	this->sigweight = sigweight;	
	this->displayweight		= displayweight;	
}

void Algorithm::PrintBestPoint(){
	FlagUselessVars(&best_point);
	std::cout << "Results ---------- : " <<name << std::endl;
	std::cout << Form("metric/sig/bkg     : %f %f %f",best_point.metric,best_point.sig,best_point.bkg) <<std::endl;
	TString cutchain = "";
	TString cutchain_skim = "";
	for(int i=0; i<N; i++){
		std::cout << Form("%s\t\t%f\t\t%f",vars->variables.at(i).name.Data(),best_point.cuts.at(i),best_point.fractionalImprovement.at(i)) <<std::endl;
		cutchain += Form("(%s > %f)*",vars->variables.at(i).name.Data(),best_point.cuts.at(i));
		if(best_point.fractionalImprovement.at(i) > 1.01)
			cutchain_skim += Form("(%s > %f)*",vars->variables.at(i).name.Data(),best_point.cuts.at(i));
	}
	std::cout << cutchain.Remove(cutchain.Length()-1) <<std::endl;
	std::cout << cutchain_skim.Remove(cutchain_skim.Length()-1) <<std::endl;
	cutchain = "";
	cutchain_skim = "";
	if(round){
		FlagUselessVars(&best_point_rounded);
		std::cout << "Rounded ---------- : " <<name << std::endl;
		std::cout << Form("metric/sig/bkg     : %f %f %f",best_point_rounded.metric,best_point_rounded.sig,best_point_rounded.bkg) <<std::endl;
		for(int i=0; i<N; i++){
			std::cout << Form("%s\t\t%f\t\t%f",vars->variables.at(i).name.Data(),best_point_rounded.cuts.at(i),best_point_rounded.fractionalImprovement.at(i)) <<std::endl;
			cutchain += Form("(%s > %f)*",vars->variables.at(i).name.Data(),best_point_rounded.cuts.at(i));
			if(best_point_rounded.fractionalImprovement.at(i) > 1.01)
				cutchain_skim += Form("(%s > %f)*",vars->variables.at(i).name.Data(),best_point_rounded.cuts.at(i));
		}
		std::cout << cutchain.Remove(cutchain.Length()-1) <<std::endl;
		std::cout << cutchain_skim.Remove(cutchain_skim.Length()-1) <<std::endl;
	}
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
		if (i==(nTemp/10)) std::cout << i << "/" << nTemp <<std::endl;
		for (int k = 0; k < vars->GetN(); k++){
			offset = ((i >> k) & 0x1);
			int vari = vars->variables.at(k).f2i(p.cuts.at(k),offset);
			float varf = vars->variables.at(k).i2f(vari);
			pars[k] = varf;
		}
		metric = -BinomialExpZ(pars);
		//metric = BinomialExpZdisplay(pars); //since we check against the non-display metric this doesn't make sense
		//unless the display metric is better
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

void Algorithm::FlagUselessVars(OptimizationPoint *p){
	std::cout << "Looking for useless variables" <<std::endl;
	float metric;
	float old_metric =  p->metric;
	Double_t pars[vars->GetN()];
	for (int k = 0; k < vars->GetN(); k++){
		pars[k] = p->cuts.at(k);
	}

	for (int k = 0; k < vars->GetN(); k++){
		float varmin = vars->variables.at(k).f_getMin();
		pars[k] = varmin;
		metric = -BinomialExpZ(pars); //This call can change p!!!
		p->fractionalImprovement.at(k) = p->metric/metric;
		pars[k] = p->cuts.at(k);
		std::cout << Form("%d\t%f\t%f\t%f\t%f",k,p->metric,metric,varmin,p->fractionalImprovement.at(k)) << std::endl;
	}
	std::cout << old_metric << " " << p->metric <<std::endl;
	if(old_metric !=p->metric)
		FlagUselessVars(p);
}
