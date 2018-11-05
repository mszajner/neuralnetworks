//
// Function.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Funkcje do obs³ugi wyboru i przeszukiwania funkcji aktywacji.
//

#include "stdafx.h"
#include <math.h>
#include "Function.h"

float lin(float x, float beta)
{
	return x*beta;
}

float lin_poch(float x, float beta)
{
	return beta;
}

float sigm(float x, float beta)
{
	return 1.0f/(1.0f+exp(-beta*x));
}

float sigm_poch(float x, float beta)
{
	return beta*(1.0f-sigm(x,beta))*sigm(x,beta);
}

float tgh(float x, float beta)
{
	return tanh(x*beta);
}

float tgh_poch(float x, float beta)
{
	return beta*(1.0f-tgh(x,beta)*tgh(x,beta));
}

float sinus(float x, float beta)
{
	return sin(x*beta);
}

float sinus_poch(float x, float beta)
{
	return beta*cos(x*beta);
}

float cosinus(float x, float beta)
{
	return cos(x*beta);
}

float cosinus_poch(float x, float beta)
{
	return -beta*sin(x*beta);
}

LPCACTFUNCTION GetFunction(int Id)
{
	#define MAX_FUNCS 5

	static ACTFUNCTION Funcs[MAX_FUNCS] = {
		{ FUNCTION_LINEAR,  "Liniowa",               0.0f, 1.0f, &lin,     &lin_poch     },
		{ FUNCTION_SIGMOID, "Sigmoidalna",           0.0f, 1.0f, &sigm,    &sigm_poch    },
		{ FUNCTION_TANH,    "Tangens hiperboliczny", 0.0f, 1.0f, &tgh,     &tgh_poch     },
		{ FUNCTION_SINUS,   "Sinus",                 0.0f, 1.0f, &sinus,   &sinus_poch   },
		{ FUNCTION_COSINUS, "Cosinus",               0.0f, 1.0f, &cosinus, &cosinus_poch },
	};

	for (int i = 0; i < MAX_FUNCS; i++)
		if (Funcs[i].Id == Id)
			return (LPCACTFUNCTION)&Funcs[i];

	return NULL;
}
