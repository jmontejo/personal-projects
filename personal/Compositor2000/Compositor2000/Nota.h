/*
 *  Nota.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 *  ID=0   corresponde a C0
 *  ID=108 corresponde a C9
 *  ID=-1  es nota inválida
 */

#ifndef NOTA_H
#define NOTA_H
#include <string>

#define ID_MIN 0
#define ID_MAX 108

enum Nota_nombre{ 
	C  = 0, Bs = 0,
	Cs = 1, Db = 1,
	D  = 2,
	Ds = 3, Eb = 3,
	E  = 4, Fb = 4,
	F  = 5, Es = 5,
	Fs = 6, Gb = 6,
	G  = 7,
	Gs = 8, Ab = 8,
	A  = 9,
	As = 10, Bb = 10,
	B  = 11, Cb = 11 };


class Nota{

public:
	Nota();
	Nota(int ID);
	Nota(Nota_nombre nom, int octava);
	~Nota();
	std::string toString();

	int ID;
	Nota_nombre nombre;
	int octava;
	int Distancia(Nota);
};

#endif