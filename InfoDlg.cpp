//
// InfoDlg.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Implementacja klasy CInfoDlg.
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "InfoDlg.h"

//
// CInfoDlg dialog
//
IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
	, m_Inputs(0)
	, m_Outputs(0)
	, m_Layers(0)
	, m_Neurons(0)
	, m_Function(_T(""))
	, m_Alfa(0.0f)
	, m_Beta(0.0f)
	, m_Eta(0.0f)
	, m_Width(0)
	, m_Height(0)
	, m_Net(NULL)
{
}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_INFO_INPUTS, m_Inputs);
	DDX_Text(pDX, IDC_INFO_OUTPUTS, m_Outputs);
	DDX_Text(pDX, IDC_INFO_LAYERS, m_Layers);
	DDX_Text(pDX, IDC_INFO_NEURONS, m_Neurons);
	DDX_Text(pDX, IDC_INFO_FUNCTION, m_Function);
	DDX_Text(pDX, IDC_INFO_ALFA, m_Alfa);
	DDX_Text(pDX, IDC_INFO_BETA, m_Beta);
	DDX_Text(pDX, IDC_INFO_ETA, m_Eta);
	DDX_Text(pDX, IDC_INFO_WIDTH, m_Width);
	DDX_Text(pDX, IDC_INFO_HEIGHT, m_Height);
}

void CInfoDlg::SetNet(CNet *Net)
{
	// store pointer to net
	m_Net = Net;

	// update controls
	UpdateInfo();
}

void CInfoDlg::UpdateInfo(void)
{
	LPCACTFUNCTION lpFunction;

	// if net is not set
	if (m_Net == NULL)
		// break
		return;

	// get inputs number
	m_Inputs = (int)m_Net->GetInputs();

	// get outputs number
	m_Outputs = (int)m_Net->GetOutputs();

	// get layers number
	m_Layers = (int)m_Net->Layers.GetCount();

	// get width
	m_Width = m_Net->Width;

	// get height
	m_Height = m_Net->Height;

	// clear neurons number
	m_Neurons = 0;

	// for each layer
	for (int i = 0; i < m_Layers; i++)
		// get neurons number in i-layer
        m_Neurons += m_Net->Layers[i].GetNeurons();

	// get function activation
	lpFunction = GetFunction(m_Net->GetFunction());

	// if function exists
	if (lpFunction != NULL)
		// get function name
		m_Function = lpFunction->Name;

	// get alfa
	m_Alfa = m_Net->Alfa;

	// get beta
	m_Beta = m_Net->Beta;

	// get eta
	m_Eta = m_Net->Eta;

	GetDlgItem(IDC_INFO_ALFA)->EnableWindow(theApp.MainFrame->m_NetCreated);
	GetDlgItem(IDC_INFO_ETA)->EnableWindow(theApp.MainFrame->m_NetCreated);

	// update controls
	UpdateData(FALSE);
}

void CInfoDlg::OnOK(void)
{
	// get values from controls
	UpdateData(TRUE);

	// update net
	m_Net->Alfa = m_Alfa;

	// update net
	m_Net->Eta = m_Eta;

	// set focus to child view
	theApp.MainFrame->SetFocus();
}

void CInfoDlg::OnCancel(void)
{
	// set focus to child view
	theApp.MainFrame->SetFocus();
}

BOOL CInfoDlg::OnInitDialog()
{
	// initialize dialog
	CDialog::OnInitDialog();

	// update controls
	UpdateInfo();

	// no focus
	return FALSE;
}

BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
END_MESSAGE_MAP()

// CInfoDlg message handlers
