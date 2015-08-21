#include "TH1D.h"
#include <vector>
#include <TMath.h>
#include <iostream>

// ALL THE FUNCTIONS ARE RESPONSIBLE FOR COMPUTING THE RATIO IN ADDITION TO THE SMOOTHING

TH1D * noSmooth(TH1D *hnom, TH1D *hsyst){
  TH1D* tmp = (TH1D*)hsyst->Clone();
  tmp->Divide(hnom);
  return tmp;
}

TH1D* rebin_dBoverB( TH1D* hnom, TH1D *hsys, double tolerance ) {  

  TH1D* nominal=(TH1D*)hnom->Clone("tmpHist");
  std::vector<double> binLimit;  
  binLimit.push_back( nominal->GetXaxis()->GetXmin() );    
  double relErr=20000;  
  double cumulInt=0;  
  double cumulErr=0;  
  int thisBin=0;  
  do {    
    do {      
      thisBin++;      
      cumulInt+=fabs(nominal->GetBinContent(thisBin));      
      cumulErr+=nominal->GetBinError(thisBin)*nominal->GetBinError(thisBin);      
      if ( cumulInt!=0 ) relErr= sqrt(cumulErr)/cumulInt;      
    } while (relErr>tolerance && thisBin!=nominal->GetNbinsX() );    
    binLimit.push_back(nominal->GetBinCenter(thisBin)+nominal->GetBinWidth(thisBin)/2);    
    cumulInt=0;    
    cumulErr=0;    
  } while ( thisBin!=nominal->GetNbinsX() );    
  double* Bins;   
  Bins=new double[binLimit.size()];
  for ( unsigned int i=0; i< binLimit.size(); i++) {    
    Bins[i]=binLimit[i];  
  }    
  TH1D* binTemplate=new TH1D( "binRef", "binRef", binLimit.size()-1, Bins);  
  binTemplate->Sumw2();    
  for (int V=1; V<=binTemplate->GetNbinsX(); V++) {
    binTemplate->SetBinContent(V,V);
  }

  TH1D* nomBinned=new TH1D(*binTemplate);
  nomBinned->Reset();
  for (int bin=0; bin<=hnom->GetNbinsX(); bin++) {
    int bigBin=nomBinned->FindBin( hnom->GetBinCenter(bin));
    nomBinned->SetBinContent( bigBin, nomBinned->GetBinContent(bigBin)+ hnom->GetBinContent(bin) );
    nomBinned->SetBinError( bigBin, sqrt( pow(nomBinned->GetBinError(bigBin),2) + pow(hnom->GetBinError(bin),2) ) );
  }
  TH1D* sysBinned=new TH1D(*binTemplate);
  sysBinned->Reset();
  for (int bin=0; bin<=hsys->GetNbinsX(); bin++) {
    int bigBin=sysBinned->FindBin( hsys->GetBinCenter(bin));
    sysBinned->SetBinContent( bigBin, sysBinned->GetBinContent(bigBin)+ hsys->GetBinContent(bin) );
    sysBinned->SetBinError( bigBin, sqrt( pow(sysBinned->GetBinError(bigBin),2) + pow(hsys->GetBinError(bin),2) ) );
  }

  TH1D* ratioBinned =(TH1D*)sysBinned->Clone();
  ratioBinned->Divide(nomBinned);
  TH1D* ratio = (TH1D *) hnom->Clone();
  for (int bin=1; bin<=ratio->GetNbinsX(); bin++) {
    ratio->SetBinContent( bin, ratioBinned->GetBinContent( ratioBinned->FindBin( ratio->GetBinCenter(bin) ) ));
    ratio->SetBinError( bin, ratioBinned->GetBinError( ratioBinned->FindBin( ratio->GetBinCenter(bin) ) ));
  }

  for(int i=0; i< hnom->GetNbinsX()+1;i++){
    if(hnom->GetBinContent(i)>0)
      break;
    ratio->SetBinContent(i,0);
  }
  for(int i=hnom->GetNbinsX(); i>= 1;i--){
    if(hnom->GetBinContent(i)>0)
      break;
    ratio->SetBinContent(i,0);
  }

  delete nominal;
  delete ratioBinned;
  delete nomBinned;
  delete sysBinned;
  delete binTemplate;
  delete [] Bins;
  return ratio;
}

