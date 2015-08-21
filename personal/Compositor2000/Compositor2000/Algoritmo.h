/*
 *  Algoritmo.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/27/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */
#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "Posicion.h"
#include "Solucion.h"
#include "Coste.h"
#include <vector>
#include <list>

class Algoritmo{

public:
	std::vector<Solucion> resolver(	std::vector<std::list <Posicion> > matrizPos, 
								   std::vector<std::vector<std::vector <Coste> > >matrizCostes);

};

#endif