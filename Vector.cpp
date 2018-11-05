// Vector.cpp : implementation file
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "Vector.h"

// CVector

CVector::CVector() 
	: m_Data(NULL)
	, m_Length(0)
	, m_Offset(0)
{
	// clear vector header
	ClearHeader();
}

CVector::CVector(int Length)
	: m_Data(NULL)
	, m_Length(0)
	, m_Offset(0)
{
	// clear entire vector
	ClearHeader();

	// set length of vector
	AllocData(Length);
}

CVector::~CVector()
{
	// clear vector data
	ClearData();
}

int CVector::AllocData(int Length)
{
	// if length is less than zero
	if (Length < 0)
		// return current length
		return m_Length;

	// if length is diff from current length
	if (Length != m_Length)
	{
		// delete previous data
		delete m_Data;

		if (Length > 0)
            // set new vector length
			m_Data = new float[Length];
		else
			m_Data = NULL;

		// store vector length
		m_Length = Length;
	}

	// return ok
	return m_Length;
}

int CVector::ReadHeader(CFile &File, ULONG64 Offset)
{
	// if offset parameter is set
	if (Offset != 0)
		// set new offset of vector
		m_Offset = Offset;

	// if offset is not set
	if (m_Offset == 0)
		// get current position in file
		m_Offset = File.GetPosition();

	// try to seek file to offset
	if (File.Seek(m_Offset, CFile::begin) != m_Offset)
		// return error
		return VECTOR_ERROR_SEEK;

	// read header of header
	if (File.Read(&m_Header, sizeof(m_Header)) != sizeof(m_Header))
		// return error
		return VECTOR_ERROR_HEADER;

	// return ok
	return VECTOR_OK;
}

int CVector::WriteHeader(CFile &File, ULONG64 Offset)
{
	// if offset parameter is set
	if (Offset != 0)
		// set new offset of vector
		m_Offset = Offset;

	// if offset is not set
	if (m_Offset == 0)
		// get current position in file
		m_Offset = File.GetPosition();

	// try to seek file to header
	if (File.Seek(m_Offset, CFile::begin) != m_Offset)
		// return error
		return VECTOR_ERROR_SEEK;

	// write header
	File.Write(&m_Header, sizeof(m_Header));

	// return ok
	return VECTOR_OK;
}

int CVector::ReadData(CFile &File, int Length)
{
	// if length is set
	if (Length != 0)
	{
		int nRet;

		// try to set new length of vector
		if ((nRet = AllocData(Length)) < 0)
			// return error
			return nRet;
	}

	// if header is loaded from file and data is not loaded
	// and offset is set and length of vector is set
	if (m_Offset && m_Length)
	{
		// calculate size of vector (in bytes)
		UINT nCount = (UINT)sizeof(float) * (UINT)m_Length;

		// try to seek file to header
		if (File.Seek(m_Offset + sizeof(m_Header), CFile::begin) !=
				(m_Offset + sizeof(m_Header)))
			// return error
			return VECTOR_ERROR_SEEK;

		// read vector data
		if (File.Read(m_Data, nCount) != nCount)
			// return error
			return VECTOR_ERROR_DATA;
	}

	// return ok
	return VECTOR_OK;
}

int CVector::WriteData(CFile &File)
{
	// if header offset is set and length of vector is set
	if (m_Offset && m_Length)
	{
		// try to seek file to header
		if (File.Seek(m_Offset + sizeof(m_Header), CFile::begin) !=
				(m_Offset + sizeof(m_Header)))
			// return error
			return VECTOR_ERROR_SEEK;

		// write data to file
		File.Write(m_Data, sizeof(float) * m_Length);
	}

	// return ok
	return VECTOR_OK;
}

int CVector::ClearData(void)
{
	// clear data
	return AllocData(0);
}

int CVector::ClearHeader(void)
{
	// zero header memory
	ZeroMemory(&m_Header, sizeof(m_Header));

	// set output to none
	m_Header.Output = -1;

	// return ok
	return VECTOR_OK;
}

