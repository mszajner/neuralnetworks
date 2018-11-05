// TestingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "TestingDlg.h"
#include ".\testingdlg.h"

// CTestingDlg dialog

IMPLEMENT_DYNAMIC(CTestingDlg, CDialog)
CTestingDlg::CTestingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestingDlg::IDD, pParent)
{
}

CTestingDlg::~CTestingDlg()
{
}

void CTestingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Vectors);
	DDX_Control(pDX, IDC_LIST2, m_Symbols);
	DDX_Control(pDX, IDC_LIST3, m_Outputs);
}

BEGIN_MESSAGE_MAP(CTestingDlg, CDialog)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_WM_CLOSE()
	ON_LBN_SELCHANGE(IDC_LIST1, OnVectorsChange)
	ON_LBN_SELCHANGE(IDC_LIST2, OnSymbolsChange)
	ON_WM_PAINT()
END_MESSAGE_MAP()

int __stdcall OnStartTesting(CTraining *Training, int Vectors, void *Parameter)
{
	CTestingDlg *TestingDlg = (CTestingDlg*)Parameter;
	return TestingDlg->DoOnStart(Training, Vectors);
}

int __stdcall OnStopTesting(CTraining *Training, void *Parameter)
{
	CTestingDlg *TestingDlg = (CTestingDlg*)Parameter;
	TestingDlg->DoOnStop(Training);
	return 0;
}

int __stdcall OnBeforeVectorTesting(CTraining *Training, CVector *Vector, void *Parameter)
{
	CTestingDlg *TestingDlg = (CTestingDlg*)Parameter;
	return TestingDlg->DoOnBeforeVector(Training, Vector);
}

int __stdcall OnAfterVectorTesting(CTraining *Training, CVector *Vector, void *Parameter)
{
	CTestingDlg *TestingDlg = (CTestingDlg*)Parameter;
	return TestingDlg->DoOnAfterVector(Training, Vector);
}

// CTestingDlg message handlers

void CTestingDlg::OnOK()
{
	if (theApp.Training.IsRunning())
		return;

	if (theApp.MainFrame->DoTestingEndDialog(IDOK) >= 0)
		DestroyWindow();
}

void CTestingDlg::OnCancel()
{
	if (theApp.Training.IsRunning())
		return;

	if (theApp.MainFrame->DoTestingEndDialog(IDCANCEL) >= 0)
		DestroyWindow();
}

BOOL CTestingDlg::OnInitDialog()
{
	// create preview image
	m_Image.Create(theApp.Training.GetWidth(), 
		theApp.Training.GetHeight(), 24);

	return CDialog::OnInitDialog();
}

void CTestingDlg::OnTest()
{
	if (!theApp.Training.IsRunning())
	{
		theApp.Training.OnStart = OnStartTesting;
		theApp.Training.OnStop = OnStopTesting;
		theApp.Training.OnBeforeVector = OnBeforeVectorTesting;
		theApp.Training.OnAfterVector = OnAfterVectorTesting;
		theApp.Training.OnBeforeEpoch = NULL;
		theApp.Training.OnAfterEpoch = NULL;
		theApp.Training.OnErms = NULL;

		if (theApp.Training.StartTesting(&theApp.Net, this) >= 0)
		{
		}
	}
	else
	{
		if (theApp.Training.StopTesting(false, false) >= 0)
		{
		}
	}
}

void CTestingDlg::Clear(void)
{
	int count;

	m_Vectors.ResetContent();

	count = m_Symbols.GetCount();

	for (int i = 0; i < count; i++)
	{
		CFloatArray *Outputs = (CFloatArray*)m_Symbols.GetItemDataPtr(i);

		delete Outputs;
	}

	m_Symbols.ResetContent();
}

