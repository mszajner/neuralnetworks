// Training.cpp : implementation file
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "Training.h"

const char TrainingHeader[4] = { 'T', 'R', 'N', '1' };

// CTraining

CTraining::CTraining()
	: m_Running(false)
	, m_Testing(false)
	, OnStart(NULL)
    , OnBeforeVector(NULL)
	, OnAfterVector(NULL)
	, OnBeforeEpoch(NULL)
	, OnAfterEpoch(NULL)
	, OnStop(NULL)
{
	// clear training
	ClearHeader();

	// clear vectors
	ClearVectors();
}

CTraining::~CTraining()
{
	// clear training
	ClearHeader();

	// clear vectors
	ClearVectors();
}

// CTraining member functions

int CTraining::Open(const char *FileName)
{
	// if running
	if (m_Running)
		// return error
		return TRAINING_ERROR_RUNNING;

	// close previous file
	Close();

	// try open file
	if (!m_File.Open(FileName, CFile::modeReadWrite))
		// return error
		return TRAINING_ERROR_OPEN;

	// return ok
	return TRAINING_OK;
}

int CTraining::Close(void)
{
	// if running
	if (m_Running)
		// return error
		return TRAINING_ERROR_RUNNING;

	// if open
	if (IsOpen())
		// close file
		m_File.Close();

	// return ok
	return TRAINING_OK;
}

int CTraining::Load(void)
{
	// if running
	if (m_Running)
		// return error
		return TRAINING_ERROR_RUNNING;

	// if closed
	if (!IsOpen())
		// return error
		return TRAINING_ERROR_CLOSED;

	// clear buffer
	char Buffer[4] = { 0, 0, 0, 0 };

	// seek file to begin
	m_File.SeekToBegin();

	// read header
	m_File.Read(Buffer, 4);

	// compare with orginal
	if (memcmp(Buffer, TrainingHeader, 4) != 0)
		// return error
		return TRAINING_ERROR_HEADER;

	// read header from file
	if (m_File.Read(&m_Header, sizeof(m_Header)) != sizeof(m_Header))
		// return error
		return TRAINING_ERROR;

	// clear vectors array
	ClearVectors();

	// create vector
	CVector Vector;

	// read vector from file
	while (Vector.ReadHeader(m_File, m_File.GetPosition()) == VECTOR_OK)
	{
		// add vector to array
		m_Vectors.Add(Vector);

		// seek to another vector
		m_File.Seek(sizeof(float) * GetWidth() * GetHeight(), CFile::current);
	}

	// return ok
	return TRAINING_OK;
}

int CTraining::Save(const char *FileName)
{
	// if running
	if (m_Running)
		// return error
		return TRAINING_ERROR_RUNNING;

	// if closed and no file name
	if (!IsOpen() && (FileName == NULL))
		// return error
		return TRAINING_ERROR_CLOSED;

	// get vectors count
	int count = (int)m_Vectors.GetCount();

	// if file name is set
	if (FileName != NULL)
	{
		CFile m_NewFile;

		// try create new file
		if (!m_NewFile.Open(FileName, CFile::modeCreate | CFile::modeReadWrite))
			// return error
			return TRAINING_ERROR;

		// write training file sign
		m_NewFile.Write(TrainingHeader, 4);

		// write training header
		m_NewFile.Write(&m_Header, sizeof(m_Header));

		// for each vector
		for (int i = 0; i < count; i++)
		{
			// if vector is not deleted
			if (!m_Vectors[i].IsDeleted())
			{
				int Length = m_Vectors[i].GetLength();

				// if not loaded
				if (!Length)
					// load vector from orginal file
					m_Vectors[i].ReadData(m_File, GetWidth() * GetHeight());

				// write vector header
				m_Vectors[i].WriteHeader(m_NewFile, m_NewFile.GetPosition());

				// write vector data
				m_Vectors[i].WriteData(m_NewFile);

				// free vector data
				m_Vectors[i].ClearData();
			}
		}

		m_NewFile.Close();

		return Open(FileName);
	}
	else
	{
		// seel to file sign
		m_File.Seek(0, CFile::begin);

		// write file sign
		m_File.Write(TrainingHeader, 4);

		// write header
		m_File.Write(&m_Header, sizeof(m_Header));

		// for each vector
		for (int i = 0; i < count; i++)
		{
			// write vector header to file
			m_Vectors[i].WriteHeader(m_File);

			// if data allocated
			if (m_Vectors[i].GetLength())
			{
				// write data
				m_Vectors[i].WriteData(m_File);

				// free data
				m_Vectors[i].ClearData();
			}
			else
			{
				// seek to another vector
				m_File.Seek(sizeof(float) * GetWidth() * GetHeight(), CFile::current);
			}
		}
	}

	// return ok
	return TRAINING_OK;
}

