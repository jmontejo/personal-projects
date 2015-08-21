#include "TLatex.h"
#include "TCanvas.h"
#include "TError.h"
#include "TFile.h"
#include "TColor.h"
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include "Algorithm.h"
#include "Functions.h"
#include "TROOT.h"
#include "TStyle.h"
#include <cmath>
//#include <assert>

using namespace std;
int getdir (TString dir, vector<TString> &files);
void doTheTest(TH1D *hnom, TH1D* hsys,const vector<Algorithm *> &v_algorithms, TString fname, float fig_weight = 0.5, bool doPlots=false);
float figMerit_binDiff(TH1D *original, TH1D *smoothed, float intratio);
float figMerit_binSmooth(TH1D *original, TH1D *smoothed);

TString inpath = "histos/";
//TString inpath = "/nfs/pic.es/user/j/jmontejo/AnaTools/MVA/FinalTreeMaker/FinalTreeMaker2012/limitfiles/mergedFiles_TRCR14_baseline/";
TString plotpath = "analyzeRebinning_plots/";

int main(){

  int debug = 1;
  gROOT->SetBatch(1);
  gErrorIgnoreLevel = 3000;
  gStyle->SetOptTitle(0);

  vector<Algorithm *> v_algorithms;
  //v_algorithms.push_back(new Algorithm("Original, not smoothed       ", &noSmooth, 2,0));
  //v_algorithms.push_back(new Algorithm("TH1D::Smooth(1)             ", &noSmooth, 2,1));
  //v_algorithms.push_back(new Algorithm("TH1D::Smooth(2)             ", &noSmooth, 3,2));
  //v_algorithms.push_back(new Algorithm("TH1D::Smooth(4)             ", &noSmooth, 4,4));
  //v_algorithms.push_back(new Algorithm("TH1D::Smooth(8)             ", &noSmooth, 5,8));
  //v_algorithms.push_back(new Algorithm("Flat                         ", &rebin_dBoverB_0, 2,0));
  //v_algorithms.push_back(new Algorithm("rebin 1%, Smooth 1          ", &rebin_dBoverB_1, 3,1));
  //v_algorithms.push_back(new Algorithm("rebin 1%, Smooth bin width 1 ", &rebin_dBoverB_1, 2,1,true));
  //v_algorithms.push_back(new Algorithm("rebin 1%, Smooth bin width 2 ", &rebin_dBoverB_1, 2,2,true));
  //v_algorithms.push_back(new Algorithm("rebin 1%, Smooth bin width 3 ", &rebin_dBoverB_1, 3,3,true));
  //v_algorithms.push_back(new Algorithm("rebin 2%, Smooth bin width 1 ", &rebin_dBoverB_2, 2,1,true));
  //v_algorithms.push_back(new Algorithm("rebin 2%, Smooth bin width 2 ", &rebin_dBoverB_2, 3,2,true));
  //v_algorithms.push_back(new Algorithm("rebin 2%, Smooth bin width 3 ", &rebin_dBoverB_2, 3,3,true));
  //v_algorithms.push_back(new Algorithm("rebin 4%, Smooth bin width 1 ", &rebin_dBoverB_4, 2,1,true));
  //v_algorithms.push_back(new Algorithm("rebin 4%, Smooth bin width 2 ", &rebin_dBoverB_4, 3,2,true));
  //v_algorithms.push_back(new Algorithm("rebin 4%, Smooth bin width 3 ", &rebin_dBoverB_4, 3,3,true));
  //v_algorithms.push_back(new Algorithm("rebin 5%, no smooth ", &rebin_dBoverB_5, 2,0));
  //v_algorithms.push_back(new Algorithm("rebin 5%, Smooth    ", &rebin_dBoverB_5, 3,1));
  //v_algorithms.push_back(new Algorithm("rebin 8%, Smooth bin width 1 ", &rebin_dBoverB_8, 3,1,true));
  //v_algorithms.push_back(new Algorithm("rebin 8%, Smooth 2           ", &rebin_dBoverB_8, 2,2));
  //v_algorithms.push_back(new Algorithm("rebin 8%, Smooth bin width 2 ", &rebin_dBoverB_8, 2,2,true));
  //v_algorithms.push_back(new Algorithm("rebin 2%, no smooth         ", &rebin_dBoverB_2, 2,0));
  //v_algorithms.push_back(new Algorithm("rebin 2%, Smooth 1          ", &rebin_dBoverB_2, 3,1));
  //v_algorithms.push_back(new Algorithm("rebin 2%, Smooth 2          ", &rebin_dBoverB_2, 3,2));
  v_algorithms.push_back(new Algorithm("rebin 8%, no smooth         ", &rebin_dBoverB_8, 2,0));
  //v_algorithms.push_back(new Algorithm("rebin 4%, Smooth 1          ", &rebin_dBoverB_4, 3,1));
  v_algorithms.push_back(new Algorithm("rebin 8%, Smooth           ", &rebin_dBoverB_8, 4,2));
  //v_algorithms.push_back(new Algorithm("rebin 8%, no smooth         ", &rebin_dBoverB_8, 2,0));
  //v_algorithms.push_back(new Algorithm("rebin 8%, Smooth 2          ", &rebin_dBoverB_8, 3,2));
  //v_algorithms.push_back(new Algorithm("max Variations 2, no smooth ", &rebin_maxVariations_2, 4,0));
  //v_algorithms.push_back(new Algorithm("max Variations 1, Smooth 2  ", &rebin_maxVariations_1, 3,2,true));
  //v_algorithms.push_back(new Algorithm("max Variations 2, Smooth 2  ", &rebin_maxVariations_2, 2,2));
  v_algorithms.push_back(new Algorithm("max Variations 3, Smooth ", &rebin_maxVariations_3, 6,2,true));
  //v_algorithms.push_back(new Algorithm("max Variations 2, Smooth 4  ", &rebin_maxVariations_2, 2,4));
  //v_algorithms.push_back(new Algorithm("max Variations 3, no smooth ", &rebin_maxVariations_3, 2,0));
  //v_algorithms.push_back(new Algorithm("max Variations 3, Smooth 2  ", &rebin_maxVariations_3, 2,2));
  //v_algorithms.push_back(new Algorithm("min Significant 2, no smooth", &rebin_minSignificant_2, 3,0));
  v_algorithms.push_back(new Algorithm("min Significant 1, Smooth ", &rebin_minSignificant_1, 3,2));
  //v_algorithms.push_back(new Algorithm("min Significant 4, Smooth 2 ", &rebin_minSignificant_4, 4,2,true));

  vector<int> scores(v_algorithms.size());
  vector<TString> files = vector<TString>();
  getdir(inpath,files);

  vector<float> meritWeight;
  meritWeight.push_back(0.5);

  vector<TString> systematics;
  systematics.push_back("BASELINE");
  systematics.push_back("BJES");
  systematics.push_back("CLOSEBY");
  systematics.push_back("ETA");
  systematics.push_back("FLAVOR_COMP");
  systematics.push_back("FLAVOR_RESP");
  systematics.push_back("PILEUP_OFFSETMU");
  systematics.push_back("PILEUP_OFFSETNPV");
  systematics.push_back("JER");
//  systematics.push_back("BTAGBREAK");
//  systematics.push_back("CTAGBREAK");
//  systematics.push_back("JVF");
//  systematics.push_back("LTAG");
//  systematics.push_back("KTFAC");
//  systematics.push_back("QFAC");
//  systematics.push_back("WJETS");
//  systematics.push_back("TTBARHF");
//  systematics.push_back("IQOPT");

  for(int s=0;s< systematics.size();s++){
    for(int m=0;m< meritWeight.size();m++){
      for(int i=0;i<files.size();i++){
        if (files.at(i).Contains("Data")) continue;
        if (!files.at(i).Contains("NOMINAL") || !files.at(i).Contains("ELEMUON")) continue;
        if (!files.at(i).Contains("ttbarAlpgen_HFOR-")) continue;
        if (!files.at(i).Contains("4jetex2btagex")) continue;
        TFile *nomfile = TFile::Open(inpath+files.at(i));
        TH1D *nomhist = (TH1D *) nomfile->Get("HTHad");
        TString base = files.at(i)(0,files.at(i).Index("NOMINAL"));
        for(int j=0;j<files.size();j++){
          if (!files.at(j).Contains(base) || files.at(j).Contains("NOMINAL") ) continue;
          if (!files.at(j).Contains(systematics.at(s))) continue;
          //if (!files.at(j).Contains("BTAGBREAK8")) continue;
          //if (files.at(j).Contains("DOWN")) continue;
          //std::cout << "Smoothing: " << files.at(j) << std::endl;
          TFile *sysfile = TFile::Open(inpath+files.at(j));
          TH1D *syshist = (TH1D *) sysfile->Get("HTHad");
          doTheTest(nomhist,syshist,v_algorithms, files.at(j), meritWeight.at(m),m==0);
          sysfile->Close();
        }
        nomfile->Close();
      }

      std::cout << " --- Evaluating algorithms ---: " << meritWeight.at(m) << " " << systematics.at(s) << std::endl;
      int   i1, i2, i3, i4;
      float min1=9e9, min2=9e9, min3=9e9, min4=9e9;
      for(int i=0; i<v_algorithms.size();i++){
        if(v_algorithms.at(i)->score[0] < min1){
          min1 = v_algorithms.at(i)->score[0];
          i1 = i;
        }
        if(v_algorithms.at(i)->score[1] < min2){
          min2 = v_algorithms.at(i)->score[1];
          i2 = i;
        }
        if(v_algorithms.at(i)->score[2] < min3){
          min3 = v_algorithms.at(i)->score[2];
          i3 = i;
        }
        if(v_algorithms.at(i)->score[3] < min4){
          min4 = v_algorithms.at(i)->score[3];
          i4 = i;
        }
      }
      for(int i=0; i<v_algorithms.size();i++){
        std::cout << Form("%s %.2f \t %.2f \t %.2f \t %.2f",v_algorithms.at(i)->name.Data(), v_algorithms.at(i)->score[0],v_algorithms.at(i)->score[1],v_algorithms.at(i)->score[2],v_algorithms.at(i)->score[3]) << std::flush;
        if (i==i1){
          std::cout << " - best 1" << std::flush;
//          scores.at(i) += 1;
        }
        if (i==i2){
          std::cout << " - best 2" << std::flush;
//          scores.at(i) += 1;
        }
        if (i==i3){
          std::cout << " - best 3" << std::flush;
//          scores.at(i) += 1;
        }
        if (i==i4){
          std::cout << " - best 4" << std::flush;
          scores.at(i) += 1;
        }
        std::cout << std::endl;
        v_algorithms.at(i)->score[0] = 0; 
        v_algorithms.at(i)->score[1] = 0; 
        v_algorithms.at(i)->score[2] = 0; 
        v_algorithms.at(i)->score[3] = 0; 
      }
    }
  }
  std::cout << " ---- Final scores ---- " << std::endl;
  int   i1, i2, i3, i4;
  float min1=9e9, min2=9e9, min3=9e9, min4=9e9;
  for(int i=0; i<v_algorithms.size();i++){
    if(v_algorithms.at(i)->totalscore[0] < min1){
      min1 = v_algorithms.at(i)->totalscore[0];
      i1 = i;
    }
    if(v_algorithms.at(i)->totalscore[1] < min2){
      min2 = v_algorithms.at(i)->totalscore[1];
      i2 = i;
    }
    if(v_algorithms.at(i)->totalscore[2] < min3){
      min3 = v_algorithms.at(i)->totalscore[2];
      i3 = i;
    }
    if(v_algorithms.at(i)->totalscore[3] < min4){
      min4 = v_algorithms.at(i)->totalscore[3];
      i4 = i;
    }
  }
  for(int i=0; i<v_algorithms.size();i++){
    std::cout << Form("%.20s %.2f %.2f %2f %.2f %d" ,v_algorithms.at(i)->name.Data(),v_algorithms.at(i)->totalscore[0],v_algorithms.at(i)->totalscore[1],v_algorithms.at(i)->totalscore[2],v_algorithms.at(i)->totalscore[3], scores.at(i)) << std::flush;
        if (i==i1){
          std::cout << " - best 1" << std::flush;
        }
        if (i==i2){
          std::cout << " - best 2" << std::flush;
        }
        if (i==i3){
          std::cout << " - best 3" << std::flush;
        }
        if (i==i4){
          std::cout << " - best 4" << std::flush;
        }
    std::cout << std::endl;
  }

}