TH1D* rebin_dBoverB_0( TH1D* hnom, TH1D *hsyst ) { return rebin_dBoverB(hnom,hsyst,0.00); }
TH1D* rebin_dBoverB_1( TH1D* hnom, TH1D *hsyst ) { return rebin_dBoverB(hnom,hsyst,0.01); }
TH1D* rebin_dBoverB_2( TH1D* hnom, TH1D *hsyst ) { return rebin_dBoverB(hnom,hsyst,0.02); }
TH1D* rebin_dBoverB_4( TH1D* hnom, TH1D *hsyst ) { return rebin_dBoverB(hnom,hsyst,0.04); }
TH1D* rebin_dBoverB_5( TH1D* hnom, TH1D *hsyst ) { return rebin_dBoverB(hnom,hsyst,0.05); }
TH1D* rebin_dBoverB_8( TH1D* hnom, TH1D *hsyst ) { return rebin_dBoverB(hnom,hsyst,0.08); }


int get_nVar(TH1D* hratio){

  int debug = 0;
  int nVar=0;
  bool thisUp = true;
  bool goingUp = true;
  double prevBin = 0, thisBin = 0;
  int usedBins = 0;
  if(debug)
    std::cout << "get_nVar" << std::endl;
  for (int bin=1; bin <=hratio->GetNbinsX(); bin++) {
    if(debug)
      std::cout << "Bin/content: " << bin << " " << hratio->GetBinContent(bin) << std::endl;
    if(hratio->GetBinContent(bin)==0) continue;
    prevBin = thisBin;
    thisBin = hratio->GetBinContent(bin);
    if(debug)
      std::cout << "Prev/this: "<<prevBin << " " << thisBin << std::endl;
    if(fabs(thisBin-prevBin)<1e-7) continue;
    usedBins++;
    thisUp = (thisBin > prevBin);
    if (usedBins > 2 && ((thisUp && !goingUp) || (!thisUp && goingUp)))
      nVar++;
    if(debug)
      std::cout << "Var, usedBins, up/wasup: " <<nVar << " "<< usedBins << " " << thisUp<<goingUp << std::endl;
    goingUp = thisUp;
  }
  if(debug)
    std::cout << "out get_nVar:" << nVar << std::endl;

  return nVar;

}

