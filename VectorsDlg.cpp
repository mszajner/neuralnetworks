// VectorsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "VectorsDlg.h"
#include "ImportDlg.h"
#include ".\vectorsdlg.h"

// CVectorsDlg dialog

IMPLEMENT_DYNAMIC(CVectorsDlg, CDialog)

CVectorsDlg::CVectorsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVectorsDlg::IDD, pParent)
	, m_OutputName(_T(""))
	, m_VectorName(_T(""))
	, m_Vector(NULL)
	, m_OutputIndex(-1)
	, m_Output(-1)
	, m_VectorTesting(FALSE)
	, m_Training(NULL)
{
}

CVectorsDlg::~CVectorsDlg()
{
}

void CVectorsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	// controls
	DDX_Control(pDX, IDC_OUTPUTS_LIST, m_OutputsList);
	DDX_Control(pDX, IDC_VECTORS_LIST, m_VectorsList);

	// edits
	DDX_Text(pDX, IDC_OUTPUT_NAME, m_OutputName);
	DDX_Text(pDX, IDC_VECTOR_NAME, m_VectorName);
	DDX_Check(pDX, IDC_VECTOR_TESTING, m_VectorTesting);
	DDX_Control(pDX, IDC_VECTOR_NAME, m_VectorEdit);
}

BOOL CVectorsDlg::OnInitDialog()
{
	CString Default;

	// call orginal OnInitDialog procedure
	CDialog::OnInitDialog();

	// clear copy list
	m_CopyList.RemoveAll();

	// clear output name edit
	m_OutputName = _T("");

	// clear vector name edit
	m_VectorName = _T("");

	// no vector selected
	m_Vector = NULL;
	m_VectorIndex = -1;

	// no output selected
	m_OutputIndex = -1;
	m_Output = -1;

	// load all vector string
	Default.LoadString(IDS_ALL_VECTORS);

	// add all vectors item
	int OutputIndex = m_OutputsList.AddString(Default);

	// set output number as -1 (all vectors)
	m_OutputsList.SetItemData(OutputIndex, (DWORD)-1);

	// get outputs number
	int Count = theApp.Net.GetOutputs();

	// for each output
	for (int i = 0; i < Count; i++)
	{
		// add output to list
		OutputIndex = m_OutputsList.AddString(theApp.Net.Categories[i]);

		// if added
		if (OutputIndex >= 0)
		{
			// set associated output number
			m_OutputsList.SetItemData(OutputIndex, (DWORD)i);
		}
	}

	// update controls
	UpdateData(FALSE);

	// create preview image
	m_Image.Create(theApp.Training.GetWidth(), 
		theApp.Training.GetHeight(), 24);

	// set focus to this dialog
	SetFocus();

	// return true -- remain focus
	return TRUE;
}

void CVectorsDlg::OnOK()
{
	if (theApp.MainFrame->DoVectorsEndDialog(IDOK) >= 0)
		// destroy window
		DestroyWindow();
}

void CVectorsDlg::OnCancel()
{
	if (theApp.MainFrame->DoVectorsEndDialog(IDCANCEL) >= 0)
		// destroy window
		DestroyWindow();
}

BOOL CVectorsDlg::DestroyWindow()
{
	// destroy image
	m_Image.Destroy();

	// destroy window
	return CDialog::DestroyWindow();
}

