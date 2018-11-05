//
// Layer.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy klasy warstwy sieci.
//

#pragma once

#include "Neuron.h"

// define array of neuron
typedef CArray<CNeuron,CNeuron&> CNeuronArray;

// CLayer

class CLayer : public CObject
{
public:
	// default constructor
	CLayer();

	// destructor
	virtual ~CLayer();

protected:
	// serializable
	DECLARE_SERIAL(CLayer)

public:
	// serialize function
	virtual void Serialize(CArchive& Archive);

public:
	// neuron array
	CNeuronArray Neurons;

public:
	void Random(void);

	// get number of inputs
	int GetInputs(void) {
		return (Neurons.GetSize() > 0) ? Neurons[0].GetInputs() : 0;
	}

	// set number of inputs for each neuron
	void SetInputs(int Value, bool Random = true);

	// get number of neurons
	int GetNeurons(void) {
		return (int)Neurons.GetSize();
	}

	// set number of neurons
	void SetNeurons(int Value) {
		Neurons.SetSize(Value);
	}
};