int rebin_getMaxVar(TH1D* hnom,TH1D* hsyst, double tolerance){

  int debug =0;
  if(debug)
    for(int i=1;i<=hsyst->GetNbinsX();i++)
      std::cout << "In: " << hnom->GetBinContent(i) << " " << hsyst->GetBinContent(i) << " " << hsyst->GetBinContent(i)/hnom->GetBinContent(i)<< std::endl;
  // define the desired rebinning
  std::vector<double> binLimit;  
  binLimit.push_back( hnom->GetXaxis()->GetXmin() );
  double relErr=20000;  
  double cumulIntSyst=0;
  double cumulInt=0;
  double cumulErr=0;  
  int thisBin=0;  

  do {    
    do {      
      thisBin++;
      cumulInt+=fabs(hnom->GetBinContent(thisBin));
      cumulErr+=hnom->GetBinError(thisBin)*hnom->GetBinError(thisBin);
      cumulIntSyst += hsyst->GetBinContent(thisBin);
      if ( cumulInt!=0 && cumulIntSyst!=0 ){
        relErr= sqrt(cumulErr)/cumulInt;
      }
      if (relErr==0) relErr=20000;

      if(debug){
        std::cout << thisBin << " " << hnom->GetBinContent(thisBin) << " "<< hnom->GetBinError(thisBin)<<std::endl;
        std::cout << sqrt(cumulErr) << " " << cumulInt << " " << relErr << " " << tolerance << std::endl;
      }
    } while (relErr > tolerance && thisBin!=hnom->GetNbinsX() );
    if( relErr < tolerance )
      binLimit.push_back(hnom->GetBinCenter(thisBin)+hnom->GetBinWidth(thisBin)/2);
    else
      binLimit.back() = hnom->GetBinCenter(thisBin)+hnom->GetBinWidth(thisBin)/2;
    if(debug)
      std::cout << "push back bin: " << thisBin <<std::endl;
    cumulInt=0;
    cumulErr=0;
    cumulIntSyst=0;
    relErr=20000;
  } while ( thisBin!=hnom->GetNbinsX() );
  double* Bins;   
  Bins=new double[binLimit.size()];
  for ( unsigned int i=0; i< binLimit.size(); i++) {
    Bins[i]=binLimit[i];
  }
  TH1D* rebinTemplate=new TH1D( "binRef", "binRef", binLimit.size()-1, Bins);
  rebinTemplate->Sumw2();
  for (int V=1; V<=rebinTemplate->GetNbinsX(); V++) {
    rebinTemplate->SetBinContent(V,V);
  }

  //apply the rebin template to sys and nominal
  TH1D* hnomBinned=new TH1D(*rebinTemplate);
  hnomBinned->Reset();
  for (int bin=0; bin <=hnom->GetNbinsX(); bin++) {
    int bigBin=hnomBinned->FindBin( hnom->GetBinCenter(bin));
    hnomBinned->SetBinContent( bigBin, hnomBinned->GetBinContent(bigBin)+ hnom->GetBinContent(bin) );
    hnomBinned->SetBinError( bigBin, sqrt( pow(hnomBinned->GetBinError(bigBin),2) + pow(hnom->GetBinError(bin),2) ) );
  }
  TH1D* hsystBinned=new TH1D(*rebinTemplate);
  hsystBinned->Reset();
  for (int bin=0; bin <=hsyst->GetNbinsX(); bin++) {
    int bigBin=hsystBinned->FindBin( hsyst->GetBinCenter(bin));
    hsystBinned->SetBinContent( bigBin, hsystBinned->GetBinContent(bigBin)+ hsyst->GetBinContent(bin) );
    hsystBinned->SetBinError( bigBin, sqrt( pow(hsystBinned->GetBinError(bigBin),2) + pow(hsyst->GetBinError(bin),2) ) );
  }
  double hsystBinnedint = hsystBinned->Integral();
  TH1D* hratioBinned= (TH1D *) hsystBinned->Clone();
  hsystBinned->Divide(hnomBinned);
  double scale = hsystBinnedint/hnomBinned->Integral()-1;
  hratioBinned->Add(hnomBinned,-1);
  hratioBinned->Divide(hnomBinned);
  //std::cout << "--- hratioBinned ---" << std::endl;
  //for(int i=0; i< hratioBinned->GetNbinsX()+1;i++)
  //  std::cout << i << " " << hratioBinned->GetBinContent(i) << std::endl;
  TH1D* unitBinned=new TH1D(*rebinTemplate);
  for(int i=1;i<=unitBinned->GetNbinsX();i++)
    unitBinned->SetBinContent(i,1);
  unitBinned->Scale(scale);
  hratioBinned->Add(unitBinned,-1);

  for(int i=1;i<=hsyst->GetNbinsX();i++)
    hsyst->SetBinContent(i,hnom->GetBinContent(i)*hsystBinned->GetBinContent(hsystBinned->FindBin( hsyst->GetBinCenter(i))));
  int nVar = get_nVar(hratioBinned);

  delete rebinTemplate;
  delete hnomBinned;
  delete hsystBinned;
  delete unitBinned;
  delete hratioBinned;
  delete [] Bins;

  return nVar;
}