void CTestingDlg::UpdateOutputs(void)
{
	int Index = m_Symbols.GetCaretIndex();

	if (Index > -1)
	{
		CFloatArray *Outputs = (CFloatArray*)m_Symbols.GetItemDataPtr(Index);

		m_Outputs.ResetContent();

		for (int i = 0; i < Outputs->GetCount(); i++)
		{
			CString Text;
			Text.Format("Wyjœcie: %s, wartoœæ: %.4f", theApp.Net.Categories[i], Outputs->ElementAt(i));
			m_Outputs.AddString(Text);
		}
	}
}

int CTestingDlg::DoOnStart(CTraining *Training, int Vectors)
{
	Clear();
	return 0;
}

int CTestingDlg::DoOnStop(CTraining *Training)
{
	return 0;
}

int CTestingDlg::DoOnBeforeVector(CTraining *Training, CVector *Vector)
{
	CString Name;
	Vector->GetName(Name);
	int Index = m_Vectors.AddString(Name);
	m_Vectors.SetItemDataPtr(Index, Vector);
	return 0;
}

int CTestingDlg::DoOnAfterVector(CTraining *Training, CVector *Vector)
{
	CString Output = theApp.Net.GetOutput();
	CFloatArray *Outputs = new CFloatArray;
	int Matched = theApp.Net.FindActiveOutput();
	float Rms = theApp.Net.CalcRms();

	CString Text;
	Text.Format("%c, %s, b³¹d: %.4f", Matched == Vector->GetOutput() ? 'T' : 'N', Output, Rms);
	int Index = m_Symbols.AddString(Text);

	CLayer &Layer = theApp.Net.Layers[theApp.Net.Layers.GetUpperBound()];

	for (int i = 0; i < Layer.GetNeurons(); i++)
		Outputs->Add(Layer.Neurons[i].Output);

	m_Symbols.SetItemDataPtr(Index, Outputs);

	return 0;
}

void CTestingDlg::OnClose()
{
	Clear();
	CDialog::OnClose();
}

void CTestingDlg::OnVectorsChange()
{
	int Item = m_Vectors.GetCaretIndex();

	ASSERT(Item >= 0);

	m_Symbols.SetCurSel(Item);

	UpdateOutputs();

	// get associated vector
	CVector *Vector = (CVector*)m_Vectors.GetItemDataPtr(Item);

	ASSERT(Vector != NULL);

	// get length of vector
	int Length = Vector->GetLength();

	// if length == 0 (vector is not loaded)
	if (!Length)
		// read vector data from file
		theApp.Training.Read(Vector);

	// get vector data
	float *Data = Vector->GetData();

	// fill image
	for (int y = 0, i = 0; y < m_Image.GetHeight(); y++)
	{
		for (int x = 0; x < m_Image.GetWidth(); x++)
		{
			BYTE c = 255 - (BYTE)(Data[i++] * 255.0f);

			// set image pixel
			m_Image.SetPixelRGB(x, y, c, c, c);
		}
	}

	// if length == 0 (vector is not loaded)
	if (!Length)
		// free vector data
		Vector->ClearData();

	// repaint image
	OnPaint();
}

void CTestingDlg::OnSymbolsChange()
{
	m_Vectors.SetCurSel(m_Symbols.GetCaretIndex());
	OnVectorsChange();
}

void CTestingDlg::OnPaint()
{
	// call orginial procedure
	CDialog::OnPaint();

	if (!m_Image.IsNull())
	{
		CRect Rect;

		// get preview image window
		CWnd *pWnd = GetDlgItem(IDC_IMAGE);

		ASSERT(pWnd != NULL);

		// get dc of preview window
		CDC *pDC = pWnd->GetWindowDC();

		ASSERT(pDC != NULL);

		// get rect of window
		pWnd->GetClientRect(&Rect);

		// move rect to (0,0)
		Rect.MoveToXY(0, 0);

		// draw image on window
		m_Image.Draw(pDC->m_hDC, Rect);
	}
}

BOOL CTestingDlg::DestroyWindow()
{
	// destroy image
	m_Image.Destroy();

	return CDialog::DestroyWindow();
}