void CVectorsDlg::DoChangeOutputsList(int Item)
{
	// set new value
	if (Item >= 0)
	{
		// store output index
		m_OutputIndex = Item;
	}

	// check range
	if (m_OutputIndex < 0)
		// break
		return;

	// get output number for associated item
	m_Output = (int)m_OutputsList.GetItemData(m_OutputIndex);

	// reset vectors list content
	m_VectorsList.ResetContent();

	// get first vector from training
	CVector *Vector = theApp.Training.Next(m_Output, true);

	// if vector is not null
	while (Vector != NULL)
	{
		CString Text;

		// get vector name
		Vector->GetName(Text);

		// add vector name to vectors list
		int VectorIndex = m_VectorsList.AddString(Text);

		// if vector added
		if (VectorIndex >= 0)
			// set item data pointer to this vector
			m_VectorsList.SetItemDataPtr(VectorIndex, Vector);

		// get next vector from training
		Vector = theApp.Training.Next(m_Output);
	}

	// clear pointer vector (no vector selected)
	m_Vector = NULL;
	m_VectorIndex = -1;
	m_VectorName = _T("");

	// clear testing vector checkbox
	m_VectorTesting = false;

	// set output name edit text
	m_OutputsList.GetText(m_OutputIndex, m_OutputName);

	// update controls
	UpdateData(FALSE);
}

void CVectorsDlg::DoChangeVectorsList(int Item)
{
	ASSERT(Item >= 0);

	// get associated vector
	CVector *Vector = (CVector*)m_VectorsList.GetItemDataPtr(Item);

	ASSERT(Vector != NULL);

	// store vector pointer
	m_Vector = Vector;

	// store vector index
	m_VectorIndex = Item;

	// get length of vector
	int Length = m_Vector->GetLength();

	// if length == 0 (vector is not loaded)
	if (!Length)
		// read vector data from file
		theApp.Training.Read(m_Vector);

	// get vector data
	float *Data = m_Vector->GetData();

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
		m_Vector->ClearData();

	// set vector name edit
	m_VectorsList.GetText(m_VectorIndex, m_VectorName);

	// get testing flag
	m_VectorTesting = m_Vector->IsTesting();

	// update controls
	UpdateData(FALSE);

	// repaint image
	OnPaint();
}

BEGIN_MESSAGE_MAP(CVectorsDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_OUTPUTS_LIST, OnChangeOutputsList)
	ON_LBN_SELCHANGE(IDC_VECTORS_LIST, OnChangeVectorsList)
	ON_BN_CLICKED(IDC_DELETE_FILE, OnDeleteFile)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CLEAR_TRAINING, OnClearTraining)
	ON_BN_CLICKED(IDC_VECTOR_TESTING, OnVectorTesting)
	ON_BN_CLICKED(IDC_TRAINING_SETTINGS, OnTrainingSettings)
	ON_BN_CLICKED(IDC_ADD_FILE, OnAddFile)
	ON_BN_CLICKED(IDC_ADD_FOLDER, OnAddFolder)
	ON_BN_CLICKED(IDC_CUT, OnCut)
	ON_BN_CLICKED(IDC_PASTE, OnPaste)
	ON_EN_CHANGE(IDC_VECTOR_NAME, OnVectorName)
END_MESSAGE_MAP()

// CVectorsDlg message handlers

void CVectorsDlg::OnChangeOutputsList()
{
	// get selected output index
	int OutputIndex = m_OutputsList.GetCaretIndex();

	// if any selected
	if ((OutputIndex >= 0) && (m_OutputIndex != OutputIndex))
	{
		// do change outputs list message
		DoChangeOutputsList(OutputIndex);
	}
}

void CVectorsDlg::OnChangeVectorsList()
{
	// get selected vector index
	int VectorIndex = m_VectorsList.GetCaretIndex();

	// if any selected
	if ((VectorIndex >= 0) && (m_VectorIndex != VectorIndex))
	{
		// do change vectors list message
		DoChangeVectorsList(VectorIndex);
	}
}