void doTheTest(TH1D *hnom, TH1D *hsyst, const vector<Algorithm *> &v_algorithms, TString fname, float fig_weight, bool doPlots){

  //float fig_weight = 0.5; // 1-> only look at difference
  // 0-> only look at smoothness
  float fig1,fig2,fig3, fig4;
  TCanvas *c1 = new TCanvas("c1","c1",300*((v_algorithms.size()+1)/2),600);
  c1->Divide((v_algorithms.size()+1)/2,2);
  TH1D *ratio = (TH1D *) hsyst->Clone();
  ratio->Divide(hnom);
  float min=9e9, max=-9e9, cont=0;
  for(int i=1;i<ratio->GetNbinsX();i++){
    cont = ratio->GetBinContent(i);
    //    std::cout << "real " << i << " " << cont << std::endl;
    if(cont==0) continue;
    if(cont>max) max=cont;
    if(cont<min) min=cont;
  }
  max+=fabs(max-min)/4;
  min-=fabs(max-min)/4;
  TLatex tex;
  tex.SetTextSize(0.04);
  tex.SetNDC(1);

  for(int i=0; i<v_algorithms.size();i++){
    TH1D *smooth = v_algorithms.at(i)->apply(hnom,hsyst);
    float intRatio = hsyst->Integral()/hnom->Integral();
    fig1 = figMerit_binDiff(ratio,smooth,intRatio);
    fig2 = figMerit_binSmooth(ratio,smooth);
    fig3 = fig1*fig_weight+fig2*(1.-fig_weight);
    fig4 = fig3*hnom->Integral();;
    //for(int bin=0;bin< smooth->GetNbinsX();bin++){
    //  if(ratio->GetBinContent(bin)==0) continue;
    //  fig4 += pow(ratio->GetBinContent(bin)-intRatio,2);
    //}
    //fig4 = fig4*fig3*hnom->Integral();
    v_algorithms.at(i)->score[0] += fig1;
    v_algorithms.at(i)->score[1] += fig2;
    v_algorithms.at(i)->score[2] += fig3;
    v_algorithms.at(i)->score[3] += fig4;
    v_algorithms.at(i)->totalscore[0] += fig1;
    v_algorithms.at(i)->totalscore[1] += fig2;
    v_algorithms.at(i)->totalscore[2] += fig3;
    v_algorithms.at(i)->totalscore[3] += fig4;
    if(doPlots){
      c1->cd(1+i);
      smooth->SetLineColor(v_algorithms.at(i)->color);
      smooth->SetLineWidth(2);
      smooth->SetFillStyle(0);
      ratio->SetMaximum(max);
      ratio->SetMinimum(min);
      ratio->DrawCopy();
      smooth->DrawCopy("hist,same");
      tex.DrawLatex(0.2,0.8,v_algorithms.at(i)->name.Data());
    }
    //std::cout << Form("%f %f %f %f",fig1,fig2,fig3,fig4) << std::endl;
    delete smooth;
  }
  if(doPlots){
  //c1->SaveAs(plotpath+fname.ReplaceAll("root","eps"));
  c1->SaveAs(plotpath+fname.ReplaceAll("root","png"));
  }
  delete c1;
  delete ratio;
}