int rebin_getSignficant(TH1D* hnom,TH1D* hsyst, double tolerance){

  int debug =0;
  if(debug)
    for(int i=1;i<=hsyst->GetNbinsX();i++)
      std::cout << "In: " << hnom->GetBinContent(i) << " " << hsyst->GetBinContent(i) << std::endl;
  // define the desired rebinning
  std::vector<double> binLimit;  
  binLimit.push_back( hnom->GetXaxis()->GetXmin() );
  double relErr=20000;  
  double cumulIntSyst=0;
  double cumulInt=0;
  double cumulErr=0;  
  int thisBin=0;  

  do {    
    do {      
      thisBin++;
      cumulInt+=fabs(hnom->GetBinContent(thisBin));
      cumulErr+=hnom->GetBinError(thisBin)*hnom->GetBinError(thisBin);
      cumulIntSyst += hsyst->GetBinContent(thisBin);
      if ( cumulInt!=0 && cumulIntSyst!=0 ){
        relErr= sqrt(cumulErr)/cumulInt;
      }
      if (relErr==0) relErr=20000;

      if(debug){
        std::cout << thisBin << " " << hnom->GetBinContent(thisBin) << " "<< hnom->GetBinError(thisBin)<<std::endl;
        std::cout << sqrt(cumulErr) << " " << cumulInt << " " << relErr << " " << tolerance << std::endl;
      }
    } while (relErr > tolerance && thisBin!=hnom->GetNbinsX() );
    binLimit.push_back(hnom->GetBinCenter(thisBin)+hnom->GetBinWidth(thisBin)/2);
    if(debug)
      std::cout << "push back bin: " << thisBin <<std::endl;
    cumulInt=0;
    cumulErr=0;
    cumulIntSyst=0;
    relErr=20000;
  } while ( thisBin!=hnom->GetNbinsX() );
  double* Bins;   
  Bins=new double[binLimit.size()];
  for ( unsigned int i=0; i< binLimit.size(); i++) {
    Bins[i]=binLimit[i];
  }
  TH1D* rebinTemplate=new TH1D( "binRef", "binRef", binLimit.size()-1, Bins);
  rebinTemplate->Sumw2();
  for (int V=1; V<=rebinTemplate->GetNbinsX(); V++) {
    rebinTemplate->SetBinContent(V,V);
  }

  //apply the rebin template to sys and nominal
  TH1D* hnomBinned=new TH1D(*rebinTemplate);
  hnomBinned->Reset();
  for (int bin=0; bin <=hnom->GetNbinsX(); bin++) {
    int bigBin=hnomBinned->FindBin( hnom->GetBinCenter(bin));
    hnomBinned->SetBinContent( bigBin, hnomBinned->GetBinContent(bigBin)+ hnom->GetBinContent(bin) );
    hnomBinned->SetBinError( bigBin, sqrt( pow(hnomBinned->GetBinError(bigBin),2) + pow(hnom->GetBinError(bin),2) ) );
  }
  TH1D* hsystBinned=new TH1D(*rebinTemplate);
  hsystBinned->Reset();
  for (int bin=0; bin <=hsyst->GetNbinsX(); bin++) {
    int bigBin=hsystBinned->FindBin( hsyst->GetBinCenter(bin));
    hsystBinned->SetBinContent( bigBin, hsystBinned->GetBinContent(bigBin)+ hsyst->GetBinContent(bin) );
    hsystBinned->SetBinError( bigBin, sqrt( pow(hsystBinned->GetBinError(bigBin),2) + pow(hsyst->GetBinError(bin),2) ) );
  }
  double hsystBinnedint = hsystBinned->Integral();
  TH1D* hratioBinned= (TH1D *) hsystBinned->Clone();
  hsystBinned->Divide(hnomBinned);
  double scale = hsystBinnedint/hnomBinned->Integral()-1;
  hratioBinned->Add(hnomBinned,-1);
  hratioBinned->Divide(hnomBinned);
  TH1D* unitBinned=new TH1D(*rebinTemplate);
  for(int i=1;i<=unitBinned->GetNbinsX();i++)
    unitBinned->SetBinContent(i,1);
  unitBinned->Scale(scale);
  hratioBinned->Add(unitBinned,-1);

  for(int i=1;i<=hsyst->GetNbinsX();i++)
    hsyst->SetBinContent(i,hnom->GetBinContent(i)*hsystBinned->GetBinContent(hsystBinned->FindBin( hsyst->GetBinCenter(i))));
  int relevantBins=0;
  for(int i=1;i<=hsystBinned->GetNbinsX();i++){
    if(fabs(hratioBinned->GetBinContent(i)) > hnomBinned->GetBinError(i)/hnomBinned->GetBinContent(i))
      relevantBins++;
    if(debug)
      std::cout << i << " " << relevantBins << " " <<(hratioBinned->GetBinContent(i)) << "  "<<hnomBinned->GetBinContent(i) << " "  << hnomBinned->GetBinError(i)/hnomBinned->GetBinContent(i) << std::endl;
  }
  delete rebinTemplate;
  delete hnomBinned;
  delete hsystBinned;
  delete hratioBinned;
  delete unitBinned;
  delete [] Bins;

  if (binLimit.size()==2) return -1;
  return relevantBins;
}

