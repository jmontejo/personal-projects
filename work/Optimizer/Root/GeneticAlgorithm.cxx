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

	chrom.at(0) = 40;     //-klf_std_bestlikelihood           40       43     93.1     96.1
	chrom.at(1) = 145;   //                  amt2           10       10      883      883
	chrom.at(2) = 1e3;     // bjet_pt[0]-bjet_pt[1] 1e3        0        0      880      990
	chrom.at(3) = 1e3;     //            bjet_pt[1] 1e3        0        0      456      513
	chrom.at(4) = 0.5;   //      dphi_jet0_ptmiss            0     0.17     3.06     3.06
	chrom.at(5) = 0.3;   //      dphi_jet1_ptmiss            0        0     3.14     3.14
	chrom.at(6) = 0.1;     //      dphi_jet2_ptmiss            0        0     3.11     3.14
	chrom.at(7) = 0.1;     //      dphi_jet3_ptmiss            0        0     3.14     3.14
	chrom.at(8) = 75e3;  //           fatjet_m[0] 1e3        0        0      567      567
	chrom.at(9) = 270e3;//          fatjet_pt[0] 1e3        0        0 1.72e+03 1.94e+03
	chrom.at(10) = 1e3;    //                    ht 1e3        0        0 2.61e+03 2.61e+03
	chrom.at(11) = 1e3;    //               ht_incl 1e3        0        0 2.79e+03 2.79e+03
	chrom.at(12) = 10;   //                ht_sig        -17.8      -10     36.8     60.2
	chrom.at(13) = 10e3; //   jet_pt[0]-jet_pt[1] 1e3        0        0 1.58e+03 1.58e+03
	chrom.at(14) = 25e3; //   jet_pt[1]-jet_pt[2] 1e3        0        0      729      729
	chrom.at(15) = 15e3; //   jet_pt[2]-jet_pt[3] 1e3        0        0      297      297
	chrom.at(16) = 25e3; //             jet_pt[3] 1e3     18.2     18.2      244      257
	chrom.at(17) = -90; //klf_noMET_bestlikelihood          -89      -89    -40.4    -40.4
	chrom.at(18) = 25e3;    //                lep_pt 1e3        0        0      738      738
	chrom.at(19) = 315e3;//                   met 1e3       60       60 1.47e+03 1.64e+03
	chrom.at(20) = 3;    //               met_sig            3     4.97     32.6     38.5
	chrom.at(21) = 175e3;//                    mt 1e3        0        0 1.26e+03 1.26e+03
	chrom.at(22) = 10;    //               mt2_tau            0        0      420      756
	chrom.at(23) = 10;    //             mt2_tau50            0        0      504      756
	chrom.at(24) = 10e3;    //               mt2stop 1e3        0        0  2.3e+03  2.3e+03
	chrom.at(25) = 10e3;    //     mt2stop_fit_noMET 1e3        0        0 2.25e+03 2.25e+03
	chrom.at(26) = 10e3;    //       mt2stop_fit_std 1e3        0        0 1.84e+03 1.84e+03
	chrom.at(27) = 7;    //               topness        -18.4      -15     15.6     15.6
		chromosomePool[Evaluate(chrom)] = chrom;

	for(int i=0;i<n;i++){
		std::cout << "GeneticAlgorithm::Iterate " << i << std::endl;
		chromosomePool[Evaluate(chrom)] = chrom;
		Iterate(i>n*mutateabs_threshold && i%2==0);
	}
	Print();

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
		std::cout << "Mutate: " << value << std::endl;
		// double mutate
		chrom = Mutate(it->second,0.9);
		value = Evaluate(chrom);
		chromosomePool[value] = chrom;
		if(value < it->first){ //returns -f
			chromosomePool.erase(it->first);
		}
		//
		std::cout << "Mutate: " << value << std::endl;
		for(it2 = std::next(it,1);it2!=original.end();it2++){
			chrom = Combine(it->second, it2->second, rand.Integer(2*var_n-1)-var_n+1); //(-var_n,var_n)
			value = Evaluate(chrom);
			std::cout << "Combine: "<< value << std::endl;
			if(value < it->first){ //returns -f
				chromosomePool[value] = chrom;
				chromosomePool.erase(it->first);
				chromosomePool.erase(it2->first);
			}
		}
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
float GeneticAlgorithm::Evaluate(std::vector<float> &c){
	Double_t ret;
	Double_t c_array[c.size()];
	std::copy(c.begin(), c.end(), c_array);
	int size = c.size();
	m_fcn(size, (Double_t *) NULL, ret, c_array, 0);
	return ret;
}  
void GeneticAlgorithm::Purge(){
	std::map<float,std::vector<float> >::iterator it;
	std::map<float,std::vector<float> >::iterator it2;
	int presize = chromosomePool.size();
	for(it=chromosomePool.begin();it!=chromosomePool.end() && chromosomePool.size()>maxpool;it++){
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
void GeneticAlgorithm::Dump(){
	std::ofstream myfile;
	myfile.open ("Optimizer/plots/ga_dump.txt");
	for(auto it=chromosomePool.begin();it!=chromosomePool.end();it++){
		for(int i=0;i<it->second.size();i++){
			myfile << it->second.at(i) << "\t";
		}
		myfile << std::endl;
	}
	myfile.close();

}
void GeneticAlgorithm::Load(){
	std::ifstream myfile;
	myfile.open ("Optimizer/plots/ga_dump.txt",std::ios::in);
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
