/*
 *  Partitura.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#ifndef PARTITURA_H
#define PARTITURA_H

#include <string>
#include <vector>
#include <list>
#include "Monitor.h"
#include "Acorde.h"
#include "Coste.h"
#include "Instrumento.h"
#include "Posicion.h"
#include "Solucion.h"

class Partitura{
public:
	Partitura (std::string acordes, std::string instrumentos, int m);
    ~Partitura ();
	void Resolver();
	std::vector<Solucion> GetSoluciones();

private:
	int numCompases;
	int max;
	std::vector<Acorde> acordes;
	std::vector<Acorde> ParseAcordes(std::string acord );
	int numPistas;
	std::vector<Instrumento> pistas;
	std::vector<Instrumento> ParseInstrumentos(std::string instrumentos);
	std::vector<std::list <Posicion> > matrizPos;
	std::vector<std::vector<std::vector <Coste> > >matrizCostes;
	std::vector<Solucion> soluciones;
	
};

#endif