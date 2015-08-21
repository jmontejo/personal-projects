/*
 *  Posicion.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */
#ifndef POSICION_H
#define POSICION_H
#include <vector>
#include <string>
#include "Nota.h"
#include "Coste.h"
#include "Acorde.h"
class Posicion{
	
public:
	Posicion(std::vector<Nota> n, Acorde a);
	std::vector<Nota> notas;
	Coste coste;
	Acorde acorde;
	int posID;
	std::string toString();
	Coste calcularCoste(std::vector<Nota> n, Acorde a);
	
	
	bool operator<(const Posicion& x)
	{
		return this->coste.valor < x.coste.valor;
	}

};

#endif POSICION_H