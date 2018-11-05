//
// Neuron.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy klasy neuronu.
//

#pragma once

#include "afxtempl.h"

// define array of float
typedef CArray<float,float&> CFloatArray;

// CNeuron

class CNeuron : public CObject
{
public:
	CNeuron(); // default constructor
	virtual ~CNeuron(); // destructor

protected:
	DECLARE_SERIAL(CNeuron)

public:
	virtual void Serialize(CArchive& Archive); // serialize function
	void Random(void); // random weights
	CNeuron& operator=(const CNeuron &Neuron); // copy operator

public:
	float Delta; // error
	float Sum; // membrane's potential
	float Output; // output
	CFloatArray Weights; // weights
	CFloatArray DeltaWeights; // last change of weights

public:
	int GetInputs(void) { // get inputs number
		return (int)Weights.GetSize();
	}

	void SetInputs(int Value) { // set inputs number
		Weights.SetSize(Value);
		DeltaWeights.SetSize(Value);
	}
};