void CVectorsDlg::OnDeleteFile()
{
	// if any vector selected
	if (m_Vector != NULL)
	{
		CArray<int,int> SelItems;

		// get selected items number
		int Count = m_VectorsList.GetSelCount();

		CString Format, Prompt;

		// if selected more than one
		if (Count > 1)
		{
			// load format string
			Format.LoadString(IDS_DELETE_VECTORS);

			// create prompt string
			Prompt.Format(Format, Count);
		}

		// if selected one vector
		else if (Count == 1)
		{
			CString Name;

			// load format string
			Format.LoadString(IDS_DELETE_VECTOR);

			// get vector name
			m_Vector->GetName(Name);

			// create prompt string
			Prompt.Format(Format, Name);
		}

		// if none selected
		else
			// break
			return;

		// show message box
		if (MessageBox(Prompt, 0, MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
			// break
			return;

		// allocate memory
        SelItems.SetSize(Count);

		// get selected items indexes
		m_VectorsList.GetSelItems(Count, SelItems.GetData());

		// for each selected vector
		for (int i = Count - 1; i >= 0; i--)
		{
			// get vector pointer
			CVector *Vector = (CVector*)m_VectorsList.GetItemDataPtr(SelItems[i]);

			ASSERT(Vector != NULL);

			// delete vector
			theApp.Training.Delete(Vector);

			// delete string (vector) from vectors list
			m_VectorsList.DeleteString(SelItems[i]);
		}

		// no vector selected
		m_Vector = NULL;
		m_VectorIndex = -1;
	}
}

void CVectorsDlg::OnPaint()
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

void CVectorsDlg::OnClearTraining()
{
	CString Prompt;

	// load prompt string
	Prompt.LoadString(IDS_CLEAR_TRAINING);

	// show messagebox
	if (MessageBox(Prompt, 0, MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		// delete all vectors
		theApp.Training.ClearVectors();

		// update vector list
		DoChangeOutputsList();
	}
}

void CVectorsDlg::OnVectorTesting()
{
	// if any vector selected
	if (m_Vector != NULL)
	{
		// get state of check box
		UpdateData(TRUE);

		// set/clear testing flag
		m_Vector->SetTesting(m_VectorTesting);
	}
}

void CVectorsDlg::OnTrainingSettings()
{
	if (theApp.MainFrame->DoVectorsEndDialog(IDC_TRAINING_SETTINGS) >= 0)
		// destroy window
		DestroyWindow();
}

void CVectorsDlg::OnAddFile()
{
	CImportDlg ImportDlg(&theApp.Net, &theApp.Training);
	ImportDlg.DoModal(m_Output, true);
	DoChangeOutputsList();
}

void CVectorsDlg::OnAddFolder()
{
	CImportDlg ImportDlg(&theApp.Net, &theApp.Training);
	ImportDlg.DoModal(m_Output, false);
	DoChangeOutputsList();
}

void CVectorsDlg::OnCut()
{
	CArray<int,int> SelItems;

	// get selected items number
	int Count = m_VectorsList.GetSelCount();

	// set selected
	SelItems.SetSize(Count);

	// get selected items indexes
	m_VectorsList.GetSelItems(Count, SelItems.GetData());

	// for each selected vector
	for (int i = 0; i < Count; i++)
	{
		// get vector pointer
		CVector *Vector = (CVector*)m_VectorsList.GetItemDataPtr(SelItems[i]);

		ASSERT(Vector != NULL);

		m_CopyList.AddHead(Vector);
	}
}

void CVectorsDlg::OnPaste()
{
	while (!m_CopyList.IsEmpty() && (m_OutputIndex >= 0))
	{
		CVector *Vector = m_CopyList.RemoveHead();

		ASSERT(Vector != NULL);

		if (Vector)
			Vector->SetOutput(m_Output);
	}

	DoChangeOutputsList();
}

void CVectorsDlg::OnVectorName()
{
	// get selected vectors item
	int Index = m_VectorsList.GetCaretIndex();

	if (Index >= 0)
	{
		CString Text;

		// get vector name
		m_VectorEdit.GetWindowText(Text);

		// delete previous item
		m_VectorsList.DeleteString(Index);

		// insert item with new text
		m_VectorsList.InsertString(Index, Text);

		// set caret index and select item
		m_VectorsList.SetCaretIndex(Index);
		m_VectorsList.SetSel(Index);
	}
}
