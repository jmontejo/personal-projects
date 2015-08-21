/*
 *  Coste.h
 *  Compositor2000
 *
 *  Created by Javier Montejo on 3/24/12.
 *  Copyright 2012 IFAE. All rights reserved.
 *
 */

#ifndef COSTE_H
#define COSTE_H
#include <string>

class Coste{

public:	
	enum tipo_coste {
		Cero							= 0
		,Sensible_Fundamental_Duplicada	= 1 << 0
		,Dominante_Sensible_Duplicada	= 1 << 1
		,Mediante_Quinta_Duplicada		= 1 << 2
		,Intervalo_MasQueOctava			= 1 << 3
		,Unisono						= 1 << 4
		,Bajo_Sin_Tonica				= 1 << 5
		,Acorde_Sin_Tercera				= 1 << 6
		,Acorde_Sin_Quinta				= 1 << 7
		,Quintas_Paralelas				= 1 << 8
		,Octavas_Paralelas				= 1 << 9
		,No_Mantener_Nota				= 1 << 10
		,Salto_Bajo_MayorQueQuinta		= 1 << 11
		,Mismo_Movimiento_QueElBajo		= 1 << 12
		,Quintas_Directas				= 1 << 13
		,Octavas_Directas				= 1 << 14
		,Sensible_Sin_Resolver			= 1 << 15
		
		,Superposicion					= 1 << 17
		,Tipo_Limite					= 1 << 18
	};
	enum valor_coste {
		Sensible_Fundamental_Duplicada_Cost	= 100
		,Dominante_Sensible_Duplicada_Cost	= 80
		,Mediante_Quinta_Duplicada_Cost		= 2
		,Intervalo_MasQueOctava_Cost		= 60
		,Unisono_Cost						= 25
		,Bajo_Sin_Tonica_Cost				= 80
		,Acorde_Sin_Tercera_Cost			= 50
		,Acorde_Sin_Quinta_Cost				= 55
		,Quintas_Paralelas_Cost				= 70
		,Octavas_Paralelas_Cost				= 75
		,Quintas_Directas_Cost				= 45
		,Octavas_Directas_Cost				= 48
		,No_Mantener_Nota_Cost				= 30
		,Sensible_Sin_Resolver_Cost 		= 68
		,Salto_Bajo_MayorQueQuinta_Cost		= 65
		,Mismo_Movimiento_QueElBajo_Cost	= 15
		,Superposicion_Cost					= 85

	};
	Coste();
	Coste(int v);
	void AddCost(tipo_coste tc);
	void mergeCost(Coste c);
	std::string toString();
	int getCost(tipo_coste tc);
	int valor;
	int tipo;
};

#endif