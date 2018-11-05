// TrainingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "TrainingDlg.h"
#include ".\trainingdlg.h"

// CTrainingDlg dialog

IMPLEMENT_DYNAMIC(CTrainingDlg, CDialog)

CTrainingDlg::CTrainingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrainingDlg::IDD, pParent)
	, m_bEdit(0)
	, m_ChangeAlfa(0)
	, m_ChangeEta(0)
	, m_EndAlfa(0.05f)
	, m_EndEta(0.02f)
	, m_Epochs(100)
	, m_BreakErms(0)
	, m_MaxErms(0.05f)
	, m_ChangeBeta(0)
	, m_Beta(3.0f)
	, m_ChangeFunction(0)
	, m_Width(0)
	, m_Height(0)
	, m_Info(_T(""))
	, m_Repeats(1)
{
}

CTrainingDlg::~CTrainingDlg()
{
}

void CTrainingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_CONST_ALFA, m_ChangeAlfa);
	DDX_Radio(pDX, IDC_CONST_ETA, m_ChangeEta);
	DDX_Text(pDX, IDC_END_ALFA, m_EndAlfa);
	DDX_Text(pDX, IDC_END_ETA, m_EndEta);
	DDX_Text(pDX, IDC_EPOCHS, m_Epochs);
	DDX_Check(pDX, IDC_BREAKERMS, m_BreakErms);
	DDX_Text(pDX, IDC_MAXERMS, m_MaxErms);
	DDX_Check(pDX, IDC_CHANGE_BETA, m_ChangeBeta);
	DDX_Text(pDX, IDC_BETA, m_Beta);
	DDX_Check(pDX, IDC_CHANGE_FUNCTION, m_ChangeFunction);
	DDX_Control(pDX, IDC_FUNCTION, m_Function);
	DDX_Text(pDX, IDC_WIDTH, m_Width);
	DDX_Text(pDX, IDC_HEIGHT, m_Height);
	DDX_Text(pDX, IDC_INFO, m_Info);
	DDX_Text(pDX, IDC_REPEATS, m_Repeats);
}

BEGIN_MESSAGE_MAP(CTrainingDlg, CDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_EPOCHS_SPIN, OnEpochsSpin)
	ON_BN_CLICKED(IDC_CONST_ALFA, OnChangeAlfa)
	ON_BN_CLICKED(IDC_CHANGE_ALFA, OnChangeAlfa)
	ON_BN_CLICKED(IDC_CONST_ETA, OnChangeEta)
	ON_BN_CLICKED(IDC_CHANGE_ETA, OnChangeEta)
	ON_BN_CLICKED(IDC_BREAKERMS, OnBreakErms)
	ON_BN_CLICKED(IDC_CHANGE_BETA, OnChangeBeta)
	ON_BN_CLICKED(IDC_CHANGE_FUNCTION, OnChangeFunction)
	ON_NOTIFY(UDN_DELTAPOS, IDC_WIDTH_SPIN, OnWidthSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_HEIGHT_SPIN, OnHeightSpin)
	ON_EN_CHANGE(IDC_WIDTH, OnChangeSize)
	ON_EN_CHANGE(IDC_HEIGHT, OnChangeSize)
	ON_BN_CLICKED(IDC_VECTORS_LIST, OnVectorsList)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnRepeatsSpin)
	ON_BN_CLICKED(IDC_BUTTON1, OnRandom)
END_MESSAGE_MAP()

// CTrainingDlg message handlers

int CTrainingDlg::CreateTraining(void)
{
	m_bEdit = 0;

	m_ChangeAlfa = FALSE;
	m_ChangeEta = FALSE;
	m_EndAlfa = theApp.Net.Alfa;
	m_EndEta = theApp.Net.Eta;
	m_Epochs = 100;
	m_BreakErms = FALSE;
	m_MaxErms = 0.0f;
	m_ChangeBeta = FALSE;
	m_Beta = theApp.Net.Beta;
	m_ChangeFunction = FALSE;
	m_Width = theApp.Net.Width;
	m_Height = theApp.Net.Height;
	m_Repeats = 1;
	SelectFunction(theApp.Net.GetFunction());

	UpdateData(FALSE);

	return 0;
}

int CTrainingDlg::EditTraining(void)
{
	m_bEdit = 1;

	m_ChangeAlfa = theApp.Training.GetChangeAlfa();
	m_ChangeEta = theApp.Training.GetChangeEta();
	m_EndAlfa = theApp.Training.GetEndAlfa();
	m_EndEta = theApp.Training.GetEndEta();
	m_Epochs = theApp.Training.GetEpochs();
	m_BreakErms = theApp.Training.GetBreakErms();
	m_MaxErms = theApp.Training.GetMaxErms();
	m_ChangeBeta = FALSE;
	m_Beta = theApp.Net.Beta;
	m_ChangeFunction = FALSE;
	m_Width = theApp.Training.GetWidth();
	m_Height = theApp.Training.GetHeight();
	m_Repeats = theApp.Training.GetRepeats();
	SelectFunction(theApp.Net.GetFunction());

	UpdateData(FALSE);

	OnChangeAlfa();
	OnChangeEta();
	OnBreakErms();

	return 0;
}

