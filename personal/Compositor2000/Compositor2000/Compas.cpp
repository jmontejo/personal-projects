/*
 *  Compas.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/25/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "Compas.h"

Compas::Compas(Acorde a, std::vector<Instrumento> i){
	acorde = a;
	instrumentos = i;
}

Acorde Compas::getAcorde(){ return acorde;}
std::vector<Instrumento> Compas::getInstrumentos(){ return instrumentos;}