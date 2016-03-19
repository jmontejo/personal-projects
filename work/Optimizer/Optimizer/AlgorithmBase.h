class AlgorithmBase{

public:
  GeneticAlgorithm(int max);
  virtual void SetFCN(int n, void (*fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t));
  virtual void SetInitPool(  std::map<float,extern_chrom > &initPool);
  virtual void Minimize(int n);
  virtual void Print();
	virtual void Load(TString tag);
	virtual void Dump(TString tag);

};

