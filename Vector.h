#pragma once

#include "afxtempl.h"
#include "atlimage.h"

class CVector : public CObject
{
private:
	ULONG64 m_Offset; // vector offset in file

	struct {
		int   Deleted;
		int   Testing;
		int   Output;
		char  Name[32];
	} m_Header; // vector header
	
	int m_Length; // vector data length (!=0 -- data allocated)
	float *m_Data; // vector data array

public:
	CVector(); // default constructor
	CVector(int Length); // constructor with specified vector size
	virtual ~CVector();	// destructor

protected:
	int AllocData(int Length); // set vector length

public:
	int ReadHeader(CFile &File, ULONG64 Offset = 0); // read header from file
	int WriteHeader(CFile &File, ULONG64 Offset = 0); // write header to file
	
	int ReadData(CFile &File, int Length = 0); // read data from file
	int WriteData(CFile &File); // write data to file

	int ClearData(void); // clear vector data (frees up memory)
	int ClearHeader(void); // clear vector header

public:
	int IsDeleted(void); // get vector deleted flag
	void SetDeleted(int Value); // set vector deleted flag

	int IsTesting(void); // set vector for testing flag
	void SetTesting(int Value); // set vector for testing flag

	int GetOutput(void); // get requested output
	void SetOutput(int Output); // set requested output

	void GetName(CString &Name); // get vector name
	void SetName(const CString &Name); // set vector name

	int GetLength(void); // get vector length
	float* GetData(void); // get vector array

public:
	int Import(CString FileName, int Monochrome, int Black, int Bias, 
		int Width, int Height); // import vector from file (txt, bmp, jpeg, gif, png, etc.)

public:
	CVector& operator=(CVector &Vector); // operator =
};

// error codes
#define VECTOR_ERROR_IMAGE_CREATE -7
#define VECTOR_ERROR_IMAGE_LOAD   -6
#define VECTOR_ERROR_SEEK         -5
#define VECTOR_ERROR_IMPORT       -4
#define VECTOR_ERROR_DATA         -3
#define VECTOR_ERROR_HEADER       -2
#define VECTOR_ERROR              -1
#define VECTOR_OK                  0
