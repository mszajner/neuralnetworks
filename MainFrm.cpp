//
// MainFrm.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Implementacja klasy CMainFrame.
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "MainFrm.h"
#include "ImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_NET_NEW, OnNetNew)
	ON_COMMAND(ID_NET_EDIT, OnNetEdit)
	ON_COMMAND(ID_NET_OPEN, OnNetOpen)
	ON_COMMAND(ID_NET_SAVE, OnNetSave)
	ON_COMMAND(ID_NET_SAVE_AS, OnNetSaveAs)
	ON_COMMAND(ID_TRAINING_NEW, OnTrainingNew)
	ON_COMMAND(ID_TRAINING_EDIT, OnTrainingEdit)
	ON_COMMAND(ID_TRAINING_OPEN, OnTrainingOpen)
	ON_COMMAND(ID_TRAINING_SAVE, OnTrainingSave)
	ON_COMMAND(ID_TRAINING_SAVE_AS, OnTrainingSaveAs)
	ON_COMMAND(ID_TRAINING_TEACH, OnTeach)
	ON_COMMAND(ID_VECTORS_FILE, OnVectorsFile)
	ON_COMMAND(ID_VECTORS_FOLDER, OnVectorsFolder)
	ON_COMMAND(ID_VECTORS_LIST, OnVectorsList)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_TESTING_TESTING, OnTesting)
	ON_COMMAND(ID_RECOGNIZE_RECOGNIZE, OnRecognize)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
	: m_NetModified(0)
	, m_TrainingModified(0)
	, m_NetCreated(0)
	, m_TrainingCreated(0)
{
	m_NetFile = _T("");
	m_TrainingFile = _T("");
}

CMainFrame::~CMainFrame()
{
	m_NetFile = _T("");
	m_TrainingFile = _T("");
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// create frame
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	// create toolbar
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// create rebar
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	// create status bar
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME,
        CBRS_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialog bar\n");
		return -1;      // fail to create
	}

	// set toolbar style
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	// create info dialog
	m_InfoDlg.Create(CInfoDlg::IDD, &m_wndDlgBar);

	// set net
	m_InfoDlg.SetNet(&theApp.Net);

	// update window caption
	UpdateCaption();

	// return 0 -- ok
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