int CTraining::Read(CVector *Vector)
{
	// if vector is not null
	if (Vector != NULL)
	{
		// read vector data
		return Vector->ReadData(m_File, GetWidth() * GetHeight());
	}

	// return ok
	return VECTOR_OK;
}

int CTraining::Write(CVector *Vector)
{
	// if running
	if (m_Running)
		// return error
		return TRAINING_ERROR_RUNNING;

	// if vector is not null
	if (Vector != NULL)
	{
		// write vector header to file
		Vector->WriteHeader(m_File);

		// if data allocated
		if (Vector->GetLength())
		{
			// write data
			Vector->WriteData(m_File);

			// free data
			Vector->ClearData();
		}
	}

	// return ok
	return VECTOR_OK;
}

int CTraining::ClearHeader(void)
{
	// if running
	if (m_Running)
		// return error
		return TRAINING_ERROR_RUNNING;

	// zero header
	ZeroMemory(&m_Header, sizeof(m_Header));

	// set defaults
	m_Header.ChangeAlfa = false;
	m_Header.ChangeEta = false;
	m_Header.EndAlfa = 0.05f;
	m_Header.EndEta = 0.02f;
	m_Header.Epochs = 100;
	m_Header.BreakErms = false;
	m_Header.MaxErms = 0.05f;
	m_Header.Width = 0;
	m_Header.Height = 0;

	// return ok
	return TRAINING_OK;
}

int CTraining::ClearVectors(void)
{
	// if running
	if (m_Running)
		// return error
		return TRAINING_ERROR_RUNNING;

	// clear vectors array
	m_Vectors.SetSize(0);

	// return ok
	return VECTOR_OK;
}

bool CTraining::IsOpen(void)
{
	// return true if file is open
	return m_File.m_hFile != CFile::hFileNull;
}

CVector* CTraining::Next(int Output, bool Reset)
{
	static int Index = 0;

	// if reset
	if (Reset)
		// reset to first vector
		Index = 0;

	// if index is less than vectors count
	while (Index < m_Vectors.GetCount())
	{
		// if output is -1 or output is equal to vector output
		if (!m_Vectors[Index].IsDeleted() && 
			((Output == -1) || (m_Vectors[Index].GetOutput() == Output)))
			// return vector
			return &m_Vectors[Index++];

		// increment index
		Index++;
	}

	// return null -- no vectors
	return NULL;
}

CVector* CTraining::Add(void)
{
	// get vectors count
	int count = (int)m_Vectors.GetCount();

	// for each vector
	for (int i = 0; i < count; i++)
	{
		// check if deleted
		if (m_Vectors[i].IsDeleted())
		{
			// clear vector
			m_Vectors[i].ClearHeader();

			// return vector
			return &m_Vectors[i];
		}
	}

	// create new vector
	CVector Vector(GetWidth() * GetHeight());

	// add vector
	int Index = (int)m_Vectors.Add(Vector);

	// return vector
	return &m_Vectors[Index];
}

int CTraining::Delete(CVector *Vector)
{
	// set vector deleted flag
	Vector->SetDeleted(true);

	// return ok
	return TRAINING_OK;
}

bool CTraining::IsRunning(void)
{
	// return training running flag
	return m_Running;
}

bool CTraining::IsTesting(void)
{
	// return testing mode flag
	return m_Testing;
}

bool CTraining::GetChangeAlfa(void)
{
	// return change alfa flag
	return m_Header.ChangeAlfa;
}

void CTraining::SetChangeAlfa(bool Value)
{
	// if not running
	if (!m_Running)
	{
		// store new change alfa flag value
		m_Header.ChangeAlfa = Value;
	}
}

bool CTraining::GetChangeEta(void)
{
	// return change eta flag
	return m_Header.ChangeEta;
}

void CTraining::SetChangeEta(bool Value)
{
	// if not running
	if (!m_Running)
	{
		// store new change eta flag value
		m_Header.ChangeEta = Value;
	}
}

float CTraining::GetEndAlfa(void)
{
	// return end alfa value
	return m_Header.EndAlfa;
}

void CTraining::SetEndAlfa(float Value)
{
	// if not running
	if (!m_Running)
	{
		// store end alfa value
		m_Header.EndAlfa = Value;
	}
}

