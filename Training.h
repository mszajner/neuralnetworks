#pragma once

#include <windows.h>
#include "Vector.h"

class CTraining;

typedef int (__stdcall *TRAINING_EVENT)(CTraining *Training, void *Parameter);
typedef int (__stdcall *TRAINING_START)(CTraining *Training, int Vectors, void *Parameter);
typedef int (__stdcall *TRAINING_VECTOR)(CTraining *Training, CVector *Vector, void *Parameter);
typedef int (__stdcall *TRAINING_EPOCH)(CTraining *Training, int Epoch, void *Parameter);
typedef int (__stdcall *TRAINING_ERMS)(CTraining *Training, float Erms, void *Parameter);

// CTraining command target

class CTraining : public CObject
{
private:
	// net
	CNet *m_Net;

	// user parameter
	void *m_Parameter;

	// training associated file
	CFile m_File;

	// vectors array
	CArray<CVector,CVector&> m_Vectors;

	struct {
		bool    ChangeAlfa;   // change alfa after each epoch
		bool    ChangeEta;    // change eta after each epoch
		float   EndAlfa;      // end alfa value (after all epochs)
		float   EndEta;       // end eta value (after all epochs)
		int     Epochs;       // epochs number
		bool    BreakErms;    // break training if erms is below MaxErms
		float   MaxErms;      // max. erms error
		int     Width;        // width of image
		int     Height;       // height of image
		int     Repeats;      // each vector repeats
		char    Reserved[24]; // reserved (fill up to 64 bytes)
	} m_Header; // training header
	
	bool m_Running; // running flag
	bool m_Testing; // testing flag

	HANDLE m_Thread; // handle to thread
	DWORD m_ThreadId; // identifier of thread

// class construction & destruction
public:
	CTraining(); // default constructor
	virtual ~CTraining(); // destructor

// file managment routines
public:
	int Open(const char *FileName); // open file
	int Close(void); // close file
	int Load(void); // load training and vectors headers
	int Save(const char *FileName = NULL); // save training and modified vectors
	int Read(CVector *Vector = NULL); // read one vector (if Vector == NULL, reads all vectors)
	int Write(CVector *Vector = NULL); // write one vector (if Vector == NULL, writes all vectors)
	int ClearHeader(void); // clear training
	int ClearVectors(void); // clear vectors

// vectors managment routines
public:
	CVector* Next(int Output = -1, bool Reset = false); // enumerate next vector
	CVector* Add(void); // add vector to training
	int Delete(CVector *Vector); // delete vector from training

// flags
public:
	bool IsOpen(void); // get file open
	bool IsRunning(void); // get running flag
	bool IsTesting(void); // get testing flag

// training settings
public:
	bool GetChangeAlfa(void); // get change alfa flag
	void SetChangeAlfa(bool Value); // set change alfa flag

	bool GetChangeEta(void); // get change eta flag
	void SetChangeEta(bool Value); // se tchange eta flag

	float GetEndAlfa(void); // get end alfa value
	void SetEndAlfa(float Value); // set end alfa value

	float GetEndEta(void); // get end eta value
	void SetEndEta(float Value); // set end eta value

	int GetEpochs(void); // get epochs number
	void SetEpochs(int Value); // set epochs number

	bool GetBreakErms(void); // get break flag
	void SetBreakErms(bool Value); // set break flag
	
	float GetMaxErms(void); // get max erms error
	void SetMaxErms(float Value); // set max erms error
	
	float GetMaxRms(void); // get max rms error
	void SetMaxRms(float Value); // set max rms error

	int GetWidth(void); // get image width
	void SetWidth(int Value); // set image width

	int GetHeight(void); // get image height
	void SetHeight(int Value); // set image height

	int GetRepeats(void); // get each vector repeats
	void SetRepeats(int Value); // set each vector repeats

protected:
	int DoOnStart(int VectorsCount);
	int DoOnStop(void);
	int DoOnBeforeVector(CVector *Vector);
	int DoOnAfterVector(CVector *Vector);
	int DoOnBeforeEpoch(int Epoch);
	int DoOnAfterEpoch(int Epoch);
	int DoOnErms(float Erms);

// training / testing routines
public:
	int StartTraining(CNet *Net, void *User); // start training
	int StopTraining(bool Close = true, bool Wait = true); // stop training

	int StartTesting(CNet *Net, void *User); // start testing
	int StopTesting(bool Close = true, bool Wait = true); // stop testing

	TRAINING_START OnStart; // called before training
	TRAINING_VECTOR OnBeforeVector; // called before vector
	TRAINING_VECTOR OnAfterVector; // called after vector
	TRAINING_EPOCH OnBeforeEpoch; // called before each epoch
	TRAINING_EPOCH OnAfterEpoch; // called after each epoch
	TRAINING_EVENT OnStop; // called after training
	TRAINING_ERMS OnErms; // called if ERMS is less then MaxErms

// training thread routine
public:
	friend DWORD WINAPI TrainingThread(LPVOID lpParameter); // training thread
};

// error codes
#define TRAINING_ERROR_TERMINATE     -10
#define TRAINING_ERROR_NET_MISMATCH  -9
#define TRAINING_ERROR_CLOSED        -8
#define TRAINING_ERROR_RUNNING       -7
#define TRAINING_ERROR_FILE          -6
#define TRAINING_ERROR_NEXT          -5
#define TRAINING_ERROR_OPEN          -4
#define TRAINING_ERROR_CREATE        -3
#define TRAINING_ERROR_HEADER        -2
#define TRAINING_ERROR               -1
#define TRAINING_OK                   0
