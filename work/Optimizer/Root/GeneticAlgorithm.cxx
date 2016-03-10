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
void GeneticAlgorithm::SetVarStart(extern_chrom &start){
	InitPool(start,maxpool);
}
void GeneticAlgorithm::SetVarSteps(std::vector<std::pair<float, int> > &var_step){
	m_var_step = var_step;
}
void GeneticAlgorithm::SetVarMin(extern_chrom &min){
	m_min = min;
}
void GeneticAlgorithm::SetVarMax(extern_chrom &max){
	m_max = max;
}
void GeneticAlgorithm::SetInitPool(  std::map<float,extern_chrom > &initPool){
	std::cout <<"SetInitPool"<<std::endl;
	for(std::map<float,extern_chrom >::iterator it = initPool.begin();it!=initPool.end();it++){
		chromosomePool[it->first] = Code(it->second);
		//for(int c=0;c<it->second.size();c++)
		//	std::cout << c << " " << chromosomePool[it->first].at(c) << " ";
		//std::cout << std::endl;
	}
	Purge();
}
void GeneticAlgorithm::Minimize(int n){
	coded_chrom chrom(var_n);

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
//	chromosomePool[Evaluate(chrom)] = chrom;
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
//	chromosomePool[Evaluate(chrom)] = chrom;
//	std::cout << "Ben point: " << Evaluate(chrom) <<std::endl;
//	}


	for(int i=0;i<n;i++){
		std::cout << "GeneticAlgorithm::Iterate " << i << std::endl;
		Iterate(i>n*mutateabs_threshold && i%2==0);
	}

}

