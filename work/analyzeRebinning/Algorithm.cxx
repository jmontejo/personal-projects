#include "Algorithm.h"
#include <iostream>

Algorithm::Algorithm(TString name, TH1D * (*function)(TH1D *, TH1D *),int color, int smooth, bool usebinwidth){

  this->name = name;
  this->color = color;
  this->smooth = smooth;
  this->usebinwidth = usebinwidth;
  this->function = function;
  score[0] = 0; score[1] = 0; score[2] = 0; score[3] = 0; score[4] = 0; 
  totalscore[0] = 0; totalscore[1] = 0; totalscore[2] = 0; totalscore[3] = 0; totalscore[4] = 0; 

}

int getBinWidth(TH1D *ratio){

  float prev=0;
  int count=1, mincount=99;
  for(int i=0;i< ratio->GetNbinsX();i++){
    if(ratio->GetBinContent(i)==0) continue;
    
    if(prev!=0){
      if( prev==ratio->GetBinContent(i) )
        count++;
      else{
        if(count < mincount) mincount=count;
        count=1;
      }
    }
    prev = ratio->GetBinContent(i);
  }
  if(count < mincount) mincount=count;
  if(mincount > 1)
    return mincount;
}

TH1D * Algorithm::apply(TH1D *h1, TH1D *h2){

  TH1D *tmphist1 = (TH1D *) h1->Clone();
  TH1D *tmphist2 = (TH1D *) h2->Clone();
  TH1D *tmphist = function(tmphist1,tmphist2);
  //std::cout << "--- Function applied ---" << std::endl;
  //for(int i=0; i< tmphist->GetNbinsX()+1;i++)
  //  std::cout << i << " " << tmphist->GetBinContent(i) << std::endl;
  
  int binwidth = 1; //getBinWidth(...)
  if(usebinwidth)
    binwidth = getBinWidth(tmphist);
  if(binwidth>4) binwidth=4;
  if(smooth > 0){
    int minbin = 0;
    for(int i=0; i< tmphist->GetNbinsX()+1;i++)
      if(tmphist->GetBinContent(i)>0){
        minbin = i;
        break;
      }
    int maxbin = tmphist->GetNbinsX();
    for(int i=maxbin; i>= 1;i--)
      if(tmphist->GetBinContent(i)>0){
        maxbin = i;
        break;
      }
    tmphist->GetXaxis()->SetRange(minbin,maxbin);
    tmphist->Smooth(binwidth*smooth,"R");

  }
  //std::cout << "--- Smoothed ---" << std::endl;
  //for(int i=0; i< tmphist->GetNbinsX()+1;i++)
  //  std::cout << i << " " << tmphist->GetBinContent(i) << std::endl;
  delete tmphist1;
  delete tmphist2;
  return tmphist;

}
