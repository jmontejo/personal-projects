/*
 *  CompositorV.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */


#ifndef COMPOSITORV_H
#define COMPOSITORV_H
#include <vector>
#include <list>
#include "Instrumento.h"
#include "Posicion.h"
#include "Compas.h"
#include "Monitor.h"

class CompositorV{
	
public:
	CompositorV(int m);
	std::list<Posicion> compose(Compas c);
	int maxOptions;
	
private:
	void setNote(int ins, int note, const std::vector< std::vector<Nota> > &notas_permitidas, std::vector<Nota> tofill, std::vector< std::vector<Nota> > &tocheck);
	bool check(std::vector<Nota> tofill);
};

#endif