/*
 *  Algoritmo.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/27/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "Algoritmo.h"
#include <iostream>

std::vector<Solucion> Algoritmo::resolver(	std::vector<std::list <Posicion> > matrizPos, 
		std::vector<std::vector<std::vector <Coste> > >matrizCostes){

	std::vector<Solucion> soluciones(matrizCostes.at(0).size());

	for(int i=0; i<matrizPos.size();i++){
		std::vector<Solucion> tempSol(soluciones.size());
		std::list<Posicion>::iterator it;
		std::vector<int> alts;
		it=matrizPos.at(i).begin();
		for (int j=0; j<matrizPos.at(i).size(); j++) {
			alts.clear();
			if(i==0){
				tempSol.at(j).addPosicion(*it);
				tempSol.at(j).addPosicionIndex(j);
			}else{
				int bestSol, bestCost=9999, tempCost;
				for (int k=0; k<matrizPos.at(i-1).size(); k++) {
					tempCost = (*it).coste.valor + matrizCostes.at(i-1).at(k).at(j).valor + soluciones.at(k).coste.valor;
					if(tempCost < bestCost){
						bestCost = tempCost;
						bestSol = k;
						alts.clear();
					}
					else if (tempCost == bestCost)
						alts.push_back(k);
				}
				Solucion temp = soluciones.at(bestSol);
				temp.addPosicion(*it);
				temp.addPosicionIndex(j);
				temp.addTransicion(matrizCostes.at(i-1).at(bestSol).at(j));
				for(int a=0;a<alts.size();a++){
					Solucion alternativa = soluciones.at(alts.at(a));
					alternativa.addPosicion(*it);
					alternativa.addPosicionIndex(j);
					alternativa.addTransicion(matrizCostes.at(i-1).at(alts.at(a)).at(j));
					temp.addAlternativa(alternativa);
				}
				tempSol.at(j) = temp;
			}
			it++;
		}
		soluciones = tempSol;
	}
	return soluciones;
}

