// RecognizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "RecognizeDlg.h"

// CRecognizeDlg dialog

IMPLEMENT_DYNAMIC(CRecognizeDlg, CDialog)
CRecognizeDlg::CRecognizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecognizeDlg::IDD, pParent)
	, m_Monochrome(0)
	, m_Black(1)
	, m_Bias(192)
	, m_FileName(_T(""))
	, m_Text(_T(""))
{
}

CRecognizeDlg::~CRecognizeDlg()
{
	m_Vector.ClearData();
}

void CRecognizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RECOGNIZE_OUTPUTS, m_Outputs);
	DDX_Radio(pDX, IDC_RECOGNIZE_MONOCHROME, m_Monochrome);
	DDX_Control(pDX, IDC_RECOGNIZE_BIAS, m_BiasCtrl);
	DDX_Radio(pDX, IDC_RECOGNIZE_WHITE, m_Black);
	DDX_Slider(pDX, IDC_RECOGNIZE_BIAS, m_Bias);
	DDX_Text(pDX, IDC_RECOGNIZE_FILENAME, m_FileName);
	DDX_Text(pDX, IDC_RECOGNIZE_TEXT, m_Text);
}

BOOL CRecognizeDlg::OnInitDialog()
{
	// call original function
	CDialog::OnInitDialog();

	CFont font;

	// create font
	VERIFY(font.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		NULL));                    // lpszFacename

	// set static text font
	GetDlgItem(IDC_RECOGNIZE_TEXT)->SetFont(&font);

	// create preview bitmap
	m_Display.Create(theApp.Net.Width, theApp.Net.Height, 24);

	// set slider range
	m_BiasCtrl.SetRange(1, 254);

	// set slider pos
	m_BiasCtrl.SetPos(192);

	// update controls
	UpdateData(FALSE);

	// update display
	UpdateDisplay();

	// enable/disable slider control
	GetDlgItem(IDC_RECOGNIZE_BIAS)->EnableWindow(m_Monochrome);

	// set focus in dialog
	return TRUE;
}

void CRecognizeDlg::OnOK()
{
	if (theApp.MainFrame->DoRecognizeEndDialog(IDOK) >= 0)
		DestroyWindow();
}

void CRecognizeDlg::OnCancel()
{
	if (theApp.MainFrame->DoRecognizeEndDialog(IDCANCEL) >= 0)
		DestroyWindow();
}

void CRecognizeDlg::UpdateDisplay(void)
{
	if (m_FileName == "")
		return;

	// try to import vector
	if (m_Vector.Import(m_FileName, m_Monochrome, m_Black, m_Bias, 
		theApp.Net.Width, theApp.Net.Height) == VECTOR_OK)
	{
		// get vector data
		float *Data = m_Vector.GetData();

		// fill image
		for (int y = 0; y < theApp.Net.Height; y++)
			for (int x = 0; x < theApp.Net.Width; x++)
			{
				BYTE c = 255 - (BYTE)((*Data++) * 255.0f);

				// set image pixel
				m_Display.SetPixelRGB(x, y, c, c, c);
			}
	}
}

BEGIN_MESSAGE_MAP(CRecognizeDlg, CDialog)
	ON_BN_CLICKED(IDC_RECOGNIZE, OnRecognize)
	ON_BN_CLICKED(IDC_RECOGNIZE_CLOSE, OnRecognizeClose)
	ON_BN_CLICKED(IDC_RECOGNIZE_MONOCHROME, OnRecognizeMonochrome)
	ON_BN_CLICKED(IDC_RECOGNIZE_GRAYED, OnRecognizeMonochrome)
	ON_BN_CLICKED(IDC_RECOGNIZE_BLACK, OnRecognizeMonochrome)
	ON_BN_CLICKED(IDC_RECOGNIZE_WHITE, OnRecognizeMonochrome)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RECOGNIZE_BROWSE, OnRecognizeBrowse)
END_MESSAGE_MAP()

// CRecognizeDlg message handlers

void CRecognizeDlg::OnRecognize()
{
	if (theApp.Net.LoadVector(&m_Vector) == VECTOR_OK)
	{
		theApp.Net.Execute();

		CString Category = theApp.Net.GetOutput();

		m_Text.Format(IDS_RECOGNIZE_TEXT, Category);

		UpdateData(FALSE);

		m_Outputs.ResetContent();

		CLayer &Layer = theApp.Net.Layers[theApp.Net.Layers.GetUpperBound()];

		float Max = 0.0f;

		for (int i = 0; i < Layer.GetNeurons(); i++)
		{
			CString Text;

			Text.Format(IDS_RECOGNIZE_OUTPUT, theApp.Net.Categories[i],
				Layer.Neurons[i].Output);

			int Index = m_Outputs.AddString(Text);

			if (Max < Layer.Neurons[i].Output)
			{
				Max = Layer.Neurons[i].Output;
				m_Outputs.SetCurSel(Index);
			}

		}
	}
}

void CRecognizeDlg::OnRecognizeClose()
{
	OnOK();
}

void CRecognizeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// call update
	OnRecognizeMonochrome();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CRecognizeDlg::OnRecognizeMonochrome()
{
	// get values from controls
	UpdateData(TRUE);

	// enable/disable slider control
	GetDlgItem(IDC_RECOGNIZE_BIAS)->EnableWindow(m_Monochrome);

	// update display
	UpdateDisplay();

	// repaint
	OnPaint();
}

void CRecognizeDlg::OnPaint()
{
	// call OnPaint for dialog
	CDialog::OnPaint();

	// if display image is loaded
	if (!m_Display.IsNull())
	{
		CRect Rect;

		// get image control
		CWnd *pWnd = GetDlgItem(IDC_RECOGNIZE_IMAGE);

		// get control device context
		CDC *pDC = pWnd->GetWindowDC();

		// get control rect
		pWnd->GetClientRect(&Rect);

		// move rect to (0,0)
		Rect.MoveToXY(0, 0);

		// draw display image on control
		m_Display.Draw(pDC->m_hDC, Rect);
	}
}

void CRecognizeDlg::OnRecognizeBrowse()
{
	CString Filter;

	// load filter
	Filter.LoadString(IDS_IMPORT_FILTER);

	// create file dialog object
	CFileDialog FileDialog(TRUE, NULL, m_FileName, OFN_HIDEREADONLY | 
		OFN_OVERWRITEPROMPT | OFN_EXPLORER, Filter);

	// show dialog
	if (FileDialog.DoModal() == IDOK)
	{
		m_FileName = FileDialog.GetPathName();

		UpdateData(FALSE);

		UpdateDisplay();

		this->RedrawWindow();
	}
}

BOOL CRecognizeDlg::DestroyWindow()
{
	m_Display.Destroy();

	return CDialog::DestroyWindow();
}
