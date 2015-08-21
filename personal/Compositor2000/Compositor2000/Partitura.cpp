/*
 *  Partitura.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */
#include "Partitura.h"
#include "CompositorH.h"
#include "CompositorV.h"
#include "Algoritmo.h"
#include <fstream>
#include <iostream>


Partitura::Partitura(std::string acordesFile, std::string instrumentosFile,int m){
	
	std::cout << "Partitura: constructor ini" << std::endl;
	acordes = ParseAcordes(acordesFile);
	pistas = ParseInstrumentos(instrumentosFile);
	numCompases = acordes.size();
	numPistas = pistas.size();
	CompositorV compV(m);
	CompositorH compH(acordes.at(0));
	
	std::cout << "Partitura: compositor V working" << std::endl;
	for(int i=0;i<acordes.size();i++){
		//acordes.at(i).Print();
		matrizPos.push_back(compV.compose(Compas(acordes.at(i),pistas)));
	}
	std::cout << "Partitura: compositor H working" << std::endl;
	matrizCostes=compH.compose(matrizPos);
	std::cout << "Partitura: constructor end" << std::endl;

}

Partitura::~Partitura(){}

std::vector<Acorde> Partitura::ParseAcordes(std::string acordes){
	
	std::vector<Acorde> toret;
	std::string temp; 
	
	std::ifstream File(acordes.c_str()); 
	if(!File) { // file couldn't be opened
		std::cerr << "Error: file could not be opened: " << acordes << std::endl;
		exit(1);
	}
	bool setTonica=true;
	Acorde t;
	while(File >> temp){
		if(setTonica){
			t = Acorde(temp);
			setTonica=false;
		}
		Acorde a(temp);
		a.fijarTonica(t);
		toret.push_back(a);
	}
	File.close(); 		  
	return toret;
		  
}

std::vector<Instrumento> Partitura::ParseInstrumentos(std::string instrumentos){
	
	std::vector<Instrumento> pistas;
	std::string temp; 
	
	std::ifstream File(instrumentos.c_str()); 
	if(!File) { // file couldn't be opened
		std::cerr << "Error: file could not be opened: " << instrumentos << std::endl;
		exit(1);
	}
	while(File >> temp){
		pistas.push_back(Instrumento(temp));
	}
	File.close(); 		  
	return pistas;
}

std::vector<Solucion> Partitura::GetSoluciones(){
	return soluciones;
}

void Partitura::Resolver(){
	std::cout << "Partitura: resolver ini" << std::endl;
	Algoritmo algoritmo;
	soluciones = algoritmo.resolver(matrizPos,matrizCostes);
	std::cout << "Partitura: resolver end" << std::endl;

}

