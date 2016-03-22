#include "Optimizer/DampedScanAlgorithm.h"
#include "RooStats/NumberCountingUtils.h"
#include "TROOT.h"
#include <iostream>

DampedScanAlgorithm::DampedScanAlgorithm(std::vector<TString> options, VariablePool *vars):Algorithm(vars){
	name=options.at(0)+"_"+options.at(1);
	dampfactor = options.at(2).Atof();
	usePrevBest = (options.size()>=4 && options.at(3)=="usePrevBest");
	optpoint = OptimizationPoint(std::vector<double>(vars->GetN()), 9999, -1., -1.);
}

void DampedScanAlgorithm::Execute(){

	setStartingPoint(usePrevBest);
	timer.Start();
	TTree *sigtree = vars->getSigTree();
	TTree *bkgtree = vars->getBkgTree();
	bool hasimproved = true;
	TH1F *hsig, *hbkg;
	Double_t sig_integral, bkg_integral, bkg_effev;
	float metric, best_metric=9999;
	int best_var, best_var_bin;
	while(hasimproved){
		hasimproved = false;
		TString evcut;
		for(unsigned int j=0;j<vars->GetN(); j++){
			evcut    = "1";
			auto varj = vars->variables.at(j);
			for(unsigned int i=0;i<vars->GetN(); i++){
				if(i==j) continue;
				evcut += Form("*(%s > %g)",vars->variables.at(i).name.Data(),optpoint.cuts.at(i));
			}
			evcut += "*"+weight;
			TString signame = Form("hsig%d",j);
			TString bkgname = Form("hbkg%d",j);
			TString sigbins = signame+Form("(%d,%f,%f)",varj.i_getMax(),varj.f_getMinFi(),varj.f_getMinFi()+varj.step*varj.i_getMax());
			TString bkgbins = bkgname+Form("(%d,%f,%f)",varj.i_getMax(),varj.f_getMinFi(),varj.f_getMinFi()+varj.step*varj.i_getMax());
			std::cout << sigbins << std::endl;
			sigtree->Draw(vars->variables.at(j).name+" >>"+sigbins,evcut,"goff");
			hsig = (TH1F *) gDirectory->Get(signame);
			hsig->Scale(lumi*sigweight);
			Cumulative(hsig);
			bkgtree->Draw(vars->variables.at(j).name+" >>"+bkgbins,evcut,"goff");
			hbkg = (TH1F *) gDirectory->Get(bkgname);
			hbkg->Scale(lumi*bkgweight);
			Cumulative(hbkg);
			int best_bin = BestBin(hsig,hbkg,metric);
			sig_integral = hsig->GetBinContent(best_bin);
			bkg_integral = hbkg->GetBinContent(best_bin);
			bkg_effev = pow(hbkg->GetBinContent(best_bin)/hbkg->GetBinError(best_bin),2);
			hasimproved = (hasimproved || checkMetric(-metric,sig_integral, bkg_integral, bkg_effev, optpoint.cuts));
			std::cout << evcut << std::endl;
			std::cout << varj.name << " " << metric << " " << hasimproved << std::endl;
			if (metric < best_metric){
				best_metric = metric;
				best_var = j;
				best_var_bin = best_bin;
			}
		}
		std::cout << "best: " << best_var << " "<< best_var_bin << std::endl;
		int current_bin = vars->variables.at(best_var).f2i(optpoint.cuts.at(best_var));
		int new_bin = current_bin + (best_var_bin - current_bin)*dampfactor;
		if(new_bin == current_bin) new_bin += (best_var_bin-current_bin)/abs(best_var_bin-current_bin);

		std::cout << "Best var: " << vars->variables.at(best_var).name << std::endl;
		std::cout << "Cut was : " << optpoint.cuts.at(best_var) << " "<< current_bin << " " << vars->variables.at(best_var).i2f(current_bin) << std::endl;
		std::cout << "Optimal : " << vars->variables.at(best_var).i2f(best_var_bin) << " " << best_var_bin << std::endl;
		std::cout << "Is now  : " << vars->variables.at(best_var).i2f(new_bin) << " " << new_bin << std::endl;
		optpoint.cuts.at(best_var) = vars->variables.at(best_var).i2f(new_bin);

	}

	double time = timer.CpuTime();
	perfgraph->SetPoint(perfgraph->GetN(),time,best_point.metric);
	timer.Stop();
	Round(best_point);
	PrintBestPoint();

}

void DampedScanAlgorithm::setStartingPoint(bool usePrevBest){
	std::cout << "DampedScanAlgorithm::setStartingPoint" <<std::endl;
	for(unsigned int i=0;i<vars->GetN(); i++){
		auto var = vars->variables.at(i);
		if (usePrevBest)
			optpoint.cuts.at(i) = std::max<double>(var.f_getMin(),best_point.cuts.at(i));
		else
			optpoint.cuts.at(i) = var.f_getStart();
	}
}

int DampedScanAlgorithm::BestBin(TH1F *hsig,TH1F *hbkg, float &best_metric){

	TH1F *expz = (TH1F *) hsig->Clone();
	expz->Clear();
	
	float bkg_integral, sig_integral, bkg_effev, metric;
	best_metric = 10000;
	int best_bin;
	for(int i=0;i<hsig->GetNbinsX();i++){
		bkg_integral = hbkg->GetBinContent(i+1);
		sig_integral = hsig->GetBinContent(i+1);
		if(bkg_integral == 0)
			bkg_effev = 1;
		else
			bkg_effev = pow(hbkg->GetBinContent(i+1)/hbkg->GetBinError(i+1),2);
		if(bkg_integral>0)
			metric = -	(1+RooStats::NumberCountingUtils::BinomialExpZ(sig_integral, bkg_integral, sqrt(bkgsyst*bkgsyst+1./bkg_effev)))*DampMC(bkg_effev);
		else
			metric = 9999;
		expz->SetBinContent(i+1,std::max(-metric,float(0)));
		if(metric<best_metric){
			best_metric = metric;
			best_bin = i+1;
		}
	}
	TCanvas c2;
	hsig->SetLineColor(2);
	hsig->Draw();
	hbkg->Draw("same");
	expz->Draw("same,h");
	c2.SaveAs(TString(hsig->GetName())+".png");
	return best_bin;
}

void DampedScanAlgorithm::Cumulative(TH1F *h){
	for(int i=h->GetNbinsX()-1;i>1;i--){
		h->SetBinContent(i,h->GetBinContent(i)+h->GetBinContent(i+1));
		h->SetBinError(i,sqrt(pow(h->GetBinError(i),2)+pow(h->GetBinError(i+1),2)));
	}

}
