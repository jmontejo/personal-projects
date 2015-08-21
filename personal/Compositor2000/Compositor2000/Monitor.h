/*
 *  Monitor.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#ifndef MONITOR_H
#define MONITOR_H

#include <vector>
#include "Solucion.h"

class Monitor{

public:
	Monitor(int m);
	~Monitor();
	void Analyze(std::vector<Solucion> soluciones);
private:
	int maxi;
};

#endif