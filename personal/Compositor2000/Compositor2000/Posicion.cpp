/*
 *  Posicion.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "Posicion.h"

Posicion::Posicion(std::vector<Nota> n, Acorde a){
	notas = n;
	acorde = a;
	coste = calcularCoste(n,a);
	posID=0;
	for(int i=0;i<n.size();i++)
		posID += n.at(i).ID;
}

std::string Posicion::toString(){
	std::string s="<";
    for(int i=0;i<notas.size();i++){
		if(i!=0)
			s+=" ";
		s += notas.at(i).toString();
	}
	s +=">(";
	char c[20];
	sprintf(c,"%d",coste.valor);
	std::string s2(c);
	s += s2+")";
	return s;
}

Coste Posicion::calcularCoste(std::vector<Nota> n, Acorde a){
	Coste coste;
	if(a.grado==7){ //Sensible duplicada como tónica del 7o
		bool fund_dupl=false;
		for(int i=0;i<n.size();i++){
			if(n.at(i).nombre == a.tonica && fund_dupl)
				coste.AddCost(Coste::Sensible_Fundamental_Duplicada);
			else if (n.at(i).nombre == a.tonica && !fund_dupl)
				fund_dupl=true;
		}
	}
	if(a.grado==5){ //Sensible duplicada como tercera del 5o
		bool fund_dupl=false;
		for(int i=0;i<n.size();i++){
			if(n.at(i).nombre == a.tercera && fund_dupl)
				coste.AddCost(Coste::Dominante_Sensible_Duplicada);
			else if (n.at(i).nombre == a.tercera && !fund_dupl)
				fund_dupl=true;
		}
	}
	if(a.grado==3){ //Sensible duplicada como quinta del 3o
		bool fund_dupl=false;
		for(int i=0;i<n.size();i++){
			if(n.at(i).nombre == a.quinta && fund_dupl)
				coste.AddCost(Coste::Mediante_Quinta_Duplicada);
			else if (n.at(i).nombre == a.quinta && !fund_dupl)
				fund_dupl=true;
		}
	}
	
	for(int i=1; i<n.size()-1;i++){ //Intervalo superior a una octava entre voces salvo bajo y tenor
		if((n.at(i+1).ID - n.at(i).ID) > 12)
			coste.AddCost(Coste::Intervalo_MasQueOctava);
	}
	
	for(int i=0; i<n.size()-1;i++){ //Sin unisonos
		if(n.at(i+1).ID == n.at(i).ID)
			coste.AddCost(Coste::Unisono);
	}

	
	bool terc=false, quin=false;
	for(int i=0; i<n.size();i++){ //Acorde sin tercera
		if(n.at(i).nombre == a.tercera)
			terc = true;
		if(n.at(i).nombre == a.quinta)
			quin = true;
	}
	if(!terc)
		coste.AddCost(Coste::Acorde_Sin_Tercera);
	if(!quin)
		coste.AddCost(Coste::Acorde_Sin_Quinta);
	
	if(n.at(0).nombre != a.tonica) //Tonica en el bajo
		coste.AddCost(Coste::Bajo_Sin_Tonica);
	
	
	return coste;
}