/*
 *  CompositorH.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */


#ifndef COMPOSITORH_H
#define COMPOSITORH_H
#include <vector>
#include <list>
#include "Coste.h"
#include "Posicion.h"
#include "Monitor.h"

class CompositorH{
	
public:
	CompositorH(Acorde t);
	~CompositorH();
	std::vector<std::vector<std::vector <Coste> > >compose(	std::vector<std::list <Posicion> > matrizPos);
	Coste getCost(Posicion a, Posicion b);
	
private:
	Acorde tonica;

};

#endif