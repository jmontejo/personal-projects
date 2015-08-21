/*
 *  Solucion.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */
#ifndef SOLUCION_H
#define SOLUCION_H
#include "Posicion.h"
#include "Coste.h"

class Solucion{

public:
	Solucion();
	void addPosicion(Posicion a);
	void addPosicionIndex(int i);
	void addTransicion(Coste c);
	void addAlternativa(Solucion a);
	int getNumAlternativas();
	std::vector<Solucion> getAlternativas();
	std::vector<int> getPosicionIndexWorst();
	std::vector<Posicion> posiciones;
	std::vector<int> posicionesIndex;
	std::vector<Coste> costes;
	Coste coste;
	std::vector<Solucion> alternativas;
	
	bool operator<(const Solucion& x)
	{
		return this->coste.valor < x.coste.valor;
	}
	
	
};

#endif