/*
 *  Solucion.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "Solucion.h"

Solucion::Solucion(){
	Coste coste;
}

void Solucion::addPosicion(Posicion a){
	posiciones.push_back(a);
	coste.mergeCost(a.coste);
}

void Solucion::addPosicionIndex(int i){
	posicionesIndex.push_back(i);
}

std::vector<int> Solucion::getPosicionIndexWorst(){
	std::vector<int> toret(posicionesIndex);
	for(int i=0;i< alternativas.size();i++){
		std::vector<int> altpos = alternativas.at(i).getPosicionIndexWorst();
		for(int j=0;j<altpos.size();j++)
			if(toret.at(j) < altpos.at(j))
				toret.at(j)=altpos.at(j);
	}
	return toret;
}

void Solucion::addTransicion(Coste c){
	costes.push_back(c);
	coste.mergeCost(c);
}
void Solucion::addAlternativa(Solucion a){
	alternativas.push_back(a);
}

int Solucion::getNumAlternativas(){
	int alt = 1 ;
	for(int i=0;i<alternativas.size();i++)
		alt += alternativas.at(i).getNumAlternativas();
	return alt;
}

std::vector<Solucion> Solucion::getAlternativas(){
	return alternativas;
}