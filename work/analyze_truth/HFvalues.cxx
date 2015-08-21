#include <iostream>
#include <fstream>
#include "HFvalues.h"

using google::sparse_hash_map;      // namespace where class lives by default
//using __gnu_cxx::hash;  // or __gnu_cxx::hash, or maybe tr1::hash, depending on your OS

//std::map<int,HFvalues::Values> HFvalues::HFmap;
sparse_hash_map<int,HFvalues::Values, hash<int> > HFvalues::HFmap;

bool HFvalues::initialize(){

  std::ifstream input_file("HFvalues.txt");
  int evnum, hftype;
  float var1, var2;

  std::cout << "Filling map" << std::endl;
  while (input_file >> evnum >> hftype >> var1 >> var2) {
    HFmap[evnum] = (HFvalues::Values) {hftype, var1, var2};
  }
  std::cout << "Filled map" << std::endl;

  return true;

}

bool HFvalues::getHFvalues(int evnum, int *HFtype, float *var1, float *var2){

  if(! HFmap.count(evnum)) return false;
  HFvalues::Values v = HFmap[evnum];
  *HFtype = v.HFtype;
  *var1   = v.var1  ;
  *var2   = v.var2  ;
  
  return true;
  
}