float CTraining::GetEndEta(void)
{
	// return end eta value
	return m_Header.EndEta;
}

void CTraining::SetEndEta(float Value)
{
	// if not running
	if (!m_Running)
	{
		// store end eta value
		m_Header.EndEta = Value;
	}
}

int CTraining::GetEpochs(void)
{
	// return epochs number
	return m_Header.Epochs;
}

void CTraining::SetEpochs(int Value)
{
	// if not running
	if (!m_Running)
	{
		// store new epochs number value
		m_Header.Epochs = Value;
	}
}

bool CTraining::GetBreakErms(void)
{
	// return break on error flg
	return m_Header.BreakErms;
}

void CTraining::SetBreakErms(bool Value)
{
	// if not running
	if (!m_Running)
	{
		// store new break on error flag value
		m_Header.BreakErms = Value;
	}
}
	
float CTraining::GetMaxErms(void)
{
	// return max erms error value
	return m_Header.MaxErms;
}

void CTraining::SetMaxErms(float Value)
{
	// if not running
	if (!m_Running)
	{
		// store new max erms error value
		m_Header.MaxErms = Value;
	}
}

int CTraining::GetWidth(void)
{
	// return width of image
	return m_Header.Width;
}

void CTraining::SetWidth(int Value)
{
	// if not running
	if (!m_Running)
	{
		// store new width value
		m_Header.Width = Value;
	}
}

int CTraining::GetHeight(void)
{
	// return height of image
	return m_Header.Height;
}

void CTraining::SetHeight(int Value)
{
	// if not running
	if (!m_Running)
	{
		// store new height value
		m_Header.Height = Value;
	}
}

int CTraining::GetRepeats(void)
{
	// return each vector repeats
	return m_Header.Repeats;
}

void CTraining::SetRepeats(int Value)
{
	// if not running
	if (!m_Running)
	{
		// store new repeats value
		m_Header.Repeats = Value;
	}
}

int CTraining::StartTraining(CNet *Net, void *Parameter)
{
	// if running
	if (m_Running)
		// return error
		return TRAINING_ERROR_RUNNING;

	if ((Net->Width != GetWidth()) || (Net->Height != GetHeight()))
		return TRAINING_ERROR_NET_MISMATCH;

	m_Net = Net;
	m_Parameter = Parameter;

	// set running flag
	m_Running = true;

	// create training thread
	m_Thread = CreateThread(NULL, 0, &TrainingThread, 
		this, 0, &m_ThreadId);

	// if thread handle is null
	if (m_Thread == NULL)
	{
		// clear running flag
		m_Running = false;

		// return error
		return TRAINING_ERROR;
	}

	// return ok
	return TRAINING_OK;
}

int CTraining::StopTraining(bool Close, bool Wait)
{
	int nRet = TRAINING_OK;

	// if running and thread create
	if (m_Running && m_Thread)
	{
		// clear running flag
		m_Running = false;

		if (Wait)
		{
			// wait for thread termination 10 sek.
			if (WaitForSingleObject(m_Thread, 10000) != WAIT_OBJECT_0)
			{
				// if not terminatem, force terminating
				TerminateThread(m_Thread, (DWORD)-1);

				// error - thread was forced terminated
				nRet = TRAINING_ERROR_TERMINATE;
			}
		}
	}

	if (Close)
	{
		// close thread handle
		CloseHandle(m_Thread);
	}

	// return ok
	return nRet;
}

int CTraining::StartTesting(CNet *Net, void *User)
{
	// set testing flag
	m_Testing = true;

	// start training
	return StartTraining(Net, User);
}

int CTraining::StopTesting(bool Close, bool Wait)
{
	// stop training
	return StopTraining(Close, Wait);
}

int CTraining::DoOnStart(int VectorsCount)
{
	if (OnStart != NULL)
		return OnStart(this, VectorsCount, m_Parameter);

	return 0;
}

int CTraining::DoOnStop(void)
{
	if (OnStop != NULL)
		return OnStop(this, m_Parameter);

	return 0;
}

int CTraining::DoOnBeforeVector(CVector *Vector)
{
	if (OnBeforeVector != NULL)
		return OnBeforeVector(this, Vector, m_Parameter);

	return 0;
}

int CTraining::DoOnAfterVector(CVector *Vector)
{
	if (OnAfterVector != NULL)
		return OnAfterVector(this, Vector, m_Parameter);

	return 0;
}

int CTraining::DoOnBeforeEpoch(int Epoch)
{
	if (OnBeforeEpoch != NULL)
		return OnBeforeEpoch(this, Epoch, m_Parameter);

	return 0;
}