float figMerit_binDiff(TH1D *original, TH1D *smoothed, float intratio){

  float fig=0, maxdiff=0;
  for(int i=1;i<original->GetNbinsX()+1;i++){
    if(original->GetBinContent(i) ==0) continue;
    fig += pow((original->GetBinContent(i)-smoothed->GetBinContent(i))/original->GetBinError(i),2);
    maxdiff += pow((original->GetBinContent(i)-intratio)/original->GetBinError(i),2);
    if(isnan(fig) || isinf(fig)){
      std::cout << "Spotted figMerit_binDiff:" << fig << " " << maxdiff << " " << intratio << std::endl;
      std::cout << Form("%d %f %f %f",i,original->GetBinContent(i),smoothed->GetBinContent(i),original->GetBinError(i)) << std::endl;
      exit(1);
    }
  }
  //assert(fig/maxdiff <= 1);
  return fig/maxdiff;
}

float figMerit_binSmooth(TH1D *original, TH1D *smoothed){

  float fig=0, max=0;
  for(int i=1;i<smoothed->GetNbinsX();i++){
    if(smoothed->GetBinContent(i) ==0 || smoothed->GetBinContent(i+1) ==0 ) continue;
    fig += pow(smoothed->GetBinContent(i)-smoothed->GetBinContent(i+1),2);
    max += pow(original->GetBinContent(i)-original->GetBinContent(i+1),2);
    if(isnan(fig) || isinf(fig) || isnan(max) || isinf(max)){
      std::cout << "Spotted " << fig << std::endl;
      std::cout << Form("%d %f %f %f %f",i,smoothed->GetBinContent(i),smoothed->GetBinContent(i+1),smoothed->GetBinError(i),smoothed->GetBinError(i+1) )<< std::endl;
      exit(2);
    }
    //if(fig > max){
    //  std::cout << "Spotted " << fig << " " << max << std::endl;
    //  std::cout << Form("%d %f %f %f %f",i,smoothed->GetBinContent(i),smoothed->GetBinContent(i+1), original->GetBinContent(i),original->GetBinContent(i+1)) << std::endl;
    //  exit(2);
    //}
  }
  return fig/max;
}

int getdir (TString dir, vector<TString> &files)
{
  DIR *dp;
  struct dirent *dirp;
  if((dp  = opendir(dir.Data())) == NULL) {
    cout << "Error opening " << dir << endl;
  }

  while ((dirp = readdir(dp)) != NULL) {
    if (!TString(dirp->d_name).BeginsWith("."))
      files.push_back(TString(dirp->d_name));
  }
  closedir(dp);
  return 0;
}
