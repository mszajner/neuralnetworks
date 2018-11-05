//
// Net.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Implemetnacja klasy CNet.
//

#include "stdafx.h"
#include <math.h>
#include "Net.h"

// CNet

IMPLEMENT_SERIAL(CNet, CObject, 1)

CNet::CNet()
	: Alfa(0.6f)
	, Beta(3.0f)
	, Eta(0.2f)
	, Erms(0.0f)
	, Function(NULL)
	, Width(0)
	, Height(0)
	, Rms(0.0f)
	, RmsCounter(0)
{
	// clear layers
	Layers.SetSize(0);

	// clear inputs 
	Inputs.SetSize(0);

	// clear outputs 
	Outputs.SetSize(0);

	// clear categories
	Categories.SetSize(0);

	// set default function - sigmoid
	SetFunction(FUNCTION_DEFAULT);
}

CNet::~CNet()
{
	// clear categories
	Categories.SetSize(0);

	// clear outputs
	Outputs.SetSize(0);

	// clear inputs
	Inputs.SetSize(0);

	// clear layers
	Layers.SetSize(0);
}

// CNet member functions

void CNet::Serialize(CArchive& Archive) throw(...)
{
	char Buffer[4];
	const char Header[4] = { 'A', 'N', 'N', '1' };

	// serialize CObject class
	CObject::Serialize(Archive);

	if (Archive.IsStoring())
	{
		// store file header
		Archive.Write(Header, 4);

		// store alfa
		Archive << Alfa;
		
		// store beta
		Archive << Beta;
		
		// store eta
		Archive << Eta;
		
		// store function id
		Archive << (Function ? Function->Id : FUNCTION_DEFAULT);

		// store layers number
		Archive << (int)Layers.GetSize();

		// store inputs number
		Archive << (int)Inputs.GetSize();
		
		// store outputs number
		Archive << (int)Outputs.GetSize();

		// store width
		Archive << Width;

		// store height
		Archive << Height;
	}
	else
	{
		int Count;

		// read file header
		Archive.Read(Buffer, 4);

		// compare file header with orginal 
		if (strncmp(Buffer, Header, 4) != 0)
		{
			// raise exception
			throw NET_ERROR_FILE_HEADER;
		}

		// read alfa
		Archive >> Alfa;
		
		// read beta
		Archive >> Beta;
		
		// read eta
		Archive >> Eta;
		
		// read function id
		Archive >> Count;

		// set function
		SetFunction(Count);

		// read layers number
		Archive >> Count;

		// set layers number
		Layers.SetSize(Count);

		// read inputs number
		Archive >> Count;

		// set inputs number
		Inputs.SetSize(Count);

		// read outputs number
		Archive >> Count;

		// set outputs number
		Outputs.SetSize(Count);

		// read width
		Archive >> Width;

		// read height
		Archive >> Height;
	}

	// serialize categories
	Categories.Serialize(Archive);

	// serialize all layers
	for (int i = 0; i < Layers.GetSize(); i++)
		// serialize i-layer
		Layers[i].Serialize(Archive);
}

void CNet::CalculateDeltas(void)
{
	// check layers
	ASSERT(Layers.GetSize() > 0);

	// get output layer
	CLayer &Layer = Layers[Layers.GetUpperBound()];

	// calculate delta for output layer
	for (int i = 0; i < Layer.GetNeurons(); i++)
	{
		// get i-neuron
		CNeuron &Neuron = Layer.Neurons[i];

		// calculate delta
		Neuron.Delta = Function->Derivative(Neuron.Sum, Beta) 
			* (Outputs[i] - Neuron.Output);

		// calculate rms error for one vector
		Rms += (Outputs[i] - Neuron.Output)*(Outputs[i] - Neuron.Output);
	}

	// calculate delta for inner layers (from last to first)
	for (i = (int)Layers.GetUpperBound() - 1; i >= 0; i--)
	{
		// get i-layer
		CLayer &Layer = Layers[i];

		// get i+1-layer (next)
		CLayer &LayerNext = Layers[i+1];

		// for each neuron in i-layer
		for (int j = 0; j < Layer.GetNeurons(); j++)
		{
			// get j-neuron
			CNeuron &Neuron = Layer.Neurons[j];

			// clear neuron's previous delta
			Neuron.Delta = 0.0f;

			// for each neuron in next layer
			for (int k = 0; k < LayerNext.GetNeurons(); k++)
			{
				// get k-neuron from next layer
				CNeuron &NeuronNext = LayerNext.Neurons[k];
	
				// calculate delta for i-layer j-neuron
				Neuron.Delta += NeuronNext.Delta * NeuronNext.Weights[j];
			}
		}
	}
}

