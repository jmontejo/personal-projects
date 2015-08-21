/*
 *  Compas.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/25/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#ifndef COMPAS_H
#define COMPAS_H

#include <vector>
#include "Acorde.h"
#include "Instrumento.h"

class Compas{
public:
	Compas(Acorde a, std::vector<Instrumento>);
	Acorde getAcorde();
	std::vector<Instrumento> getInstrumentos();
	
private:
	Acorde acorde;
	std::vector<Instrumento> instrumentos;
	
};

#endif