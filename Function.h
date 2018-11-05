//
// Function.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy obs³ugi funkcji aktywacji.
//

#pragma once

#define FUNCTION_ERROR   -1
#define FUNCTION_LINEAR   0
#define FUNCTION_SIGMOID  1
#define FUNCTION_TANH     2
#define FUNCTION_SINUS    3
#define FUNCTION_COSINUS  4

#define FUNCTION_DEFAULT  FUNCTION_SIGMOID

typedef float (*FUNCTION)(float, float);

typedef struct tagACTFUNCTION {
	int         Id;
	const char *Name;
	float       False;
	float       True;
	FUNCTION    Function;
	FUNCTION    Derivative;
} ACTFUNCTION, *LPACTFUNCTION;

typedef const ACTFUNCTION* LPCACTFUNCTION;

LPCACTFUNCTION GetFunction(int Id);
