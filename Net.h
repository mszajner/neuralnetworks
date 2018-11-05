//
// Net.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy klasy sieci.
//

#pragma once

#include "Layer.h"
#include "Function.h"
#include "Vector.h"

// define array of layer
typedef CArray<CLayer,CLayer&> CLayerArray;

// CNet

class CNet : public CObject
{
public:
	// default constructor
	CNet();

	// destructor
	virtual ~CNet();

protected:
	// serializable
	DECLARE_SERIAL(CNet)

public:
	// serialize function
	virtual void Serialize(CArchive& Archive);

private:
	// activation function
	LPCACTFUNCTION Function;

public:
	// alfa
	float Alfa;

	// beta
	float Beta;

	// eta
	float Eta;

	// rms (for one vector)
	float Rms;

	// rms counter
	int RmsCounter;

	// erms (for epoch)
	float Erms;

	// width of image
	int Width;

	// height of image
	int Height;

	// array of layers
	CLayerArray Layers;

	// array of inputs
	CFloatArray Inputs;

	// array of outputs
	CFloatArray Outputs;

	// array of output categories
	CStringArray Categories;

protected:
	// calculate delta for each neuron
	void CalculateDeltas(void);

	// correct weights for each neuron
	void CorrectWeights(void);

public:
	// random weights of all neurons
	void Random(void);

	// execute signal from input to last layer
	void Execute(void);

	// get output with defined precision
	CString GetOutput(bool Max = true);

	// compare neurons output with output array
	bool Compare(float Precision);

	int FindActiveOutput(void);

    // teach net
	void Teach(void);

	float CalcRms(void);

	// calculate erms error
	float CalcErms(void);

	// load net from file
	int Load(CString FileName);

	// save net to file
	int Save(CString FileName);

	// load input to vector
	int LoadVector(CVector *Vector);

public:
	// get function id
	int GetFunction(void) {
		return Function ? Function->Id : FUNCTION_ERROR;
	}
	
	// set function
	void SetFunction(int Id);

	// get net inputs
	int GetInputs(void) {
		return (int)Inputs.GetSize();
	}

	// set net inputs
	void SetInputs(int Value);

	// get net outputs
	int GetOutputs(void) {
		return (int)Outputs.GetSize();
	}

	// set net outputs
	void SetOutputs(int Value);
};

// error codes
#define NET_ERROR_FILE_OPEN       -4
#define NET_ERROR_FILE_CREATE     -3
#define NET_ERROR_FILE_HEADER     -2
#define NET_ERROR                 -1
#define NET_OK                     0