void CNet::CorrectWeights(void)
{
	// for each layer (from last to first)
	for (int i = (int)Layers.GetUpperBound(); i >= 0; i--)
	{
		// get i-layer
		CLayer &Layer = Layers[i];

		// for each neuron
		for (int j = 0; j < Layer.GetNeurons(); j++)
		{
			// get j-neuron
			CNeuron &Neuron = Layer.Neurons[j];

			// for each input
			for (int k = 0; k < Neuron.GetInputs(); k++)
			{
				// calculate delta weight
				Neuron.DeltaWeights[k] = Eta * Neuron.Delta *
					((i==0) ? Inputs[k] : Layers[i-1].Neurons[k].Output)
					 + Alfa * Neuron.DeltaWeights[k];

				// apply delta weight
				Neuron.Weights[k] += Neuron.DeltaWeights[k];
			}
		}
	}
}

void CNet::Random(void)
{
	for (int i = 0; i < Layers.GetCount(); i++)
		Layers[i].Random();
}

void CNet::Execute(void)
{
	// for each layer
	for (int i = 0; i < Layers.GetSize(); i++)
	{
		// get i-layer
		CLayer &Layer = Layers[i];

		// for each neuron
		for (int j = 0; j < Layer.GetNeurons(); j++)
		{
			// get j-neuron
			CNeuron &Neuron = Layer.Neurons[j];

			// clear neuron sum
			Neuron.Sum = 0.0f;

			// for each input
			for (int k = 0; k < Neuron.GetInputs(); k++)
			{
				// calculate sum
				Neuron.Sum += ((i == 0) ? Inputs[k] : 
					Layers[i-1].Neurons[k].Output) * Neuron.Weights[k];
			}

			// calculate output
			Neuron.Output = Function->Function(Neuron.Sum, Beta);
		}
	}
}

CString CNet::GetOutput(bool Max)
{
	float Output;
	int Index = 0;

	// check if any layer exists
	ASSERT(Layers.GetSize() > 0);

	// get last layer
	CLayer &Layer = Layers[Layers.GetUpperBound()];

	// check if any neurons in last layer
	ASSERT(Layer.Neurons.GetSize() > 0);

	// get output of first neuron
	Output = Layer.Neurons[0].Output;

	// for each neuron in last layer
	for (int i = 0; i < Layer.GetNeurons(); i++)
	{
		// check if i-output is greater than previous
		if (Layer.Neurons[i].Output > Output)
		{
			// set greater output
			Output = Layer.Neurons[i].Output;

			// get index of output
			Index = i;
		}
	}

	// return max categories
	return Categories[Index];
}

bool CNet::Compare(float Precision)
{
	// check if any layer exists
	ASSERT(Layers.GetSize() > 0);

	// get last layer
	CLayer &Layer = Layers[Layers.GetUpperBound()];

	// for each neuron in last layer
	for (int i = 0; i < Layer.GetNeurons(); i++)
	{
		// get output from i-neuron
		float Output = Layer.Neurons[i].Output;

		// get requested value
		float Requested = Outputs[i];

		// check if output value is in requested and precision bound
		if ((Output >= (Precision + Requested)) || (Output <= (Requested - Precision)))
			// return false -- one of neurons output does not match
			return false;
	}

	// return true -- all neurons ok
	return true;
}

int CNet::FindActiveOutput(void)
{
	// check if any layer exists
	ASSERT(Layers.GetSize() > 0);

	// get last layer
	CLayer &Layer = Layers[Layers.GetUpperBound()];

	// check if any layer exists
	ASSERT(Layer.Neurons.GetSize() > 0);

	int Index = 0;

	float Max = Layer.Neurons[Index].Output;

	// for each neuron in last layer
	for (int i = 1; i < Layer.GetNeurons(); i++)
		if (Max < Layer.Neurons[i].Output)
		{
			Index = i;
			Max = Layer.Neurons[i].Output;

		}

	return Index;
}

