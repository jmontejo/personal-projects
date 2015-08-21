/*
 *  Acorde.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "Acorde.h"
#include <iostream>

Acorde::Acorde(){
	tipo = invalido;
}

Acorde::Acorde(std::string acorde){

	original = acorde;
	if(acorde.size()<1)
		tipo = invalido;
	else{
		tipo=0;
		char nota = acorde.at(0);
		bool sos = false;
		bool bem = false;
		bool men = false;
		if(acorde.size()>1){
		if (acorde.at(1)=='b')
			bem = true;
		else if(acorde.at(1)=='#')
			sos = true;
		else if(acorde.at(1)=='m')
			men = true;
		}
		if((bem || sos) && acorde.size()>2 && acorde.at(2)=='m')
			men = true;

		if(nota == 'C'){
			nombre = C;
		if(bem)
			nombre = Cb;
		if(sos)
			nombre = Cs;
		}
		else if(nota == 'D'){
			nombre = D;
			if(bem)
				nombre = Db;
			if(sos)
				nombre = Ds;
		}
		else if(nota == 'E'){
			nombre = E;
			if(bem)
				nombre = Eb;
			if(sos)
				nombre = Es;
		}
		else if(nota == 'F'){
			nombre = F;
			if(bem)
				nombre = Fb;
			if(sos)
				nombre = Fs;
		}
		else if(nota == 'G'){
			nombre = G;
			if(bem)
				nombre = Gb;
			if(sos)
				nombre = Gs;
		}
		else if(nota == 'A'){
			nombre = A;
			if(bem)
				nombre = Ab;
			if(sos)
				nombre = As;
		}
		else if(nota == 'B'){
			nombre = B;
			if(bem)
				nombre = Bb;
			if(sos)
				nombre = Bs;
		}
		else{
			tipo = invalido;
		}

		if(tipo!=invalido){
			if(men)
				tipo += menor;
	if (acorde.find("dim") != std::string::npos)
		tipo += disminuido;
	if (acorde.find("aug") != std::string::npos)
		tipo += aumentado;
	if (acorde.find('7') != std::string::npos)
		tipo += septima;	
	if ((tipo & disminuido) && (tipo & aumentado))
		tipo = invalido;
	}
		
		tonica = nombre;
		if(tipo & menor)
			tercera = Nota_nombre((nombre+3)%12);
		else
			tercera = Nota_nombre((nombre+4)%12);
		if(tipo & disminuido)
			quinta = Nota_nombre((nombre+6)%12);
		else if (tipo & aumentado)
			quinta = Nota_nombre((nombre+8)%12);
		else 
			quinta = Nota_nombre((nombre+7)%12);
		
	}
	
}

void Acorde::fijarTonica(Acorde tonica){
	
	int o = original.at(0);
	int t = tonica.original.at(0);
	int diff = o-t+1;
	if(diff <= 0) diff+=7;	
	grado = diff;
	std::cout << original <<" Grado " << diff << std::endl;
}

std::string Acorde::toString(){
	return std::string("toString Acorde"); //std::cout << "Print Acorde: " << original << std::endl;
}

Acorde::~Acorde(){}
