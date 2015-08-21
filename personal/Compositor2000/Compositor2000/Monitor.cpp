/*
 *  Monitor.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "Monitor.h"
#include <iostream>

Monitor::Monitor(int m){
	maxi = m;
};

Monitor::~Monitor(){};

void Monitor::Analyze(std::vector<Solucion> soluciones){

	std::vector<int> histo(maxi,0);
	for(int i=0;i<soluciones.size();i++){
		std::vector<int> indices = soluciones.at(i).getPosicionIndexWorst();
		for(int j=0;j<indices.size();j++){
			histo.at(indices.at(j)) += 1;
		}
	}
	for(int i=0;i<histo.size();i++)
		std::cout << i << " " << histo.at(i) << std::endl;

};