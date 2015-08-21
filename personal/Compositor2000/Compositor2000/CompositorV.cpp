/*
 *  CompositorV.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "CompositorV.h"
#include <iostream>


CompositorV::CompositorV(int m){
	maxOptions = m;
}

std::list<Posicion> CompositorV::compose(Compas c){
	std::vector< std::vector<Nota> > notas_permitidas;
	std::vector<Instrumento> instrumentos = c.getInstrumentos();
	Acorde acorde = c.getAcorde();
	for(int i=0; i<instrumentos.size(); i++){
		notas_permitidas.push_back(instrumentos.at(i).getNotasPermitidas(acorde));
	}
	
	std::vector< std::vector<Nota> >tocheck;
	std::vector<Nota> tofill(instrumentos.size());
	int perm=1;
	for(int i=0;i<notas_permitidas.size();i++)
		perm*= notas_permitidas.at(i).size();
	setNote(0,0,notas_permitidas,tofill,tocheck);
	std::list<Posicion> best;
	int threshold = 9999;
	for(int i=0;i<tocheck.size();i++){
		Posicion temp(tocheck.at(i),acorde);
		if(temp.coste.valor <= threshold)
			best.push_back(temp);
		if(best.size() > maxOptions){
			best.sort();
			best.pop_back();
			threshold = best.back().coste.valor;
		}
	}
	
//	for (std::list<Posicion>::iterator it=best.begin(); it!=best.end(); ++it){
//		(*it).Print();
//		std::cout << std::endl;
//	}
	
	return best;		

}

void CompositorV::setNote(int ins, int note, const std::vector< std::vector<Nota> > &notas_permitidas, std::vector<Nota> tofill, std::vector< std::vector<Nota> > &tocheck){
	while(note < notas_permitidas.at(ins).size()){
		tofill.at(ins) = notas_permitidas.at(ins).at(note);
		if (ins != notas_permitidas.size()-1)
			setNote(ins+1,0,notas_permitidas, tofill,tocheck);
		else {
			if(check(tofill))
				tocheck.push_back(std::vector<Nota>(tofill));
		}
		note++;
	}
}

bool CompositorV::check(std::vector<Nota> tofill){
	int prev = -1;
	for(int i=0; i< tofill.size(); i++){
		if(tofill.at(i).ID < prev)
			return false;
		prev = tofill.at(i).ID;
	}
	return true;
}