void CTrainingDlg::OnOK()
{
	UpdateData(TRUE);

	theApp.Training.SetChangeAlfa(m_ChangeAlfa ? true : false);
	theApp.Training.SetChangeEta(m_ChangeEta ? true : false);
	theApp.Training.SetEndAlfa(m_EndAlfa);
	theApp.Training.SetEndEta(m_EndEta);
	theApp.Training.SetEpochs(m_Epochs);
	theApp.Training.SetBreakErms(m_BreakErms ? true : false);
	theApp.Training.SetMaxErms(m_MaxErms);
	theApp.Training.SetWidth(m_Width);
	theApp.Training.SetHeight(m_Height);
	theApp.Training.SetRepeats(m_Repeats);
	
	if (m_ChangeBeta)
		theApp.Net.Beta = m_Beta;
	
	if (m_ChangeFunction)
	{
		int Item = (int)m_Function.GetCurSel();

		// get id of selected function
		int Id = (int)m_Function.GetItemData(Item);

		// set function
		theApp.Net.SetFunction(Id);
	}

	if (theApp.MainFrame->DoTrainingEndDialog(IDOK) >= 0)
		DestroyWindow();
}

void CTrainingDlg::OnCancel()
{
	if (theApp.MainFrame->DoTrainingEndDialog(IDCANCEL) >= 0)
		DestroyWindow();
}

void CTrainingDlg::OnEpochsSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	UpdateData(TRUE);

	m_Epochs -= pNMUpDown->iDelta;

	if (m_Epochs < 1)
		m_Epochs = 1;

	UpdateData(FALSE);

	*pResult = 0;
}

void CTrainingDlg::OnChangeAlfa()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_END_ALFA)->EnableWindow(m_ChangeAlfa);
}

void CTrainingDlg::OnChangeEta()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_END_ETA)->EnableWindow(m_ChangeEta);
}

BOOL CTrainingDlg::OnInitDialog()
{
	unsigned int Id = 0;
	LPCACTFUNCTION lpFunction;

	// call ancestor function
	CDialog::OnInitDialog();

	// for each function
	while (lpFunction = GetFunction(Id++))
	{
		// add function name to combo box
		int Item = m_Function.AddString(lpFunction->Name);
		
		// if added
		if (Item >= 0)
			// set item data as function id
			m_Function.SetItemData(Item, lpFunction->Id);
	}

	// select default
	SelectFunction(FUNCTION_DEFAULT);
    
	OnChangeSize();

	// set focus to this dialog
	SetFocus();

	return TRUE;
}

void CTrainingDlg::SelectFunction(int Id)
{
	// get functions number
	int I, Count = m_Function.GetCount();

	// for each function in combo box
	for (I = 0; I < Count; I++)
	{
		// check id match
		if (m_Function.GetItemData(I) == Id)
		{
			// select item
			m_Function.SetCurSel(I);

			// break for-loop
			break;
		}
	}
}

void CTrainingDlg::OnBreakErms()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_MAXERMS)->EnableWindow(m_BreakErms);
}

void CTrainingDlg::OnChangeBeta()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_BETA)->EnableWindow(m_ChangeBeta);
}

void CTrainingDlg::OnChangeFunction()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_FUNCTION)->EnableWindow(m_ChangeFunction);
}

void CTrainingDlg::OnWidthSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// get values from controls
	UpdateData(TRUE);

	// update width variable
	m_Width -= pNMUpDown->iDelta;

	// check if width is less than one
	if (m_Width < 0)
		// set one
		m_Width = 0;

	// update controls
	UpdateData(FALSE);

	OnChangeSize();

	*pResult = 0;
}

void CTrainingDlg::OnHeightSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// get values from controls
	UpdateData(TRUE);

	// update height variable
	m_Height -= pNMUpDown->iDelta;

	// check if height is less than one
	if (m_Height < 0)
		// set one
		m_Height = 0;

	// update controls
	UpdateData(FALSE);

	OnChangeSize();

	*pResult = 0;
}

void CTrainingDlg::OnChangeSize()
{
	UpdateData(TRUE);
	
	if (!theApp.Net.Width || !theApp.Net.Height)
	{
		m_Info.LoadString(IDS_INFO_NETZERO);
	}

	else if (!m_Width || !m_Height)
	{
		m_Info.LoadString(IDS_INFO_ZERO);
	}

	else if ((m_Width != theApp.Net.Width) || (m_Height != theApp.Net.Height))
	{
		m_Info.LoadString(IDS_INFO_SIZE);
	}

	else
	{
		m_Info = _T("");
	}

	UpdateData(FALSE);
}

void CTrainingDlg::OnVectorsList()
{
	if (theApp.MainFrame->DoTrainingEndDialog(IDC_VECTORS_LIST) >= 0)
		DestroyWindow();
}

void CTrainingDlg::OnRepeatsSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// get values from controls
	UpdateData(TRUE);

	// update height variable
	m_Repeats -= pNMUpDown->iDelta;

	// check if height is less than one
	if (m_Repeats < 0)
		// set one
		m_Repeats = 0;

	// update controls
	UpdateData(FALSE);

	OnChangeSize();

	*pResult = 0;
}

void CTrainingDlg::OnRandom()
{
	// random weights
	theApp.Net.Random();
}
