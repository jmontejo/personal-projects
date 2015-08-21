/*
 *  Nota.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "Nota.h"
#include <iostream>

Nota::Nota(){
	Nota(-1);
}

Nota::~Nota(){}


Nota::Nota(int id){
	if(id < ID_MIN || id > ID_MAX)
		ID = -1;
	else{
	ID = id;
	nombre = Nota_nombre(id%12);
	octava = id/12;
	}
}

Nota::Nota(Nota_nombre nom, int o){
	nombre = nom;
	octava = o;
	ID = octava*12+nom;
	if(ID < ID_MIN || ID > ID_MAX)
		ID = -1;
}

int Nota::Distancia(Nota otra){
	return otra.ID - ID;
}

std::string Nota::toString(){
	char c[20];
	sprintf(c,"%d",ID);
	std::string s(c);
	return s;
}