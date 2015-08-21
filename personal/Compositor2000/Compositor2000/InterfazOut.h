/*
 *  InterfazOut.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "Solucion.h"

class InterfazOut{

public:
	InterfazOut(std::vector<Solucion> soluciones);
	void Print();
	void PrintSolucion(Solucion s);
	std::vector<Solucion> soluciones;
};