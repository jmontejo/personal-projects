/*
 *  CompositorH.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "CompositorH.h"



CompositorH::CompositorH(Acorde t){
	tonica = t;
};

CompositorH::~CompositorH(){};

std::vector<std::vector<std::vector <Coste> > >CompositorH::compose(	std::vector<std::list <Posicion> > matrizPos){
	
	std::vector<std::vector<std::vector <Coste> > > costes; //primer indice "compas desde", segundo posicion A, tercero posicion B
	
	for(int i=0;i<matrizPos.size()-1;i++){
		costes.push_back(std::vector<std::vector <Coste> >());
		std::list<Posicion>::iterator prevCompas = matrizPos.at(i).begin();
		for(int j=0;j<matrizPos.at(i).size();j++){
			std::list<Posicion>::iterator postCompas = matrizPos.at(i+1).begin();
			costes.at(i).push_back(std::vector <Coste>());
			for(int k=0;k<matrizPos.at(i+1).size();k++){
				costes.at(i).at(j).push_back(getCost(*prevCompas,*postCompas));
				postCompas++;
			}
			prevCompas++;
		}

	}
	
	return costes;
}

Coste CompositorH::getCost(Posicion a, Posicion b){
	Coste coste;
	for(int i=0; i<a.notas.size(); i++){
		//Sensible en la soprano resuelve a tónica
		if(i==a.notas.size()-1){
			if(a.notas.at(i).nombre == (tonica.nombre +11)%12 && b.notas.at(i).ID != a.notas.at(i).ID +1)
				coste.AddCost(Coste::Sensible_Sin_Resolver);														
		}
		
		for(int j=i+1; j<a.notas.size(); j++){
		if(abs(a.notas.at(j).ID - a.notas.at(i).ID)%12 == 7 && abs(b.notas.at(j).ID - b.notas.at(i).ID)%12 == 7) //Quintas paralelas
			coste.AddCost(Coste::Quintas_Paralelas);														
		if(abs(a.notas.at(j).ID - a.notas.at(i).ID)%12==0 && abs(b.notas.at(j).ID - b.notas.at(i).ID)%12 == 0) //Octavas paralelas
			coste.AddCost(Coste::Octavas_Paralelas);
		if(((a.notas.at(j).ID - a.notas.at(i).ID > 0 && b.notas.at(j).ID - b.notas.at(i).ID > 0) ||
	        (a.notas.at(j).ID - a.notas.at(i).ID < 0 && b.notas.at(j).ID - b.notas.at(i).ID < 0))
		   && abs(a.notas.at(j).ID - b.notas.at(j).ID) == 7 ) //Quintas directas
			coste.AddCost(Coste::Quintas_Directas);
		if(((a.notas.at(j).ID - a.notas.at(i).ID > 0 && b.notas.at(j).ID - b.notas.at(i).ID > 0) ||
			(a.notas.at(j).ID - a.notas.at(i).ID < 0 && b.notas.at(j).ID - b.notas.at(i).ID < 0))
		   && abs(a.notas.at(j).ID - b.notas.at(j).ID) == 12 ) //Octavas directas
			coste.AddCost(Coste::Octavas_Directas);
		}

		if(i==0){ //Salto en el bajo superior a una quinta
			int salto = abs(a.notas.at(i).ID - b.notas.at(i).ID);
			if( salto > 7 && salto != 12)
				coste.AddCost(Coste::Salto_Bajo_MayorQueQuinta);
		}
		
		// Una voz no puede superar la nota anterior de la voz superior
		if(i<a.notas.size()-1){
			if(b.notas.at(i).ID > a.notas.at(i+1).ID || b.notas.at(i+1).ID < a.notas.at(i).ID)
				coste.AddCost(Coste::Superposicion);
		}
		
		// Si dos acordes tienen notas en común estas se repiten y el resto se mueve a la mas cercana (excepción página 25)
			if (a.notas.at(i).nombre == b.acorde.tonica && a.notas.at(i).ID != b.notas.at(i).ID)
				coste.AddCost(Coste::No_Mantener_Nota);
			if (a.notas.at(i).nombre == b.acorde.tercera && a.notas.at(i).ID != b.notas.at(i).ID)
				coste.AddCost(Coste::No_Mantener_Nota);
			if (a.notas.at(i).nombre == b.acorde.quinta && a.notas.at(i).ID != b.notas.at(i).ID)
				coste.AddCost(Coste::No_Mantener_Nota);

		// Si no tienen en común las voces superiores se mueven en sentido contrario al bajo (excepción página 25)
		if(i!=0 && !(a.acorde.tonica  == b.acorde.tonica  ||
		   a.acorde.tonica  == b.acorde.tercera ||
		   a.acorde.tonica  == b.acorde.quinta  ||
		   a.acorde.tercera == b.acorde.tonica  ||
		   a.acorde.tercera == b.acorde.tercera ||
		   a.acorde.tercera == b.acorde.quinta  ||
		   a.acorde.quinta  == b.acorde.tonica  ||
		   a.acorde.quinta  == b.acorde.tercera ||
		   a.acorde.quinta  == b.acorde.quinta)){
			if((a.notas.at(0).ID > b.notas.at(0).ID) && (a.notas.at(i).ID > b.notas.at(i).ID || abs(b.notas.at(i).ID - a.notas.at(i).ID) > 5))
				coste.AddCost(Coste::Mismo_Movimiento_QueElBajo);			
			if((a.notas.at(0).ID < b.notas.at(0).ID) && (a.notas.at(i).ID < b.notas.at(i).ID || abs(b.notas.at(i).ID - a.notas.at(i).ID) > 5))
					coste.AddCost(Coste::Mismo_Movimiento_QueElBajo);
		}
		
		// 
	}
	return coste;
}
