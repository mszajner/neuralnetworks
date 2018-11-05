//
// ImportDlg.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiow:   2FD 2004/2005
//
// Implementacja klasy CImportDlg.
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "ImportDlg.h"
#include "Vector.h"

//
// CImportDlg dialog
//

IMPLEMENT_DYNAMIC(CImportDlg, CDialog)

CImportDlg::CImportDlg(CNet *Net, CTraining *Training, CWnd* pParent)
	: CDialog(CImportDlg::IDD, pParent)
	, m_Monochrome(0)
	, m_Black(1)
	, m_Bias(192)
	, m_Testing(0)
	, m_Output(-1)
	, m_Name(_T(""))
	, m_Net(Net)
	, m_Training(Training)
	, m_FileName(_T(""))
{
	ASSERT(m_Net);
	ASSERT(m_Training);

	// create preview image
	m_Display.Create(m_Training->GetWidth(), 
		m_Training->GetHeight(), 24);
}

CImportDlg::~CImportDlg()
{
	// destroy preview image
	m_Display.Destroy();
}

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_IMPORT_GRAYED, m_Monochrome);
	DDX_Radio(pDX, IDC_IMPORT_WHITE, m_Black);
	DDX_Control(pDX, IDC_IMPORT_BIAS, m_BiasCtrl);
	DDX_Control(pDX, IDC_IMPORT_OUTPUT, m_OutputCtrl);
	DDX_Slider(pDX, IDC_IMPORT_BIAS, m_Bias);
	DDX_Check(pDX, IDC_IMPORT_TESTING, m_Testing);
	DDX_Text(pDX, IDC_IMPORT_FILENAME, m_FileName);
	DDX_Text(pDX, IDC_IMPORT_NAME, m_Name);
}

BOOL CImportDlg::OnInitDialog()
{
	char FileTitle[MAX_PATH];

	// call original function
	CDialog::OnInitDialog();

	// set slider range
	m_BiasCtrl.SetRange(1, 254);

	// set slider pos
	m_BiasCtrl.SetPos(192);

	// get outputs number
	int Count = m_Net->GetOutputs();

	// for each output
	for (int i = 0; i < Count; i++)
	{
		// add output name to combobox
		int Index = m_OutputCtrl.AddString(m_Net->Categories[i]);

		// set item number
		m_OutputCtrl.SetItemData(Index, i);
	}

	// select last output
	m_OutputCtrl.SetCurSel(m_Output);

	if (Count <= 0)
	{
		// load text
		CString Text;

		// load text
		Text.LoadString(IDS_IMPORT_NO_OUTPUTS);

		// add text
		int Index = m_OutputCtrl.AddString(Text);

		// set item number
		m_OutputCtrl.SetItemData(Index, -1);

		// select item
		m_OutputCtrl.SetCurSel(Index);
	}

	// get file title
	GetFileTitle(m_FileName, FileTitle, MAX_PATH);

	// set vector name as file title
	m_Name = FileTitle;

	// update controls
	UpdateData(FALSE);

	// update display
	UpdateDisplay();

	// enable/disable slider control
	GetDlgItem(IDC_IMPORT_BIAS)->EnableWindow(m_Monochrome);

	// set focus in dialog
	return TRUE;
}

void CImportDlg::UpdateDisplay(void)
{
	CVector m_Vector;

	// try to import vector
	if (m_Vector.Import(m_FileName, m_Monochrome, m_Black, m_Bias, 
		m_Training->GetWidth(), m_Training->GetHeight()) == VECTOR_OK)
	{
		// get vector data
		float *Data = m_Vector.GetData();

		// fill image
		for (int y = 0; y < m_Training->GetHeight(); y++)
			for (int x = 0; x < m_Training->GetWidth(); x++)
			{
				BYTE c = 255 - (BYTE)((*Data++) * 255.0f);

				// set image pixel
				m_Display.SetPixelRGB(x, y, c, c, c);
			}
	}
}

