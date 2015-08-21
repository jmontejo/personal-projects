/*
 *  Instrumento.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */
#include "Instrumento.h"
#include <iostream>

Instrumento::Instrumento(std::string ins ){
	
	if(ins == "bajo"){
		max = bajo_up;
		min = bajo_low;
	}
	else if(ins == "baritono"){
		max = baritono_up;
		min = baritono_low;
	}
	else if(ins == "tenor"){
		max = tenor_up;
		min = tenor_low;
	}
	else if(ins == "contraalto"){
		max = contraalto_up;
		min = contraalto_low;
	}
	else if(ins == "mezzo" || ins == "mezzosoprano"){
		max = mezzo_up;
		min = mezzo_low;
	}
	else if(ins == "soprano"){
		max = soprano_up;
		min = soprano_low;
	}
	else{
		std::cout << "Instrumento desconocido: " << ins << std::endl;
		exit(1);
	}

}

Instrumento::~Instrumento(){}

std::vector<Nota> Instrumento::getNotasPermitidas(Acorde a){
	std::vector<Nota> permitidas;
	
	for(int i=min.ID; i<max.ID;i++){
		if(i%12 == a.tonica)
			permitidas.push_back(Nota(i));
		if(i%12 == a.tercera)
			permitidas.push_back(Nota(i));
		if(i%12 == a.quinta)
			permitidas.push_back(Nota(i));
		for(int j=0;j<a.adicionales.size();j++)
			if(i%12 == a.adicionales.at(j))
				permitidas.push_back(Nota(i));			
	}
	return permitidas;
}