int CVector::IsDeleted(void)
{
	// get deleted flag
	return m_Header.Deleted;
}

void CVector::SetDeleted(int Value)
{
	// re/set deleted flag
	m_Header.Deleted = Value;
}

int CVector::IsTesting(void)
{
	// get "for testing" flag
	return m_Header.Testing;
}

void CVector::SetTesting(int Value)
{
	// set "for testing" flag
	m_Header.Testing = Value;
}

int CVector::GetOutput(void)
{
	// get requested output
	return m_Header.Output;
}

void CVector::SetOutput(int Output)
{
	// set requested output
	m_Header.Output = Output;
}

void CVector::GetName(CString &Name)
{
	// get vector name
	Name.SetString(m_Header.Name, 32);
}

void CVector::SetName(const CString &Name)
{
	// set vector name
	strncpy(m_Header.Name, Name, 32);
}

int CVector::GetLength(void)
{
	// return vector length
	return m_Length;
}

float* CVector::GetData(void)
{
	// get vector data
	return m_Data;
}

int CVector::Import(CString FileName, int Monochrome, int Black, 
					int Bias, int Width, int Height)
{
	int nRet;
	CImage Image;

	// try to load image
	if (Image.Load(FileName) == E_FAIL)
	{
		FILE *File;

		// try to open file for reading
		if ((File = fopen(FileName, "rt")) != NULL)
		{
			// try to set vector length
			if ((nRet = AllocData(Width * Height)) < 0)
			{
				// close file
				fclose(File);

				// return error
				return nRet;
			}
			
			// read vector values
			for (int i = 0; !feof(File) && (i < m_Length); i++)
			{
				fscanf(File, "%f", &m_Data[i]);

				if (Monochrome)
				{
					if (m_Data[i] > (float)(Bias / 255.0f))
						m_Data[i] = 1.0f;
					else
						m_Data[i] = 0.0f;
				}
			}

			// close file
			fclose(File);

			// return ok
			return VECTOR_OK;
		}
	}
	else
	{
		CImage Convert;

		// create second image
		if (!Convert.Create(Width, Height, Image.GetBPP()))
			// return error
			return VECTOR_ERROR_IMAGE_CREATE;

		// try set length of vector
		if ((nRet = AllocData(Width * Height)) < 0)
			// return error
			return nRet;

		// copy in image to out image
		Image.StretchBlt(Convert.GetDC(), 0, 0, Width, Height,
			0, 0, Image.GetWidth(), Image.GetHeight());

		// destroy bitmap
		Image.Destroy();

		for (int y = 0, i = 0; y < Height; y++)
		{
			for (int x = 0; x < Width; x++)
			{
				// get out pixel
				COLORREF InPixel = Convert.GetPixel(x, y);
				
				float Pixel = (float)GetRValue(InPixel) + 
					(float)GetGValue(InPixel) + (float)GetBValue(InPixel);

				Pixel /= 3.0f;

				if (Monochrome)
				{
					if (Pixel > (float)Bias)
						Pixel = 255;
					else
						Pixel = 0;
				}

				if (Black)
					Pixel = 255 - Pixel;

				// calculate float value from 0.0 to 1.0
				m_Data[i++] = Pixel / 255.0f;
			}
		}

		// release dc
		Convert.ReleaseDC();

		// destroy bitmap
		Convert.Destroy();

		// return ok
		return VECTOR_OK;
	}

	// return error
	return VECTOR_ERROR_IMPORT;
}

CVector& CVector::operator=(CVector &Vector)
{
	// copy vector to this vector
	memcpy(this, &Vector, sizeof(Vector));

	// clear data
	this->m_Data = NULL;

	// clear length to reallocate memory in next statement
	this->m_Length = 0;

	// alloc memory for data
	this->AllocData(Vector.m_Length);

	// copy vector data
	memcpy(this->m_Data, Vector.m_Data, sizeof(float) * Vector.m_Length);

	// return this
	return *this;
}