INT_PTR CImportDlg::DoImport(CString FilePath, bool Reset)
{
	BOOL bWorking;
	CFileFind Find;
	static INT_PTR RetCode;
	
	RetCode = Reset ? IDC_IMPORT_OK : RetCode;

	// find first file
	bWorking = Find.FindFile(FilePath);

	// till find file
	while (bWorking)
	{
		// get found file
		bWorking = Find.FindNextFile();

		// check if is not directory and not '.' or not '..'
		if (!Find.IsDirectory() && !Find.IsDots())
		{
			int Pos = 0;
			bool Found = false;

			CString Ext, FileExt = ".txt;.jpeg;.jpg;.bmp;.gif;.png";

			// get first extension
			Ext = FileExt.Tokenize(";", Pos);

			while (Ext != "")
			{
				// check if file has extension
				if (Find.GetFileName().Right(4).CompareNoCase(Ext) == 0)
				{
					// set found flag
					Found = true;
					break;
				}

				// get next extension
				Ext = FileExt.Tokenize(";", Pos);
			}

			// if extension not found
			if (!Found)
				// find next file
				continue;

			// set file name
			m_FileName = Find.GetFilePath();

			// if last selected button was not import all
			if (RetCode != IDC_IMPORT_ALL)
			{
				// show modal dialog
                RetCode = CDialog::DoModal();

				// if cancel button clicked
				if (RetCode == IDC_IMPORT_CANCEL)
				{
					// close find class
					Find.Close();

					// return cancel
					return IDC_IMPORT_CANCEL;
				}
			}

			// if not selected output
			if (m_Output < 0)
			{
				CString Text;

				// load text
				Text.LoadString(IDS_IMPORT_NO_SELECTED_OUTPUT);

				// show messabe box
				MessageBox(Text, 0, MB_ICONERROR | MB_OK);
			}
			else if (RetCode != IDC_IMPORT_SKIP)
			{
				// add vector to training
				CVector *Vector = m_Training->Add();

				// if added
				if (Vector != NULL)
				{
					// import vector data
					if (Vector->Import(m_FileName, m_Monochrome, m_Black, m_Bias, 
						m_Training->GetWidth(), m_Training->GetHeight()) == VECTOR_OK)
	                {
						// set testing flag
						Vector->SetTesting(m_Testing);

						// set vector output
						Vector->SetOutput(m_Output);

						// set vector name
						if (RetCode != IDC_IMPORT_ALL)
							// set user vector name
							Vector->SetName(m_Name);
						else
							// set file title name as vector name
							Vector->SetName(Find.GetFileName());
					}
				}
			}
		}
	}

	// close find class
	Find.Close();

	// return OK
	return IDC_IMPORT_OK;
}

INT_PTR CImportDlg::DoModal(int Output, bool BrowseFile)
{
	// set default output
	m_Output = Output;

    if (!BrowseFile)
	{
		BROWSEINFO bi;
		char DisplayName[MAX_PATH];

		// clear display name buffer
		memset(DisplayName, 0, MAX_PATH);

		// clear browse info struct
		memset(&bi, 0, sizeof(bi));

		// load browse info struct
		bi.hwndOwner = NULL;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = DisplayName;
		bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE
			| BIF_NONEWFOLDERBUTTON	| BIF_RETURNONLYFSDIRS;
		bi.lpfn = NULL;
		bi.lParam = 0;

		// try to browse for folder
		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

		// ok clicked
		if (pidl != NULL)
		{
			char Folder[MAX_PATH << 2];

			// clear folder path buffer
			memset(Folder, 0, sizeof(Folder));

			// get path from id list
			SHGetPathFromIDList(pidl, Folder);

			// free id list
			CoTaskMemFree(pidl);

			// copy folder name
			CString FilePath = Folder;

			// append *.* to seach whole folder
			FilePath += "\\*.*";

			// do import
			return DoImport(FilePath, true);
		}
	}
	else
	{
		CString Filter;

		// load filter
		Filter.LoadString(IDS_IMPORT_FILTER);

		// create file dialog object
		CFileDialog FileDialog(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT |
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, Filter);

		// extend buffer for file name to 2048 chars
		LPTSTR lpstrFile = (LPTSTR)malloc(5000);

		if (lpstrFile != NULL)
		{
			FileDialog.m_pOFN->lpstrFile = lpstrFile;
			FileDialog.m_pOFN->lpstrFile[0] = '\0';
			FileDialog.m_pOFN->nMaxFile = 2048;
		}

		// show dialog
		if (FileDialog.DoModal() == IDOK)
		{
			bool Reset = true;

			// get first selected file
			POSITION Pos = FileDialog.GetStartPosition();

			while (Pos != NULL)
			{
				// do import for each file
				if (DoImport(FileDialog.GetNextPathName(Pos), Reset) == IDC_IMPORT_CANCEL)
					// return cancel
					return IDC_IMPORT_CANCEL;

				Reset = false;
			}
		}

		// free extended buffer
		free(lpstrFile);

		// return ok
		return IDC_IMPORT_OK;
	}

	// return cancel
	return IDC_IMPORT_CANCEL;
}