int CTraining::DoOnAfterEpoch(int Epoch)
{
	if (OnAfterEpoch != NULL)
		return OnAfterEpoch(this, Epoch, m_Parameter);

	return 0;
}

int CTraining::DoOnErms(float Erms)
{
	if (OnErms != NULL)
		return OnErms(this, Erms, m_Parameter);

	return 0;
}

DWORD WINAPI TrainingThread(LPVOID lpParameter)
{
	typedef CArray<CVector*,CVector*> CVectorArray;
	typedef CList<CVector*,CVector*> CVectorList;

	float DeltaEta, DeltaAlfa;
	float NetEta, NetAlfa;
	int Epoch = 0, Repeats;
	CVector *Vector;
	CVectorList Queue;
	CVectorArray Backup;
	CVectorArray Vectors;

	// get training object
	CTraining *Training = (CTraining*)lpParameter;

	Repeats = Training->GetRepeats();

	// get vectors from training
	for (Vector = Training->Next(-1, true); Vector != NULL; Vector = Training->Next())
	{
		if ((Vector->GetOutput() > -1) && (Training->m_Testing == (Vector->IsTesting() ? true : false)))
			Backup.Add(Vector);
	}

	if (!Training->m_Testing)
	{
		NetEta = Training->m_Net->Eta;
		NetAlfa = Training->m_Net->Alfa;
		DeltaEta = (NetEta - Training->GetEndEta()) / (float)Training->GetEpochs();
		DeltaAlfa = (NetAlfa - Training->GetEndAlfa()) / (float)Training->GetEpochs();
	}

	// do OnStart event
	Training->DoOnStart((int)Backup.GetCount());

	// initialize random number generator
	srand((unsigned)time(NULL));

	// main training/testing loop
	while (Training->m_Running && (Epoch < Training->GetEpochs()))
	{
		Vectors.Copy(Backup);

		// Losowanie epoki...
		while (Vectors.GetCount())
		{
			int Index = rand() % (int)Vectors.GetCount();

			Queue.AddTail(Vectors[Index]);

			Vectors.RemoveAt(Index);
		}

		if (Training->m_Testing)
		{
			while (Training->m_Running && !Queue.IsEmpty() && (Vector = Queue.RemoveHead()))
			{
				// TODO: oprogramowac kazda sciezke bledow!!!

				if ((Training->Read(Vector) >= 0) && (Training->m_Net->LoadVector(Vector) >= 0))
				{
					if (Training->DoOnBeforeVector(Vector) < 0)
					{
						Vector->ClearData();
						break;
					}

					Training->m_Net->Execute();

					if (Training->DoOnAfterVector(Vector) < 0)
					{
						Vector->ClearData();
						break;
					}
				}

				Vector->ClearData();
			}

			break;
		}
		else
		{
			if (Training->DoOnBeforeEpoch(Epoch) < 0)
				break;

			// Przetwarzanie epoki...
			while (Training->m_Running && !Queue.IsEmpty() && (Vector = Queue.RemoveHead()))
			{
				// TODO: oprogramowac kazda sciezke bledow!!!

				if ((Training->Read(Vector) >= 0) && (Training->m_Net->LoadVector(Vector) >= 0))
				{
					if (Training->DoOnBeforeVector(Vector) < 0)
					{
						Vector->ClearData();
						break;
					}

					for (int i = 0; (i < Repeats) && (Training->m_Running); i++)
						Training->m_Net->Teach();

					if (Training->DoOnAfterVector(Vector) < 0)
					{
						Vector->ClearData();
						break;
					}
				}

				Vector->ClearData();
			}

			Training->m_Net->CalcErms();

			if (Training->DoOnAfterEpoch(Epoch++) < 0)
				break;

			if (Training->GetBreakErms() && (Training->m_Net->Erms < Training->GetMaxErms()))
			{
				Training->DoOnErms(Training->m_Net->Erms);
				break;
			}
			
			if (Training->GetChangeAlfa())
				Training->m_Net->Alfa -= DeltaAlfa;
			if (Training->GetChangeEta())
				Training->m_Net->Eta -= DeltaEta;
		}
	}

	if (!Training->m_Testing)
	{
		Training->m_Net->Alfa = NetAlfa;
		Training->m_Net->Eta = NetEta;
	}

	// do OnStop event
	Training->DoOnStop();

	// turn off testing mode
	Training->m_Testing = false;

	// close thread handle
	Training->StopTraining(true, false);

	// return ok
	return TRAINING_OK;
}
