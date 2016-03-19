#ifndef ComputeVar_H
#define ComputeVar_H
#include <vector>

class ComputeVar{

public:
  static double MTW(double var);
  static double Diff(double a, double b);
  static double BtagN(std::vector<float> btagw, int n);

};

#endif