BEGIN_MESSAGE_MAP(CImportDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_IMPORT_MONOCHROME, OnImportMonochrome)
	ON_BN_CLICKED(IDC_IMPORT_GRAYED, OnImportMonochrome)
	ON_BN_CLICKED(IDC_IMPORT_BLACK, OnImportMonochrome)
	ON_BN_CLICKED(IDC_IMPORT_WHITE, OnImportMonochrome)
	ON_CBN_SELCHANGE(IDC_IMPORT_OUTPUT, OnImportOutput)
	ON_BN_CLICKED(IDC_IMPORT_ALL, OnImportAll)
	ON_BN_CLICKED(IDC_IMPORT_SKIP, OnImportSkip)
	ON_BN_CLICKED(IDC_IMPORT_OK, OnImportOk)
	ON_BN_CLICKED(IDC_IMPORT_CANCEL, OnImportCancel)
	ON_WM_CONTEXTMENU()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CImportDlg message handlers

void CImportDlg::OnPaint()
{
	// if display image is loaded
	if (!m_Display.IsNull())
	{
		CRect Rect;

		// get image control
		CWnd *pWnd = GetDlgItem(IDC_IMPORT_IMAGE);

		// get control device context
		CDC *pDC = pWnd->GetWindowDC();

		// get control rect
		pWnd->GetClientRect(&Rect);

		// move rect to (0,0)
		Rect.MoveToXY(0, 0);

		// draw display image on control
		m_Display.Draw(pDC->m_hDC, Rect);
	}

	// call OnPaint for dialog
	CDialog::OnPaint();
}

void CImportDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// call update
	OnImportMonochrome();

	// call orginal procedure
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CImportDlg::OnImportMonochrome()
{
	// get values from controls
	UpdateData(TRUE);

	// enable/disable slider control
	GetDlgItem(IDC_IMPORT_BIAS)->EnableWindow(m_Monochrome);

	// update display
	UpdateDisplay();

	// repaint
	OnPaint();
}

void CImportDlg::OnImportOutput()
{
	// get selected output
	int Index = m_OutputCtrl.GetCurSel();

	// if selected
	if (Index >= 0)
		// set vector output
		m_Output = (int)m_OutputCtrl.GetItemData(Index);
	else
		// select no output
		m_Output = -1;
}

void CImportDlg::OnImportAll()
{
	// get values from controls
	UpdateData(TRUE);

	// import all
	EndDialog(IDC_IMPORT_ALL);
}

void CImportDlg::OnImportSkip()
{
	// get values from controls
	UpdateData(TRUE);

	// skip this file
	EndDialog(IDC_IMPORT_SKIP);
}

void CImportDlg::OnImportOk()
{
	// get values from controls
	UpdateData(TRUE);

	// import this file
	EndDialog(IDC_IMPORT_OK);
}

void CImportDlg::OnImportCancel()
{
	// get values from controls
	UpdateData(TRUE);

	// cancel this and next files
	EndDialog(IDC_IMPORT_CANCEL);
}

