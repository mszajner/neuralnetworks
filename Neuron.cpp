//
// Neuron.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Implementacja klasy CNeuron.
//

#include "stdafx.h"
#include "Neuron.h"

// CNeuron

IMPLEMENT_SERIAL(CNeuron, CObject, 1)

CNeuron::CNeuron()
	: Delta(0)
	, Output(0)
	, Sum(0)
{
	// clear weights
	SetInputs(0);

	// clear delta weights
	DeltaWeights.SetSize(0);
}

CNeuron::~CNeuron()
{
	// clear weights
	SetInputs(0);

	// clear delta weights
	DeltaWeights.SetSize(0);
}


// CNeuron member functions

void CNeuron::Serialize(CArchive& Archive)
{
	// serialize CObject class
	CObject::Serialize(Archive);

	// serialize weights
	Weights.Serialize(Archive);
}

void CNeuron::Random(void)
{
	// for each input
	for (int i = 0; i < (int)Weights.GetSize(); i++)
	{
		// random i-weight
		Weights[i] = (((rand() % 1000000L) / 1700.0f) - 9.8f)*0.0015f;

		// clear last delta of i-weight
		DeltaWeights[i] = 0.0f;

		// if i-weight is zero
		if(Weights[i] == 0.0) 
			// set default value
			Weights[i] = 0.01492f;
	}
}

CNeuron& CNeuron::operator=(const CNeuron &Neuron)
{
	// copy delta weights
	DeltaWeights.Copy(Neuron.DeltaWeights);

	// copy weights
	Weights.Copy(Neuron.Weights);

	// copy delta
	Delta = Neuron.Delta;

	// copy sum
	Sum = Neuron.Sum;

	// copy output
	Output = Neuron.Output;

	// return pointer to this class
	return (*this);
}
