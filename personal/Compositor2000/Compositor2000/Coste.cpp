/*
 *  Coste.cpp
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#include "Coste.h"
#include <iostream>
#include <cmath>

Coste::Coste(){
	valor = 0;
	tipo = Cero;
}

Coste::Coste(int v){
	valor = v;
}

void Coste::AddCost(tipo_coste tc){	
	tipo = tipo | tc;
	valor += getCost(tc);
}

int Coste::getCost(tipo_coste tc){

	switch (tc) {
		case Sensible_Fundamental_Duplicada:
			return Sensible_Fundamental_Duplicada_Cost;
			break;
		case Dominante_Sensible_Duplicada:
			return Dominante_Sensible_Duplicada_Cost;
			break;
		case Mediante_Quinta_Duplicada:
			return Mediante_Quinta_Duplicada_Cost;
			break;
		case Intervalo_MasQueOctava:
			return Intervalo_MasQueOctava_Cost;
			break;
		case Unisono:
			return Unisono_Cost;
			break;
		case Superposicion:
			return Superposicion_Cost;
			break;
		case Bajo_Sin_Tonica:
			return Bajo_Sin_Tonica_Cost;
			break;
		case Acorde_Sin_Tercera:
			return Acorde_Sin_Tercera_Cost;
			break;
		case Acorde_Sin_Quinta:
			return Acorde_Sin_Quinta_Cost;
			break;
		case Quintas_Paralelas:
			return Quintas_Paralelas_Cost;
			break;
		case Octavas_Paralelas:
			return Octavas_Paralelas_Cost;
			break;
		case Quintas_Directas:
			return Quintas_Directas_Cost;
			break;
		case Octavas_Directas:
			return Octavas_Directas_Cost;
			break;
		case Sensible_Sin_Resolver:
			return Sensible_Sin_Resolver_Cost;
			break;
		case Salto_Bajo_MayorQueQuinta:
			return Salto_Bajo_MayorQueQuinta_Cost;
			break;
		case No_Mantener_Nota:
			return No_Mantener_Nota_Cost;
			break;
		case Mismo_Movimiento_QueElBajo:
			return Mismo_Movimiento_QueElBajo_Cost;
			break;
		default:
			std::cout << "Coste sin mapear!!!: "<< tc << std::endl;
			exit(1);
			return 0;
	}

}

void Coste::mergeCost(Coste c){
	tipo = tipo | c.tipo;
	valor += c.valor;
}

std::string Coste::toString(){
	char c[20];
	sprintf(c,"%d:%d",tipo,valor);
	std::string s(c);
	return s;
}