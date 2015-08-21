#include "computeVar.h"
#include <vector>
#include <iostream>
#include <algorithm>

double computeVar::MTW(double var){
  //std::cout << "In MTW" << std::endl;
  return 2.;
}

double computeVar::Diff(double a, double b){
  return a-b;
}

double computeVar::BtagN(std::vector<float> btagw, int n){
  std::cout << "computeVar::BtagN" << n <<std::endl;
  std::sort(btagw.begin(),btagw.end());
  return btagw.at(n);
}
