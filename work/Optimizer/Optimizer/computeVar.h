#ifndef computeVar_H
#define computeVar_H
#include <vector>

class computeVar{

public:
  static double MTW(double var);
  static double Diff(double a, double b);
  static double BtagN(std::vector<float> btagw, int n);

};

#endif
