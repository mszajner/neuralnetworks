//
// Layer.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Implementacja klasy CLayer.
//

#include "stdafx.h"
#include "Layer.h"

// CLayer

IMPLEMENT_SERIAL(CLayer, CObject, 1)

CLayer::CLayer()
{
	// clear neurons array
	Neurons.SetSize(0);
}

CLayer::~CLayer()
{
	// clear neurons array
	Neurons.SetSize(0);
}


// CLayer member functions

void CLayer::Serialize(CArchive& Archive)
{
	// serialize CObject class
	CObject::Serialize(Archive);

	// check if is storing
	if (Archive.IsStoring())
	{
		// store neurons number
		Archive << GetNeurons();

		// store inputs number
		Archive << GetInputs();
	}
	else
	{
		int Count;

		// read outputs number
		Archive >> Count;

		// set neurons number
		SetNeurons(Count);

		// read inputs number
		Archive >> Count;

		// set inputs number
		SetInputs(Count, false);
	}

	// serialize neurons
	for (int i = 0; i < Neurons.GetSize(); i++)
		// serialize i-neuron
		Neurons[i].Serialize(Archive);
}

void CLayer::Random(void)
{
	for (int i = 0; i < Neurons.GetCount(); i++)
		Neurons[i].Random();
}

void CLayer::SetInputs(int Value, bool Random)
{
	// for each neuron
	for (int i = 0; i < Neurons.GetSize(); i++)
	{
		// set number of inputs for i-neuron
		Neurons[i].SetInputs(Value);

		// randomize weights?
		if (Random)
			// randomize weights
			Neurons[i].Random();
	}
}
