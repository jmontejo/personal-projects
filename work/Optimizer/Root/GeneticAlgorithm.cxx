#include "Optimizer/GeneticAlgorithm.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include <set>
#define threshold 0.01

GeneticAlgorithm::GeneticAlgorithm(int max){
  maxpool = max;
}

void GeneticAlgorithm::SetFCN(int n, void (*fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t)){
  var_n = n;
  m_fcn = fcn;
}
void GeneticAlgorithm::SetVarStart(std::vector<float> &start){
  InitPool(start,maxpool);
}
void GeneticAlgorithm::SetInitPool(  std::map<float,std::vector<float> > &initPool){
  chromosomePool = initPool;
  Purge();
}
void GeneticAlgorithm::Minimize(int n){
  for(int i=0;i<n;i++){
    std::cout << "GeneticAlgorithm::Iterate " << i << std::endl;
    Iterate();
  }
  Print();

}
void GeneticAlgorithm::Analyze(std::map<TString, double> &min){
  std::set<TString> uselessvar;
  int i=0;
  for(auto itvar = min.begin(); itvar!=min.end();itvar++){
    bool useless = true;
//    std::map<float,std::vector<float> >::iterator it = chromosomePool.begin();
    for(auto it = chromosomePool.begin();it!=chromosomePool.end() && useless;it++){
      if(it->second.at(i) > itvar->second){
        useless = false;
      }
    }
    if (useless)
      uselessvar.insert(itvar->first);
    i++;
  }

  std::cout <<std::endl << "----------------- RESULTS --------------" <<std::endl;
  std::cout <<std::setw(20) << std::setfill(' ') << "Some header"<< std::endl;
  i=0;
  for(auto itvar = min.begin(); itvar!=min.end();itvar++){
    if(uselessvar.count(itvar->first))
      std::cout << "X-";
    else
      std::cout << "  ";
    std::cout << std::setw(20) << std::setfill(' ') << itvar->first;
    float modulo = 0;
    TString gev;
    for(auto it = chromosomePool.begin();it!=chromosomePool.end();it++){
      if(!modulo){
        modulo = (fabs(it->second.at(i))>1000 ? 1000.: 1.);
        gev    = (fabs(it->second.at(i))>1000 ? " 1e3": "    ");
        std::cout << gev;
      }
       std::cout << std::setw(7) << std::setfill(' ') << std::setprecision(3) << it->second.at(i)/modulo;
    }
    std::cout << std::endl;
    i++;
  }




}
void GeneticAlgorithm::Print(){

  std::cout << "GeneticAlgorithm::Print "<< chromosomePool.size() << std::endl;
  std::map<float,std::vector<float> >::iterator it = chromosomePool.begin();
  for(;it!=chromosomePool.end();it++){
    std::cout << it->first << std::endl;
    for(int v=0;v<var_n;v++)
      std::cout <<"\t" << it->second.at(v);
    std::cout << std::endl;
  }   
}
void GeneticAlgorithm::InitPool(std::vector<float> &start, int ini){
  std::vector<float> chrom(var_n);
  while(chromosomePool.size() < ini){
    for(int v=0;v<var_n;v++){
      chrom.at(v) = rand.Gaus(start.at(v),start.at(v)/2.);
    }
    chromosomePool[Evaluate(chrom)] = chrom;
  }
}
void GeneticAlgorithm::Iterate(){
  std::vector<float> chrom(var_n);
  std::map<float,std::vector<float> > original(chromosomePool);
  std::map<float,std::vector<float> >::iterator it = original.begin();
  for(;it!=original.end();it++){
    chrom = Mutate(it->second);
    chromosomePool[Evaluate(chrom)] = chrom;
  }
  std::map<float,std::vector<float> >::iterator it2;

  for(it=original.begin();it!=original.end();it++){
    for(it2 = original.begin();it2!=original.end();it2++){
      if(it==it2) continue;
      chrom = Combine(it->second, it2->second, rand.Integer(var_n-1));
      chromosomePool[Evaluate(chrom)] = chrom;
      //if(it->first > it2->first){
      //  chrom = Average(it->second, it2->second);
      //  chromosomePool[Evaluate(chrom)] = chrom;
      //}
    }
  }

  Purge();
}
std::vector<float> GeneticAlgorithm::Mutate(std::vector<float> &original){
  std::vector<float> chrom(original);
  for(int i=0;i<chrom.size();i++)
    chrom.at(i) *= rand.Gaus(1,0.5);
  return chrom;
}
std::vector<float> GeneticAlgorithm::Combine(std::vector<float> &c1, std::vector<float> &c2, int swappoint){
  std::vector<float> chrom(c1);
  for(int i=0;i<swappoint+1;i++)
    chrom.at(i) = c2.at(i);
  return chrom;
}
std::vector<float> GeneticAlgorithm::Average(std::vector<float> &c1, std::vector<float> &c2){
  std::vector<float> chrom(c1);
  for(int i=0;i<chrom.size();i++)
    chrom.at(i) = (c1.at(i)+c2.at(i))/2.;
  return chrom;
}
float GeneticAlgorithm::Evaluate(std::vector<float> &c){
  Double_t ret;
  Double_t c_array[c.size()];
  std::copy(c.begin(), c.end(), c_array);
  int size = c.size();
  m_fcn(size, (Double_t *) NULL, ret, c_array, 0);
  return ret;
}  
void GeneticAlgorithm::Purge(){
  std::map<float,std::vector<float> >::iterator it;
  std::map<float,std::vector<float> >::iterator it2;
  for(it=chromosomePool.begin();it!=chromosomePool.end();it++){
    for(it2 = std::next(it,1);it2!=chromosomePool.end();it2++){
      if(it==it2) continue;
      float similarity = Similarity(it->second,it2->second);
      if(similarity < threshold*var_n){
        chromosomePool.erase(it2);
        //std::cout << similarity <<" Delete " << it2->first << std::endl;
      }
    }
  }
  if(chromosomePool.size()>maxpool)
  chromosomePool = std::map<float,std::vector<float> >(chromosomePool.begin(),std::next(chromosomePool.begin(),maxpool));
}
float GeneticAlgorithm::Similarity(std::vector<float> &c1, std::vector<float> &c2){
  float sim=0;
  for(int i=0;i<c1.size();i++){
    if ((c1.at(i)+c2.at(i))>0)
      sim += fabs(c1.at(i)-c2.at(i))/(c1.at(i)+c2.at(i));
  }
  return sim;
}
