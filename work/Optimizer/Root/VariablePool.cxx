#include "TFile.h"
#include "TLeaf.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "Optimizer/ComputeVar.h"
#include "Optimizer/VariablePool.h"
#include <iostream>
#include <iomanip>
#include <set>
#include <utility>

#define DEBUG 1

void Test_f2i(VariablePool *vp){

	for(unsigned int i=0;i<vp->variables.size();i++){
		float vmin  = vp->variables.at(i).f_getMin();
		float vmax  = vp->variables.at(i).f_getMax();
		float range = vmax - vmin;
		TString name = vp->variables.at(i).name;
		for(unsigned int j=1;j<=100;j++){
			float point = vmin + j*range/100.;
			std::cout << Form("%d\t%s\t%f\t%f\t%f",j,name.Data(),point,vp->variables.at(i).i2f(vp->variables.at(i).f2i(point)),vp->variables.at(i).i2f(vp->variables.at(i).f2i(point,true))) << std::endl;
		}
	}
	vp->Print();
	exit(1);
};

VariablePool::VariablePool(TTree *sigtree, TChain *bkgtree, Options *options){
	m_cut     	= options->get("cut");
	m_weight  	= options->get("weight");
	m_vars    	= options->getVars();
	m_vars_set  = std::set<TString>(m_vars.begin(),m_vars.end());
	m_vars_step = options->getVarsSteps();
	m_noCheck 	= (options->get("noCheck")=="true");
	m_doPlots   = (options->get("doPlots")=="true");
	const char *envVarContent = getenv("ROOTCOREBIN");
	plotfolder 	= TString(envVarContent)+"/../Optimizer/plots";
	TString commandString = "mkdir -p -m 755 "+plotfolder;
	if(system(commandString.Data()) != 0) std::cout << "^[[31m" << commandString << " failed^[[0m" << std::endl;

	std::cout << "Skimming signal tree..." << std::endl;
	dummy = TFile::Open(plotfolder+"/dummy_"+options->get("tag")+".root","recreate");
	m_sigtree = sigtree->CopyTree(m_cut);
	std::cout << "Skimming bkg tree (can take long)..." << std::endl;
	m_bkgtree = bkgtree->CopyTree(m_cut);
	std::cout << "Cut reduced signal tree    : " <<sigtree->GetEntries() << " -> " << m_sigtree->GetEntries() << std::endl;
	std::cout << "Cut reduced background tree: " <<bkgtree->GetEntries() << " -> " << m_bkgtree->GetEntries() << std::endl;
	std::cout << "Building VariablePool..." << std::endl;

	for(unsigned int i=0;i<m_vars.size();i++){
		TString varname = m_vars.at(i);
		float 	step		= m_vars_step.at(i);
		AddVar(varname,step);
	}
	if(m_doPlots)
		ComputeCorrelations();
	std::cout << "End VariablePool" << std::endl;
	if(m_doPlots){
		std::cout << "Plots done, exiting" <<std::endl;
		exit(0);
	}
	//Test_f2i(this);
}

VariablePool::~VariablePool(){
	dummy->Clear();
}

// --- Add vars
void VariablePool::AddVar(TString varname, float step){

	if(DEBUG) std::cout << "AddVar " << varname << std::endl;

	TString hsigname = "hsig"+varname;
	hsigname = hsigname.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","").ReplaceAll(",","");
	m_sigtree->Draw(varname+">>"+hsigname,m_cut+"*"+m_weight);
	TH1F *hsig = (TH1F *) gDirectory->Get(hsigname);

	TString hbkgname = "hbkg"+varname;
	hbkgname = hbkgname.ReplaceAll(":","").ReplaceAll("(","").ReplaceAll(")","").ReplaceAll(",","");
	TH1F *hbkg = (TH1F *) hsig->Clone(hbkgname);
	m_bkgtree->Draw(varname+">>"+hbkgname,m_cut+"*"+m_weight);

	hsig->Scale(1./hsig->Integral());
	hbkg->Scale(1./hbkg->Integral());

	Variable var(varname,hsig,hbkg,step);
	variables.push_back(var);
	if(m_doPlots)
		doPlot(varname,hsig,hbkg);

	delete hsig;
	delete hbkg;
}


// --- Print content
void VariablePool::Print(){

	std::cout << "--- Variable Pool ---" << std::endl;
	for(unsigned int i=0;i<GetN(); i++){
		variables.at(i).Print();
	}

}

