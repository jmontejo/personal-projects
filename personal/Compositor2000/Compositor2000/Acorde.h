/*
 *  Acorde.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */


#ifndef Acorde_H
#define Acorde_H

#include "Nota.h"
#include <string>
#include <vector>

enum tipo_acorde{invalido=-1, mayor=0, menor=1, septima=2, aumentado=4, disminuido=8};

class Acorde{

public:
	Acorde(std::string acorde);
	Acorde();
	~Acorde();
	std::string toString();
	void fijarTonica(Acorde a);
	
	std::string original;
	int grado;
	Nota_nombre nombre;
	Nota_nombre tonica;
	Nota_nombre tercera;
	Nota_nombre quinta;
	std::vector<Nota_nombre> adicionales;
	unsigned int tipo;
};

#endif