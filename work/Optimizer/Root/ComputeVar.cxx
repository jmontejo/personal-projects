#include "Optimizer/ComputeVar.h"
#include <vector>
#include <iostream>
#include <algorithm>

double ComputeVar::MTW(double var){
  //std::cout << "In MTW" << std::endl;
  return 2.;
}

double ComputeVar::Diff(double a, double b){
  return a-b;
}

double ComputeVar::BtagN(std::vector<float> btagw, int n){
  std::cout << "ComputeVar::BtagN" << n <<std::endl;
  std::sort(btagw.begin(),btagw.end());
  return btagw.at(n);
}
