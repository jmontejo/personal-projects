#include<TRandom3.h>
#include<map>
#include<vector>

class GeneticAlgorithm{

public:
  GeneticAlgorithm(int max);
  void SetMaxPool(int n);
  void SetFCN(int n, void (*fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t));
  void SetVarStart(std::vector<float> &start);
  void SetLimMin(std::vector<float> &min);
  void SetLimMax(std::vector<float> &max);
  void SetInitPool(  std::map<float,std::vector<float> > &initPool);
  void Analyze(std::map<TString, double> &min);
  void Minimize(int n);
  void Print();
	void Load();
	void Dump();

private:
  std::map<float,std::vector<float> > chromosomePool;
  void InitPool(std::vector<float> &start, int ini);
  void Iterate(bool mutateAbs);
  void Purge();
  float Similarity(std::vector<float> &c1, std::vector<float> &c2);
  std::vector<float> Mutate(std::vector<float> &original, float frac);
  std::vector<float> MutateAbs(std::vector<float> &original);
  std::vector<float> Combine(std::vector<float> &c1, std::vector<float> &c2, int swappoint);
  std::vector<float> Average(std::vector<float> &c1, std::vector<float> &c2);
  void (*m_fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t);
  float Evaluate(std::vector<float> &c);
  TRandom3 rand;
  int var_n, maxpool;
	std::vector<float> m_min, m_max;

};