void Smooth_maxVariations(TH1D* hnom,TH1D* hsyst, int nbins){

  int debug = 0;
  double tolerance = 0.8;
  int nVar = rebin_getMaxVar(hnom,hsyst,tolerance);
  if(debug)
    std::cout << "---: " << tolerance << " " << nVar << std::endl;
  while (nVar > nbins){
    tolerance = tolerance/2.;
    nVar = rebin_getMaxVar(hnom,hsyst,tolerance);
    if(debug)
      std::cout << "---: " << tolerance << " " << nVar << std::endl;
    if(tolerance==0){
      std::cout << "Buuuuuuuuuuug: infinite while" << std::endl;
      std::cout << hnom->GetName() << " " << hnom->GetTitle() << " nbins: " << nbins << std::endl;
      break;
    }
  }
  if(debug)
    std::cout << "Final: " << tolerance << " " << nVar << std::endl;
}

void Smooth_minSignificant(TH1D* hnom,TH1D* hsyst, int nbins){

  int debug = 0;
  double tolerance = 0.8;
  int countBin = rebin_getSignficant(hnom,hsyst,tolerance);
  if(debug)
    std::cout << "---: " << tolerance << " " << countBin << std::endl;
  while (countBin < nbins && countBin >= 0){
    tolerance = tolerance/2.;
    countBin = rebin_getSignficant(hnom,hsyst,tolerance);
    if(debug)
      std::cout << "---: " << tolerance << " " << countBin << std::endl;
    if(tolerance==0){
      std::cout << "Buuuuuuuuuuug: infinite while" << std::endl;
      std::cout << hnom->GetName() << " " << hnom->GetTitle() << " nbins: " << nbins << std::endl;
      exit(1);
      break;
    }
  }
  if(debug)
    std::cout << "Final: " << tolerance << " " << countBin << std::endl;
}


TH1D* rebin_maxVariations_0( TH1D* hnom, TH1D *hsyst ) { Smooth_maxVariations(hnom, hsyst, 0);   TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_maxVariations_1( TH1D* hnom, TH1D *hsyst ) { Smooth_maxVariations(hnom, hsyst, 1);   TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_maxVariations_2( TH1D* hnom, TH1D *hsyst ) { Smooth_maxVariations(hnom, hsyst, 2);   TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_maxVariations_3( TH1D* hnom, TH1D *hsyst ) { Smooth_maxVariations(hnom, hsyst, 3);   TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_maxVariations_4( TH1D* hnom, TH1D *hsyst ) { Smooth_maxVariations(hnom, hsyst, 4);   TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_maxVariations_5( TH1D* hnom, TH1D *hsyst ) { Smooth_maxVariations(hnom, hsyst, 5);   TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_maxVariations_6( TH1D* hnom, TH1D *hsyst ) { Smooth_maxVariations(hnom, hsyst, 6);   TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_maxVariations_8( TH1D* hnom, TH1D *hsyst ) { Smooth_maxVariations(hnom, hsyst, 8);   TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_maxVariations_adapt( TH1D* hnom, TH1D *hsyst ) { 
  Double_t error, integ;
  integ = hsyst->IntegralAndError(0,-1, error);
  int var = int(-log(error/integ) );
  //std::cout << error/integ << " " << var << std::endl;
  //std::cout << var << std::endl;
Smooth_maxVariations(hnom, hsyst, var);   TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; 
}
TH1D* rebin_minSignificant_1( TH1D* hnom, TH1D *hsyst ) { Smooth_minSignificant(hnom, hsyst, 1); TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_minSignificant_2( TH1D* hnom, TH1D *hsyst ) { Smooth_minSignificant(hnom, hsyst, 2); TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }
TH1D* rebin_minSignificant_4( TH1D* hnom, TH1D *hsyst ) { Smooth_minSignificant(hnom, hsyst, 4); TH1D *toret = (TH1D *) hsyst->Clone(); toret->Divide(hnom); return toret; }