void VariablePool::doPlot(TString var,TH1F *hsig, TH1F *hbkg){
	c1.cd();

	hsig->SetLineColor(2);
	hsig->SetLineWidth(2);
	hbkg->SetLineWidth(2);
	int rebinN = getRebinN(hsig);
	hsig->Rebin(rebinN);
	hbkg->Rebin(rebinN);
	hbkg = (TH1F *) hbkg->DrawNormalized();
	hsig = (TH1F *) hsig->DrawNormalized("same");

	c1.SetLogy(0);
	c1.SaveAs(plotfolder+"/"+var.ReplaceAll("[","").ReplaceAll("]","").ReplaceAll("$","")+".png");
	c1.SetLogy(1);
	c1.SaveAs(plotfolder+"/"+var.ReplaceAll("[","").ReplaceAll("]","").ReplaceAll("$","")+"_logscale.png");

	delete hbkg;
	delete hsig;
}

// --- Get trees
TTree* VariablePool::getSigTree(){ return m_sigtree; }
TTree* VariablePool::getBkgTree(){ return m_bkgtree; }

int VariablePool::getRebinN(TH1F *h){

	TH1F *copy = (TH1F *) h->Clone("copy");
	bool empty = emptyMiddleBins(copy);
	int rb = 1;
	if (empty && copy->GetNbinsX() >= 50 && copy->GetNbinsX()%5==0){
		copy->Rebin(5);
		rb *= 5;
	}
	while (emptyMiddleBins(copy)){
		if(copy->GetNbinsX() <= 10) break;
		if(copy->GetNbinsX()%2==0){copy->Rebin(2); rb*=2;}
		else if(copy->GetNbinsX()%5==0){copy->Rebin(5); rb*=5;}
		else break;
	}

	return rb; 
}

bool VariablePool::emptyMiddleBins(TH1F *h){
	int start=1, end=h->GetNbinsX();
	for(int i=1; i<h->GetNbinsX();i++)
		if(h->GetBinContent(i) > 0){ start=i; break;}
	for(int i=h->GetNbinsX(); i>0;i--)
		if(h->GetBinContent(i) > 0){ end=i; break;}
	for(int i=start; i<end;i++)
		if(h->GetBinContent(i) == 0) return true;
	return false;
}

std::vector<float> VariablePool::GetVarMin(){
	return var_min;
}
std::vector<float> VariablePool::GetLimMin(){
	return lim_min;
}
std::vector<float> VariablePool::GetVarMax(){
	return var_max;
}
std::vector<float> VariablePool::GetLimMax(){
	return lim_max;
}
std::vector<float> VariablePool::GetVarStart(){
	return var_start;
}
std::vector<TString> VariablePool::GetVarName(){
	return m_vars;
}
std::vector<std::pair<float, int> > VariablePool::GetVarStep(){
	return var_step;
}

void VariablePool::ComputeCorrelations(){
	int n = variables.size();
	float corr_sig;
	float corr_bkg;
	TH2F *matrix_sig = new TH2F("matrix_sig","matrix_sig",n,-0.5,n-0.5,n,-0.5,n-0.5);
	TH2F *matrix_bkg = new TH2F("matrix_bkg","matrix_bkg",n,-0.5,n-0.5,n,-0.5,n-0.5);
	for(int i=0;i<n;i++)
		for(int j=i;j<n;j++){
			if(i==j){
				corr_sig = 1;
				corr_bkg = 1;
			}
			else{
				m_sigtree->Draw(variables.at(i).name+":"+variables.at(j).name+Form(">> hsig2D%d%d",i,j),m_cut+"*"+m_weight);
				TH2F *hsig = (TH2F *) gDirectory->Get(Form("hsig2D%d%d",i,j));
				corr_sig = hsig->GetCorrelationFactor();
				m_bkgtree->Draw(variables.at(i).name+":"+variables.at(j).name+Form(">> hbkg2D%d%d",i,j),m_cut+"*"+m_weight);
				TH2F *hbkg = (TH2F *) gDirectory->Get(Form("hbkg2D%d%d",i,j));
				corr_bkg = hbkg->GetCorrelationFactor();
			}
			matrix_sig->SetBinContent(i+1,j+1,corr_sig);
			matrix_bkg->SetBinContent(i+1,j+1,corr_bkg);
			if(fabs(corr_sig-corr_bkg)>0.1)
				std::cout << "Exploitable correlation: " << (variables.at(i).name+":"+variables.at(j).name) << Form(" %f:%f\t%f",corr_sig,corr_bkg,fabs(corr_sig-corr_bkg)) <<std::endl;
			if(i!=j && (fabs(corr_sig)>0.6 || fabs(corr_bkg)>0.6))
				std::cout << "Highly correlated vars: " << (variables.at(i).name+":"+variables.at(j).name) << Form(" %f:%f",corr_sig,corr_bkg) <<std::endl;
	}
	if(m_doPlots){
		c1.cd();
		c1.SetLogy(0);
		matrix_sig->Draw("colz");
		c1.SaveAs(plotfolder+"/corrmatrix_sig.png");
		matrix_bkg->Draw("colz");
		c1.SaveAs(plotfolder+"/corrmatrix_bkg.png");
	}

}
