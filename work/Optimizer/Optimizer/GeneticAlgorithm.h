#include<TRandom3.h>
#include<map>
#include<vector>

typedef std::vector<int> 	 coded_chrom;
typedef std::vector<float> extern_chrom;

class GeneticAlgorithm{

public:
  GeneticAlgorithm(int max);
  void SetMaxPool(int n);
  void SetFCN(int n, void (*fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t));
  void SetVarStart(extern_chrom &start);
	void SetVarSteps(std::vector<std::pair<float, int> > &var_step);
  void SetVarMin(extern_chrom &min);
  void SetVarMax(extern_chrom &max);
  void SetInitPool(  std::map<float,extern_chrom > &initPool);
  void Analyze(std::vector<TString> &name, std::vector<float> &min);
  void Minimize(int n);
  void Print();
	void Load(TString tag);
	void Dump(TString tag);

private:
  std::map<float,coded_chrom > chromosomePool;
  void InitPool(extern_chrom &start, int ini);
  void Iterate(bool mutateAbs);
  void Purge();
  float Similarity(coded_chrom &c1, coded_chrom &c2);
  coded_chrom Mutate(coded_chrom &original, float frac);
  coded_chrom Combine(coded_chrom &c1, coded_chrom &c2, int swappoint);
  coded_chrom Average(coded_chrom &c1, coded_chrom &c2);
  void (*m_fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t);
  float Evaluate(coded_chrom &c, int forceOutput=0);
  TRandom3 rand;
  int var_n, maxpool;
	extern_chrom m_min, m_max;
	std::vector<std::pair<float, int> > m_var_step;
	int Code(int i, float v);
	float Decode(int i, int v);
	extern_chrom Decode(coded_chrom coded);
	coded_chrom Code(extern_chrom ex);

};
