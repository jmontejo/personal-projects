#include "GeneticAlgorithm.h"
#include <iostream>
#include <iterator>

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
  Print();
  exit(1);
  Purge();
}
void GeneticAlgorithm::Minimize(int n){
  for(int i=0;i<n;i++){
    std::cout << "GeneticAlgorithm::Iterate " << i << std::endl;
    Iterate();
  }
  Print();

}
void GeneticAlgorithm::Analyze(std::vector<float> &min){
  for(int i=0;i<min.size();i++){
    bool useless = true;
    std::map<float,std::vector<float> >::iterator it = chromosomePool.begin();
    for(;it!=chromosomePool.end() && useless;it++){
      if(it->second.at(i) > min.at(i))
        useless = false;
    }
    if (useless)
      std::cout << "Useless variable: " << i <<" " << min.at(i) << std::endl;
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
      if(it->first > it2->first){
        chrom = Average(it->second, it2->second);
        chromosomePool[Evaluate(chrom)] = chrom;
      }
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
  if(chromosomePool.size()>maxpool)
  chromosomePool = std::map<float,std::vector<float> >(chromosomePool.begin(),std::next(chromosomePool.begin(),maxpool));

}
