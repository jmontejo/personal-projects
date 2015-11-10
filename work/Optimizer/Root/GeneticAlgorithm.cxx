#include "Optimizer/GeneticAlgorithm.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <fstream>
#include <set>
#define simil_threshold 0.01
#define equal_threshold 0.9
#define mutateabs_threshold 0.4

GeneticAlgorithm::GeneticAlgorithm(int max){
	maxpool = max;
	rand.SetSeed(0);
}

void GeneticAlgorithm::SetMaxPool(int max){
	maxpool = max;
}

void GeneticAlgorithm::SetFCN(int n, void (*fcn)(Int_t &, Double_t *, Double_t &f, Double_t *, Int_t)){
	var_n = n;
	m_fcn = fcn;
}
void GeneticAlgorithm::SetVarStart(std::vector<float> &start){
	InitPool(start,maxpool);
}
void GeneticAlgorithm::SetLimMin(std::vector<float> &min){
	m_min = min;
}
void GeneticAlgorithm::SetLimMax(std::vector<float> &max){
	m_max = max;
}
void GeneticAlgorithm::SetInitPool(  std::map<float,std::vector<float> > &initPool){
	for(std::map<float,std::vector<float> >::iterator it = initPool.begin();it!=initPool.end();it++){
		chromosomePool[it->first] = it->second;
	}
	Purge();
}
void GeneticAlgorithm::Minimize(int n){
	std::vector<float> chrom(var_n);

//	if(var_n==13){
//	chrom.at(0) = -2.5;  //        -dr_bjet_lep        -5.67    -5.13    -0.27    -0.27
//	chrom.at(1) = 175;  //                amt2 1e3     0.06     0.06     1.08     1.14
//	chrom.at(2) = 140e3;  //  fatjet_R10_L0_m[0] 1e3        0        0      765      765
//	chrom.at(3) = 300e3;  // fatjet_R10_L0_pt[0] 1e3        0        0 2.44e+03 2.74e+03
//	chrom.at(4) = 15;  //              ht_sig          -14      -14       85       85
//	chrom.at(5) = 120e3; // jet_pt[0]-jet_pt[1] 1e3        0        0 1.55e+03 1.63e+03
//	chrom.at(6) = 80e3; // jet_pt[1]-jet_pt[2] 1e3        0        0 1.94e+03 1.94e+03
//	chrom.at(7) = 50e3; // jet_pt[2]-jet_pt[3] 1e3        0        0      693      693
//	chrom.at(8) = 25e3; //           jet_pt[3] 1e3        0        0      513      513
//	chrom.at(9) = 350e3; //                 met 1e3       40       40 1.79e+03 1.79e+03
//	chrom.at(10) = 200e3; //                  mt 1e3        0        0 2.61e+03 2.61e+03
//	chrom.at(11) = 100; //             mt2_tau            0        0      952 1.01e+03
//	chrom.at(12) = 1.5; //             n_bjet
//	chromosomePool[Evaluate(chrom,1)] = chrom;
//	std::cout << "Ben point: " << Evaluate(chrom) <<std::endl;
//	}
//	if(var_n==15){
//	chrom.at(0) = -0.8;  //        -abs(met-met_cst)*pow(met,-1)
//	chrom.at(1) = -2.5;  //        -dr_bjet_lep        -5.67    -5.13    -0.27    -0.27
//	chrom.at(2) = -0.8;  //        -sqrt(pow(met_x-met_cst_x,2)+pow(met_y-met_cst_y,2))*pow(met,-1)
//	chrom.at(2+1) = 175;  //                amt2 1e3     0.06     0.06     1.08     1.14
//	chrom.at(2+2) = 140e3;  //  fatjet_R10_L0_m[0] 1e3        0        0      765      765
//	chrom.at(2+3) = 300e3;  // fatjet_R10_L0_pt[0] 1e3        0        0 2.44e+03 2.74e+03
//	chrom.at(2+4) = 15;  //              ht_sig          -14      -14       85       85
//	chrom.at(2+5) = 120e3; // jet_pt[0]-jet_pt[1] 1e3        0        0 1.55e+03 1.63e+03
//	chrom.at(2+6) = 80e3; // jet_pt[1]-jet_pt[2] 1e3        0        0 1.94e+03 1.94e+03
//	chrom.at(2+7) = 50e3; // jet_pt[2]-jet_pt[3] 1e3        0        0      693      693
//	chrom.at(2+8) = 25e3; //           jet_pt[3] 1e3        0        0      513      513
//	chrom.at(2+9) = 350e3; //                 met 1e3       40       40 1.79e+03 1.79e+03
//	chrom.at(2+10) = 200e3; //                  mt 1e3        0        0 2.61e+03 2.61e+03
//	chrom.at(2+11) = 100; //             mt2_tau            0        0      952 1.01e+03
//	chrom.at(2+12) = 1.5; //             n_bjet
//	chromosomePool[Evaluate(chrom,1)] = chrom;
//	std::cout << "Ben point: " << Evaluate(chrom) <<std::endl;
//	}


	for(int i=0;i<n;i++){
		std::cout << "GeneticAlgorithm::Iterate " << i << std::endl;
		chromosomePool[Evaluate(chrom)] = chrom;
		Iterate(i>n*mutateabs_threshold && i%2==0);
	}

}
void GeneticAlgorithm::Analyze(std::map<TString, double> &min){
	std::set<TString> uselessvar;
	std::set<TString> uselessvar1st;
	int i=0;
	for(auto itvar = min.begin(); itvar!=min.end();itvar++){
		bool useless = true;
		bool useless1st = true;
		//    std::map<float,std::vector<float> >::iterator it = chromosomePool.begin();
		for(auto it = chromosomePool.begin();it!=chromosomePool.end() && useless;it++){
			if(it->second.at(i) > itvar->second){
				useless = false;
			}
			if(it->second.at(i) > itvar->second && it == chromosomePool.begin()){
				useless1st = false;
			}
		}
		if (useless)
			uselessvar.insert(itvar->first);
		if (useless1st)
			uselessvar1st.insert(itvar->first);
		i++;
	}

	std::cout <<std::endl << "----------------- RESULTS --------------" <<std::endl;
	std::cout <<std::setw(2+22+4) << std::setfill(' ') << "Vars / fig of merit    "; //four spaces for gev
	for(auto it = chromosomePool.begin();it!=chromosomePool.end();it++){
		std::cout <<  std::setw(9) << std::setfill(' ') << std::setprecision(3) << -it->first;
	}
	std::cout << std::endl;
	i=0;
	for(auto itvar = min.begin(); itvar!=min.end();itvar++){
		if(uselessvar.count(itvar->first))
			std::cout << "X ";
		else if(uselessvar.count(itvar->first))
			std::cout << "x ";
		else
			std::cout << "  ";
		std::cout << std::setw(22) << std::setfill(' ') << itvar->first;
		float modulo = 0;
		TString gev;
		for(auto it = chromosomePool.begin();it!=chromosomePool.end();it++){
			if(!modulo){
				modulo = (fabs(it->second.at(i))>1000 ? 1000.: 1.);
				gev    = (fabs(it->second.at(i))>1000 ? " 1e3": "    ");
				std::cout << gev;
			}
			std::cout << std::setw(9) << std::setfill(' ') << std::setprecision(3) << it->second.at(i)/modulo;
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
void GeneticAlgorithm::Iterate(bool mutateAbs){
	std::vector<float> chrom(var_n);
	std::map<float,std::vector<float> > original(chromosomePool);
	std::map<float,std::vector<float> >::iterator it = original.begin();
	std::map<float,std::vector<float> >::iterator it2;
	float value;
	int rank=1;
	for(;it!=original.end();it++){
		if(mutateAbs)
			chrom = MutateAbs(it->second);
		else
			chrom = Mutate(it->second,rank/chromosomePool.size());
		value = Evaluate(chrom);
		chromosomePool[value] = chrom;
		if(value < it->first){ //returns -f
			chromosomePool.erase(it->first);
		}
		//std::cout << "Mutate: " << -value << std::endl;
		// double mutate
		chrom = Mutate(it->second,0.9);
		value = Evaluate(chrom);
		chromosomePool[value] = chrom;
		if(value < it->first){ //returns -f
			chromosomePool.erase(it->first);
		}
		//
		//std::cout << "Mutate: " << -value << std::endl;
		for(it2 = std::next(it,1);it2!=original.end();it2++){
			chrom = Combine(it->second, it2->second, rand.Integer(2*var_n-1)-var_n+1); //(-var_n,var_n)
			value = Evaluate(chrom);
			//std::cout << "Combine: "<< -value << std::endl;
			if(value < it->first){ //returns -f
				chromosomePool[value] = chrom;
				chromosomePool.erase(it->first);
				chromosomePool.erase(it2->first);
			}
		}
		//std::cout << rank;
		rank++;
	}

	Purge();
}
std::vector<float> GeneticAlgorithm::Mutate(std::vector<float> &original, float frac){
	std::vector<float> chrom(original);
	for(int i=0;i<chrom.size();i++)
		if(rand.Uniform()<frac)
		chrom.at(i) *= rand.Gaus(.99,0.4*frac);
	return chrom;
}
std::vector<float> GeneticAlgorithm::MutateAbs(std::vector<float> &original){
	std::vector<float> chrom(original);
	for(int i=0;i<chrom.size();i++)
		if(rand.Uniform()<(1./var_n)){
			float frac = rand.Uniform(2);
			if(frac<1)
				chrom.at(i) = m_min.at(i)*(1-frac)+chrom.at(i)*frac;
			else{
				frac -= 1;
				chrom.at(i) = m_max.at(i)*(1-frac)+chrom.at(i)*frac;
			}
		}
	return chrom;
}
std::vector<float> GeneticAlgorithm::Combine(std::vector<float> &c1, std::vector<float> &c2, int swappoint){
	if(swappoint<0){
	swappoint = abs(swappoint);
	std::vector<float> chrom(c1);
	for(int i=0;i<swappoint+1;i++){
		chrom.at(i) = c2.at(i);
	}
	return chrom;
	}
	else{
	std::vector<float> chrom(c2);
	for(int i=0;i<swappoint;i++)
		chrom.at(i) = c1.at(i);
	return chrom;
	}
}
std::vector<float> GeneticAlgorithm::Average(std::vector<float> &c1, std::vector<float> &c2){
	std::vector<float> chrom(c1);
	for(int i=0;i<chrom.size();i++)
		chrom.at(i) = (c1.at(i)+c2.at(i))/2.;
	return chrom;
}
float GeneticAlgorithm::Evaluate(std::vector<float> &c, int forceOutput){
	Double_t ret;
	Double_t c_array[c.size()];
	std::copy(c.begin(), c.end(), c_array);
	int size = c.size();
	m_fcn(size, (Double_t *) NULL, ret, c_array, forceOutput);
	return ret;
}  
void GeneticAlgorithm::Purge(){
	std::map<float,std::vector<float> >::iterator it;
	std::map<float,std::vector<float> >::iterator it2;
	int presize = chromosomePool.size();
	for(it=chromosomePool.begin();it!=chromosomePool.end() && chromosomePool.size()>maxpool && std::distance(chromosomePool.begin(),it)<maxpool;it++){
		for(it2 = std::next(it,1);it2!=chromosomePool.end() && chromosomePool.size()>maxpool;it2++){
			if(it==it2) continue;
			float similarity = Similarity(it->second,it2->second);
			if(similarity < simil_threshold*var_n){
				chromosomePool.erase(it2);
				//std::cout << similarity <<" Delete " << it2->first << std::endl;
			}
		}
	}
	std::cout << "Purge: " << presize << " -> " << chromosomePool.size() << std::endl;
	if(chromosomePool.size()>maxpool)
		chromosomePool = std::map<float,std::vector<float> >(chromosomePool.begin(),std::next(chromosomePool.begin(),maxpool));
}
float GeneticAlgorithm::Similarity(std::vector<float> &c1, std::vector<float> &c2){
	float sim=0;
	int equalcount = 0;
	for(int i=0;i<c1.size();i++){
		if ((c1.at(i)+c2.at(i))>0)
			sim += sqrt(fabs(c1.at(i)-c2.at(i)))/(c1.at(i)+c2.at(i));
		if (fabs(c1.at(i)-c2.at(i)) < 1e-6) 
			equalcount++;
	}
	if(equalcount > equal_threshold*c1.size()) return 0;
	return sim;
}
void GeneticAlgorithm::Dump(TString tag){
	std::ofstream myfile;
	myfile.open ("Optimizer/plots/ga_dump_"+tag+".txt");
	for(auto it=chromosomePool.begin();it!=chromosomePool.end();it++){
		for(int i=0;i<it->second.size();i++){
			myfile << it->second.at(i) << "\t";
		}
		myfile << std::endl;
	}
	myfile.close();

}
void GeneticAlgorithm::Load(TString tag){
	std::ifstream myfile;
	myfile.open ("Optimizer/plots/ga_dump_"+tag+".txt",std::ios::in);
	std::string line;
	while ( myfile.good() )
	{
		getline (myfile,line);
		std::vector<float> chrom(var_n);
		std::istringstream iss(line);
		int i=0;
		while(iss)
		{
			std::string sub;
			iss >> sub;
			if(sub=="" || sub=="\t" || sub=="\n") break;
			chrom.at(i) = TString(sub).Atof();
			i++;
		}
		chromosomePool[Evaluate(chrom)] = chrom;
	}
	myfile.close();
}