int CMainFrame::DoNetNew(void)
{
	if (theApp.Training.IsRunning())
		return 0;

	// check if no dialog if open
	if (!m_wndView.IsDialog())
	{
		// create net dialog
		m_NetDlg.Create(CNetDlg::IDD, &m_wndView);

		// set parent of dialog
		m_NetDlg.SetParent(&m_wndView);

		// set creating new net
		m_NetDlg.CreateNet(&theApp.Net);

		// store pointer for alignment algorithm (OnSize)
		m_wndView.SetDialog(&m_NetDlg);

		// disable info dialog
		m_InfoDlg.EnableWindow(0);

		// return 0 -- ok
		return 0;
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoNetEdit(void)
{
	if (theApp.Training.IsRunning())
		return 0;

	// check if no dialog if open
	if (!m_wndView.IsDialog())
	{
		// create net dialog
		m_NetDlg.Create(CNetDlg::IDD, this);

		// set parent of dialog
		m_NetDlg.SetParent(&m_wndView);

		// set editing new net
		m_NetDlg.EditNet(&theApp.Net);

		// store pointer for alignment algorithm (OnSize)
		m_wndView.SetDialog(&m_NetDlg);

		// disable info dialog
		m_InfoDlg.EnableWindow(0);

		// return 0 -- ok
		return 0;
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoNetOpen(void)
{
	CString Filter, DefaultExt;

	if (theApp.Training.IsRunning())
		return 0;

	// prompt for save
	if (DoNetPrompt() != 0)
		return -1;

	// load filter list
	Filter.LoadString(IDS_NETFILTER);

	// load default extension for net
	DefaultExt.LoadString(IDS_NETDEFEXT);

	// create file dialog object
	CFileDialog FileDialog(TRUE, DefaultExt, m_NetFile,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, Filter);

	// show dialog
	if (FileDialog.DoModal() == IDOK)
	{
		CString Error;

		// try to load net
		switch (theApp.Net.Load(FileDialog.GetPathName()))
		{
		case NET_OK:
			// update info dialog
			m_InfoDlg.UpdateInfo();
			
			// store file name of net
			SetNetFile(FileDialog.GetPathName());

			// net is not modified
			m_NetModified = 0;
			m_NetCreated = TRUE;

			// update info dialog
			m_InfoDlg.UpdateInfo();

			// return 0 -- ok
			return 0;

		case NET_ERROR_FILE_OPEN:
			// load error string
			Error.LoadString(IDS_ERROR_OPEN);
			break;

		case NET_ERROR_FILE_HEADER:
			// load error string
			Error.LoadString(IDS_ERROR_NET_HEADER);
			break;
		}

		// if there was an error
		if (Error != "")
			// show error message
			MessageBox(Error, 0, MB_ICONERROR | MB_OK);
	}

	//return -1 -- error
	return -1;
}

int CMainFrame::DoNetSave(CString FileName)
{
	if (theApp.Training.IsRunning())
		return 0;

	// if file was selected
	if (FileName != "")
	{
		CString Error;

		// try to save net
		switch (theApp.Net.Save(FileName))
		{
			case NET_ERROR_FILE_CREATE:
				// load error string
				Error.LoadString(IDS_ERROR_SAVE);
				break;

			case NET_OK:
				// net is not modified
				m_NetModified = 0;

				// return 0 -- ok
				return 0;
		}

		// if there was an error
		if (Error != "")
			// show error message
			MessageBox(Error, 0, MB_ICONERROR | MB_OK);

		// return -1 -- error
		return -1;
	}
	else
	{
		// select file
		return DoNetSaveAs();
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoNetSaveAs(void)
{
	CString Filter, DefaultExt;

	if (theApp.Training.IsRunning())
		return 0;

	// load filter list
	Filter.LoadString(IDS_NETFILTER);

	// load default extension for net
	DefaultExt.LoadString(IDS_NETDEFEXT);

	// create file dialog object
	CFileDialog FileDialog(FALSE, DefaultExt, m_NetFile,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, Filter);

	// show dialog
	if (FileDialog.DoModal() == IDOK)
	{
		if (DoNetSave(FileDialog.GetPathName()) == 0)
		{
			// store file name in this class
			SetNetFile(FileDialog.GetPathName());

			// return 0 -- ok
			return 0;
		}
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoNetPrompt(void)
{
	CString Prompt;

	// if net is not modified
	if (!m_NetModified || !m_NetCreated)
		// return 0 -- ok
		return 0;

	// load prompt message
	Prompt.LoadString(IDS_PROMPT_NET_SAVE);

	// ask for saving
	switch (MessageBox(Prompt, 0, MB_ICONEXCLAMATION | MB_YESNOCANCEL))
	{
	case IDYES:
		// save net to file
		return DoNetSave(m_NetFile);

	case IDNO:
		// return 0 -- ok
		return 0;
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoNetEndDialog(UINT Result)
{
	if (Result == IDOK)
	{
		// net is modified
		m_NetModified = 1;

		// create new net so clear filename
		if (!m_NetDlg.m_Edit)
		{
			// clear net file
			SetNetFile("");
		}

		m_NetCreated = TRUE;

		// update info dialog
		m_InfoDlg.UpdateInfo();
	}

	// enable info dialog
	m_InfoDlg.EnableWindow();

	// set dialog
	m_wndView.SetDialog(NULL);

	// return 0 -- ok
	return 0;
}

int CMainFrame::DoTrainingNew(void)
{
	if (theApp.Training.IsRunning())
		return 0;

	// check if no dialog if open
	if (!m_wndView.IsDialog())
	{
		// create training dialog
		m_TrainingDlg.Create(CTrainingDlg::IDD, &m_wndView);

		// set parent of dialog
		m_TrainingDlg.SetParent(&m_wndView);

		// set creating new training
		m_TrainingDlg.CreateTraining();

		// store pointer for alignment algorithm (OnSize)
		m_wndView.SetDialog(&m_TrainingDlg);
		
		// return 0 -- ok
		return 0;
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoTrainingEdit(void)
{
	if (theApp.Training.IsRunning())
		return 0;

	// check if no dialog if open
	if (!m_wndView.IsDialog())
	{
		// create training dialog
		m_TrainingDlg.Create(CTrainingDlg::IDD, &m_wndView);

		// set parent of dialog
		m_TrainingDlg.SetParent(&m_wndView);

		// set creating new training
		m_TrainingDlg.EditTraining();

		// store pointer for alignment algorithm (OnSize)
		m_wndView.SetDialog(&m_TrainingDlg);

		// return 0 -- ok
		return 0;
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoTrainingOpen(void)
{
	CString Filter, DefaultExt;

	if (theApp.Training.IsRunning())
		return 0;

	// prompt for save
	if (DoTrainingPrompt() != 0)
		return -1;

	// load filter list
	Filter.LoadString(IDS_TRNFILTER);

	// load default extension for net
	DefaultExt.LoadString(IDS_TRNDEFEXT);

	// create file dialog object
	CFileDialog FileDialog(TRUE, DefaultExt, m_TrainingFile,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, Filter);

	// show dialog
	if (FileDialog.DoModal() == IDOK)
	{
		CString Error;

		// open file
		theApp.Training.Open(FileDialog.GetPathName());

		// try to load net
		switch (theApp.Training.Load())
		{
		case TRAINING_OK:
			// store training file name
			SetTrainingFile(FileDialog.GetPathName());

			// net is not modified
			m_TrainingModified = 0;
			m_TrainingCreated = TRUE;

			// return 0 -- ok
			return 0;
		}

		// if there was an error
		if (Error != "")
			// show error message
			MessageBox(Error, 0, MB_ICONERROR | MB_OK);
	}

	//return -1 -- error
	return -1;
}

int CMainFrame::DoTrainingSave(CString FileName, bool SaveAs)
{
	if (theApp.Training.IsRunning())
		return 0;

	// if file was selected
	if (FileName != "")
	{
		CString Error;

		// try to save net
		switch (theApp.Training.Save(SaveAs ? (const char*)FileName : NULL))
		{
			case TRAINING_OK:
				// training is not modified
				m_TrainingModified = 0;

				// return 0 -- ok
				return 0;
		}

		// if there was an error
		if (Error != "")
			// show error message
			MessageBox(Error, 0, MB_ICONERROR | MB_OK);

		// return -1 -- error
		return -1;
	}
	else
	{
		// select file
		return DoTrainingSaveAs();
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoTrainingSaveAs(void)
{
	CString Filter, DefaultExt;

	if (theApp.Training.IsRunning())
		return 0;

	// load filter list
	Filter.LoadString(IDS_TRNFILTER);

	// load default extension for net
	DefaultExt.LoadString(IDS_TRNDEFEXT);

	// create file dialog object
	CFileDialog FileDialog(FALSE, DefaultExt, m_TrainingFile,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, Filter);

	// show dialog
	if (FileDialog.DoModal() == IDOK)
	{
		if (DoTrainingSave(FileDialog.GetPathName(), true) == 0)
		{
			// store file name in this class
			SetTrainingFile(FileDialog.GetPathName());

			// return 0 -- ok
			return 0;
		}
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoTrainingPrompt(void)
{
	CString Prompt;

	// if training is not modified
	if (!m_TrainingModified || !m_TrainingCreated)
		// return 0 -- ok
		return 0;

	// load prompt message
	Prompt.LoadString(IDS_PROMPT_TRAINING_SAVE);

	// ask for saving
	switch (MessageBox(Prompt, 0, MB_ICONEXCLAMATION | MB_YESNOCANCEL))
	{
	case IDYES:
		// save net to file
		return DoTrainingSave(m_TrainingFile);

	case IDNO:
		// return 0 -- ok
		return 0;
	}

	// return -1 -- error
	return -1;
}

int CMainFrame::DoTrainingEndDialog(UINT Result)
{
	if ((Result == IDOK) || (Result == IDC_VECTORS_LIST))
	{
		// training is modified
		m_TrainingModified = 1;

		if (!m_TrainingDlg.m_bEdit)
		{
			// close training file
			theApp.Training.Close();

			// clear training vectors
			theApp.Training.ClearVectors();

			// clear training file
			SetTrainingFile("");
		}

		m_TrainingCreated = TRUE;
	}

	// set dialog
	m_wndView.SetDialog(NULL);

	// if vectors list button clicked
	if (Result == IDC_VECTORS_LIST)
	{
		// show vectors
		OnVectorsList();
	}

	// return 0 -- ok
	return 0;
}

int CMainFrame::DoVectorsEndDialog(UINT Result)
{
	// training is modified
	m_TrainingModified = 1;

	// set dialog
	m_wndView.SetDialog(NULL);

	// if training settings button clicked
	if (Result == IDC_TRAINING_SETTINGS)
	{
		// show training settings
        return DoTrainingEdit();
	}

	// return 0 -- ok
	return 0;
}

int CMainFrame::DoTeachEndDialog(UINT Result)
{
	// set dialog
	m_wndView.SetDialog(NULL);

	// enable info dialog
	m_InfoDlg.EnableWindow();

	// return ok
	return 0;
}

int CMainFrame::DoTestingEndDialog(UINT Result)
{
	// set dialog
	m_wndView.SetDialog(NULL);

	// return ok
	return 0;
}

int CMainFrame::DoRecognizeEndDialog(UINT Result)
{
	// set dialog
	m_wndView.SetDialog(NULL);

	// return ok
	return 0;
}

BOOL CMainFrame::CheckNetCreated(void)
{
	if (!m_NetCreated)
	{
		CString Text;
		Text.LoadString(IDS_NET_NOT_CREATED);
		MessageBox(Text, 0, MB_ICONERROR | MB_OK);
	}

	return m_NetCreated;
}

BOOL CMainFrame::CheckTrainingCreated(void)
{
	if (!m_TrainingCreated)
	{
		CString Text;
		Text.LoadString(IDS_TRAINING_NOT_CREATED);
		MessageBox(Text, 0, MB_ICONERROR | MB_OK);
	}

	return m_TrainingCreated;
}

bool CMainFrame::TrainingMustBeSaved(void)
{
	CString Text;

	Text.LoadString(IDS_TRN_MUST_SAVE);

	if (MessageBox(Text, 0, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
		return DoTrainingSave(m_TrainingFile) == 0;

	return false;
}

void CMainFrame::SetNetFile(CString FileName)
{
	// store file name
	m_NetFile = FileName;

	// update caption
	UpdateCaption();
}

void CMainFrame::SetTrainingFile(CString FileName)
{
	// store file name
	m_TrainingFile = FileName;

	// update caption
	UpdateCaption();
}

void CMainFrame::UpdateCaption(void)
{
	CString Caption;
	char FileTitle[MAX_PATH];

	// load caption string
	Caption.LoadString(IDR_MAINFRAME);

	// if net file name is set
	if (m_NetFile != "")
	{
		// clear buffer
		memset(FileTitle, 0, MAX_PATH);

		// extract file title only
		GetFileTitle(m_NetFile, FileTitle, MAX_PATH);

		// append "["
		Caption.Append(" [");

		// append file title
		Caption.Append(FileTitle);

		// append "]"
		Caption.Append("]");
	}

	// if training file name is set
	if (m_TrainingFile != "")
	{
		// clear buffer
		memset(FileTitle, 0, MAX_PATH);

		// extract file title only
		GetFileTitle(m_TrainingFile, FileTitle, MAX_PATH);

		// append "["
		Caption.Append(" [");

		// append file title
		Caption.Append(FileTitle);

		// append "]"
		Caption.Append("]");
	}

	// set window text
	SetWindowText(Caption);
}

// CMainFrame message handlers

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnNetNew()
{
	DoNetNew();
}

void CMainFrame::OnNetEdit()
{
	if (CheckNetCreated())
		DoNetEdit();
}

void CMainFrame::OnNetOpen()
{
	DoNetOpen();
}

void CMainFrame::OnNetSave()
{
	if (CheckNetCreated())
		DoNetSave(m_NetFile);
}

void CMainFrame::OnNetSaveAs()
{
	if (CheckNetCreated())
		DoNetSaveAs();
}

void CMainFrame::OnTrainingNew()
{
	DoTrainingNew();
}

void CMainFrame::OnTrainingEdit()
{
	if (CheckTrainingCreated())
		DoTrainingEdit();
}

void CMainFrame::OnTrainingOpen()
{
	DoTrainingOpen();
}

void CMainFrame::OnTrainingSave()
{
	if (CheckTrainingCreated())
		DoTrainingSave(m_TrainingFile);
}

void CMainFrame::OnTrainingSaveAs()
{
	if (CheckTrainingCreated())
		DoTrainingSaveAs();
}

void CMainFrame::OnClose()
{
	if (theApp.Training.IsRunning())
		return;

	// prompt for save
	if (!DoNetPrompt() && !DoTrainingPrompt())
		// close window
		CFrameWnd::OnClose();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CRect Rect;

	// call ancestor method
	CFrameWnd::OnSize(nType, cx, cy);

	// get dialog bar rect
	m_wndDlgBar.GetClientRect(Rect);

	// inflate rect
	Rect.InflateRect(-4, -4);

	// move dialog info with update
	m_InfoDlg.MoveWindow(Rect, TRUE);
}

void CMainFrame::OnVectorsFile()
{
	if (theApp.Training.IsRunning())
		return;

	if (!CheckTrainingCreated())
		return;

	CImportDlg ImportDlg(&theApp.Net, &theApp.Training);
	ImportDlg.DoModal(-1, true);
}

void CMainFrame::OnVectorsFolder()
{
	if (theApp.Training.IsRunning())
		return;

	if (!CheckTrainingCreated())
		return;

	CImportDlg ImportDlg(&theApp.Net, &theApp.Training);
	ImportDlg.DoModal(-1);
}

void CMainFrame::OnVectorsList()
{
	if (theApp.Training.IsRunning())
		return;

	if (!CheckTrainingCreated())
		return;

	// check if no dialog if open
	if (!m_wndView.IsDialog())
	{
		// create vectors dialog
		m_VectorsDlg.Create(CVectorsDlg::IDD, &m_wndView);

		// set parent of dialog
		m_VectorsDlg.SetParent(&m_wndView);

		// store pointer for alignment algorithm (OnSize)
		m_wndView.SetDialog(&m_VectorsDlg);
	}
}

void CMainFrame::OnTeach()
{
	if (!CheckNetCreated() || !CheckTrainingCreated())
		return;

	if (m_TrainingModified && !TrainingMustBeSaved())
		return;

	// check if no dialog if open
	if (!m_wndView.IsDialog())
	{
		// create teach dialog
		m_TeachDlg.Create(CTeachDlg::IDD, &m_wndView);

		// set parent of dialog
		m_TeachDlg.SetParent(&m_wndView);

		// store pointer for alignment algorithm (OnSize)
		m_wndView.SetDialog(&m_TeachDlg);

		// disable info dialog
		m_InfoDlg.EnableWindow(0);
	}
}

void CMainFrame::OnTesting()
{
	if (!CheckNetCreated() || !CheckTrainingCreated())
		return;

	if (m_TrainingModified && !TrainingMustBeSaved())
		return;

	// check if no dialog if open
	if (!m_wndView.IsDialog())
	{
		// create testing dialog
		m_TestingDlg.Create(CTestingDlg::IDD, &m_wndView);

		// set parent of dialog
		m_TestingDlg.SetParent(&m_wndView);

		// store pointer for alignment algorithm (OnSize)
		m_wndView.SetDialog(&m_TestingDlg);
	}
}

void CMainFrame::OnRecognize()
{
	if (!CheckNetCreated())
		return;

	// check if no dialog if open
	if (!m_wndView.IsDialog())
	{
		// create recognize dialog
		m_RecognizeDlg.Create(CRecognizeDlg::IDD, &m_wndView);

		// set parent of dialog
		m_RecognizeDlg.SetParent(&m_wndView);

		// store pointer for alignment algorithm (OnSize)
		m_wndView.SetDialog(&m_RecognizeDlg);
	}
}