void GeneticAlgorithm::Analyze(std::vector<TString> &name, std::vector<float> &min){
	std::set<TString> uselessvar;
	std::set<TString> uselessvar1st;
	
	for(int i=0;i<name.size();i++){
		bool useless = true;
		bool useless1st = true;
		//    std::map<float,coded_chrom >::iterator it = chromosomePool.begin();
		for(auto it = chromosomePool.begin();it!=chromosomePool.end() && useless;it++){
			if(Decode(i,it->second.at(i)) > min.at(i)){
				useless = false;
			}
			if(Decode(i,it->second.at(i)) > min.at(i) && it == chromosomePool.begin()){
				useless1st = false;
			}
		}
		if (useless)
			uselessvar.insert(name.at(i));
		if (useless1st)
			uselessvar1st.insert(name.at(i));
	}

	std::cout <<std::endl << "----------------- TOP 10 RESULTS ------------" <<std::endl;
	std::cout <<std::setw(2+22+4) << std::setfill(' ') << "Vars / fig of merit    "; //four spaces for gev
	int c=0;
	for(auto it = chromosomePool.begin();it!=chromosomePool.end() && c<10;it++){
		std::cout <<  std::setw(9) << std::setfill(' ') << std::setprecision(3) << -it->first;
		c++;
	}
	std::cout << std::endl;
	for(int i=0;i<name.size();i++){
		if(uselessvar.count(name.at(i)))
			std::cout << "X ";
		else if(uselessvar.count(name.at(i)))
			std::cout << "x ";
		else
			std::cout << "  ";
		std::cout << std::setw(22) << std::setfill(' ') << name.at(i);
		float modulo = 0;
		TString gev;
		c=0;
		for(auto it = chromosomePool.begin();it!=chromosomePool.end() && c<10;it++){
			float value = Decode(i,it->second.at(i));
			if(!modulo){
				modulo = (fabs(value)>1000 ? 1000.: 1.);
				gev    = (fabs(value)>1000 ? " 1e3": "    ");
				std::cout << gev;
			}
			std::cout << std::setw(9) << std::setfill(' ') << std::setprecision(3) << value/modulo;
			c++;
		}
		std::cout << std::endl;
	}




}
void GeneticAlgorithm::Print(){

	std::cout << "GeneticAlgorithm::Print "<< chromosomePool.size() << std::endl;
	std::map<float,coded_chrom >::iterator it = chromosomePool.begin();
	for(;it!=chromosomePool.end();it++){
		std::cout << it->first << std::endl;
		for(int v=0;v<var_n;v++)
			std::cout <<"\t" << Decode(v,it->second.at(v));
		std::cout << std::endl;
	}   
}
void GeneticAlgorithm::InitPool(extern_chrom &start, int ini){
	coded_chrom chrom(var_n);
	while(chromosomePool.size() < ini){
		for(int v=0;v<var_n;v++){
			chrom.at(v) = Code(v,rand.Gaus(start.at(v),start.at(v)/2.));
		}
		chromosomePool[Evaluate(chrom)] = chrom;
	}
}
void GeneticAlgorithm::Iterate(bool mutateAbs){
	coded_chrom chrom(var_n);
	std::map<float,coded_chrom > original(chromosomePool);
	std::map<float,coded_chrom >::iterator it = original.begin();
	std::map<float,coded_chrom >::iterator it2;
	float value;
	int rank=1;
	for(;it!=original.end();it++){
		chrom = Mutate(it->second,float(rank*0.5)/original.size());
		value = Evaluate(chrom);
		//std::cout << "@@@@@@@@@ Mutate: " << rank << " " << -value << std::endl;
		chromosomePool[value] = chrom;
		if(value < it->first){ //returns -f
			chromosomePool.erase(it->first);
		}
		for(it2 = std::next(it,1);it2!=original.end();it2++){
			int swap = rand.Integer(2*var_n-1)-var_n+1;
			chrom = Combine(it->second, it2->second, swap); //(-var_n,var_n)
			value = Evaluate(chrom);
			//std::cout << "Combine: "<< -value << "  " << swap << std::endl;
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
coded_chrom GeneticAlgorithm::Mutate(coded_chrom &original, float frac){
	coded_chrom chrom(original);
	for(int i=0;i<chrom.size();i++)
		if(rand.Uniform()<frac){
			chrom.at(i) = rand.Binomial(m_var_step.at(i).second,float(chrom.at(i))/m_var_step.at(i).second);
			//chrom.at(i) = rand.Poisson(std::max(1,chrom.at(i)));
		}
	return chrom;
}
coded_chrom GeneticAlgorithm::Combine(coded_chrom &c1, coded_chrom &c2, int swappoint){
	if(swappoint<0){
	swappoint = abs(swappoint);
	coded_chrom chrom(c1);
	for(int i=0;i<swappoint+1;i++){
		chrom.at(i) = c2.at(i);
	}
	return chrom;
	}
	else{
	coded_chrom chrom(c2);
	for(int i=0;i<swappoint;i++)
		chrom.at(i) = c1.at(i);
	return chrom;
	}
}
coded_chrom GeneticAlgorithm::Average(coded_chrom &c1, coded_chrom &c2){
	coded_chrom chrom(c1);
	for(int i=0;i<chrom.size();i++)
		chrom.at(i) = (c1.at(i)+c2.at(i))/2.;
	return chrom;
}
float GeneticAlgorithm::Evaluate(coded_chrom &c, int forceOutput){
	Double_t ret;
	Double_t c_array[c.size()];
	extern_chrom ec = Decode(c);
	std::copy(ec.begin(), ec.end(), c_array);
	int size = c.size();
	m_fcn(size, (Double_t *) NULL, ret, c_array, forceOutput);
	return ret;
}  
void GeneticAlgorithm::Purge(){
	std::map<float,coded_chrom >::iterator it;
	std::map<float,coded_chrom >::iterator it2;
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
		chromosomePool = std::map<float,coded_chrom >(chromosomePool.begin(),std::next(chromosomePool.begin(),maxpool));
}
float GeneticAlgorithm::Similarity(coded_chrom &c1, coded_chrom &c2){
	float sim=0;
	int equalcount = 0;
	for(int i=0;i<c1.size();i++){
		if ((c1.at(i)+c2.at(i))>0)
			sim += sqrt(fabs(c1.at(i)-c2.at(i)))/(m_var_step.at(i).second);
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
			myfile << Decode(i,it->second.at(i)) << "\t";
		}
		myfile << std::endl;
	}
	myfile.close();

}
void GeneticAlgorithm::Load(TString tag){
	std::cout << "Load" <<std::endl;
	std::ifstream myfile;
	myfile.open ("Optimizer/plots/ga_dump_"+tag+".txt",std::ios::in);
	std::string line;
	while ( myfile.good() )
	{
		getline (myfile,line);
		coded_chrom chrom(var_n);
		std::istringstream iss(line);
		int i=0;
		while(iss)
		{
			std::string sub;
			iss >> sub;
			if(sub=="" || sub=="\t" || sub=="\n") break;
			chrom.at(i) = Code(i,TString(sub).Atof());
			i++;
		}
		chromosomePool[Evaluate(chrom)] = chrom;
	}
	myfile.close();
}

int GeneticAlgorithm::Code(int i, float v){
	//std::cout << "Code  : "<< i << " " << v << " ->  "<< int((v-m_min.at(i))/m_var_step.at(i).first) <<std::endl;
	return std::max(0,int((v-m_min.at(i))/m_var_step.at(i).first));	
}
float GeneticAlgorithm::Decode(int i, int v){
	//std::cout << "Decode: "<< i << " " << v << " ->  "<< m_min.at(i)+v*m_var_step.at(i).first <<std::endl;
	return m_min.at(i)+v*m_var_step.at(i).first;
}

extern_chrom GeneticAlgorithm::Decode(coded_chrom coded){
	extern_chrom ex(coded.size());
	for(int i=0; i<coded.size(); i++)
		ex.at(i) = Decode(i,coded.at(i));
	return ex;
}

coded_chrom GeneticAlgorithm::Code(extern_chrom ex){
	coded_chrom coded(ex.size());
	for(int i=0; i<ex.size(); i++)
		coded.at(i) = Code(i,ex.at(i));
	return coded;
}