void CNet::Teach(void)
{
	// check if any layer exists
	ASSERT(Layers.GetSize() > 0);

	// execute data through the net
	Execute();

	// calculate delta (error) for each neuron
	CalculateDeltas();

	// correct weights for each neuron
	CorrectWeights();

	// rms counter
	RmsCounter++;
}

float CNet::CalcRms(void)
{
	float Rms = 0.0f;

	// check layers
	ASSERT(Layers.GetSize() > 0);

	// get output layer
	CLayer &Layer = Layers[Layers.GetUpperBound()];

	// calculate delta for output layer
	for (int i = 0; i < Layer.GetNeurons(); i++)
	{
		// get i-neuron
		CNeuron &Neuron = Layer.Neurons[i];

		// calculate delta
		Neuron.Delta = Function->Derivative(Neuron.Sum, Beta) 
			* (Outputs[i] - Neuron.Output);

		// calculate rms error for one vector
		Rms += (Outputs[i] - Neuron.Output)*(Outputs[i] - Neuron.Output);
	}

	return Rms / (float)Layer.GetNeurons();
}

float CNet::CalcErms(void)
{
	// clear erms error
	Erms = 0.0f;

	if (RmsCounter > 0)
	{
		// calculate error
		Erms = sqrt(Rms / (RmsCounter * GetOutputs()));

		// clear rms error
		Rms = 0.0f;

		// zero counter
		RmsCounter = 0;
	}

	// return erms
	return Erms;
}

int CNet::Load(CString FileName)
{
	CFile File;
	int nRetCode = NET_OK;

	// try to open file
	if (!File.Open(FileName, CFile::modeRead))
		// return error code
		return NET_ERROR_FILE_OPEN;

	// create archive object
	CArchive Archive(&File, CArchive::load);

	try
	{
		// serialize this class
		Serialize(Archive);
	}
	catch(int Error)
	{
		// return error
		nRetCode = Error;
	}

	// close archive
	Archive.Close();

	// close file
	File.Close();

	// return error code
	return nRetCode;
}

int CNet::Save(CString FileName)
{
	CFile File;
	int nRetCode = NET_OK;

	// try to create/open for writing file
	if (!File.Open(FileName, CFile::modeWrite | CFile::modeCreate))
		// return error code
		return NET_ERROR_FILE_CREATE;

	// create archive object
	CArchive Archive(&File, CArchive::store);

	try
	{
		// serialize this class
		Serialize(Archive);
	}
	catch(int Error)
	{
		// return error
		nRetCode = Error;
	}

	// close archive
	Archive.Close();

	// close file
	File.Close();

	// return error code
	return nRetCode;
}

int CNet::LoadVector(CVector *Vector)
{
	if (Vector->GetLength() != Inputs.GetCount())
		return NET_ERROR;

	memcpy(Inputs.GetData(), Vector->GetData(), 
		sizeof(float) * Inputs.GetCount());

	for (int i = 0; i < GetOutputs(); i++)
		Outputs[i] = (i == Vector->GetOutput()) ? 
			Function->True : Function->False;

	return NET_OK;
}

void CNet::SetFunction(int Id)
{
	// get function
	Function = ::GetFunction(Id);
	
	// if function doesn't exists
	if (Function == NULL)
		// get default function
		Function = ::GetFunction(FUNCTION_DEFAULT);
}

void CNet::SetInputs(int Value)
{
	// set inputs vector size
	Inputs.SetSize(Value);

	// if there is some layers
	if (Layers.GetCount() > 0)
		// set inputs number of first layer
		Layers[0].SetInputs(Value);
}

void CNet::SetOutputs(int Value)
{
	// set outputs vector size
	Outputs.SetSize(Value);

	// set categories vector size
	Categories.SetSize(Value);

	// if there is some layers
	if (Layers.GetCount() > 0)
		// set neurons number of last layer
		Layers[Layers.GetUpperBound()].SetNeurons(Value);
}
