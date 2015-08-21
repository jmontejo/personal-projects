/*
 *  Instrumento.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */


#ifndef INSTRUMENTO_H
#define INSTRUMENTO_H
#include "Nota.h"
#include "Acorde.h"
#include <string>
#include <vector>

const Nota bajo_low(D,1); 
const Nota bajo_up (C,3); 
const Nota baritono_low(G,1); 
const Nota baritono_up (F,3); 
const Nota tenor_low(C,2); 
const Nota tenor_up (A,3); 
const Nota contraalto_low(G,2); 
const Nota contraalto_up (D,4);
const Nota mezzo_low(A,2); 
const Nota mezzo_up (A,4); 
const Nota soprano_low(C,3); 
const Nota soprano_up (A,5); 

class Instrumento{
	
public:
	Instrumento(std::string ins);
	~Instrumento();
	std::vector<Nota> getNotasPermitidas(Acorde a);
	
private:
	Nota max;
	Nota min;
	int registro;
	
};

#endif