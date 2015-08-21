/*
 *  InterfazOut.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "InterfazOut.h"
#include <iostream>
#include <list>

InterfazOut::InterfazOut(std::vector<Solucion> s){
	soluciones = s;
}

void InterfazOut::Print(){
	std::list<Solucion> best;
	for(int i=0; i<soluciones.size();i++)
		best.push_back(soluciones.at(i));
	best.sort();
	std::cout << "-----------------------"<< std::endl;
	for(std::list<Solucion>::iterator it = best.begin(); it != best.end(); it++){
		PrintSolucion(*it);
	}
}

void InterfazOut::PrintSolucion(Solucion s){
	std::string out;
	std::cout << "Alternativas: "<< s.getNumAlternativas() << std::endl;
	for(int p=0;p<s.posiciones.size();p++){
		out += s.posiciones.at(p).toString();
		if(p<s.costes.size())
			out+= s.costes.at(p).toString()+" | ";
		//if(p<s.costes.size())
		//	std::cout << " cost " << s.costes.at(p).valor << " "  << std::flush;
	}
	out += s.coste.toString();
	std::cout << out << std::endl;
	//std::vector<Solucion> alts = s.getAlternativas();
	//for(int i=0;i<alts.size();i++)
	//	PrintSolucion(alts.at(i));
}