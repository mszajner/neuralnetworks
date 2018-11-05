// TeachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "TeachDlg.h"

// CTeachDlg dialog

IMPLEMENT_DYNAMIC(CTeachDlg, CDialog)

CTeachDlg::CTeachDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTeachDlg::IDD, pParent)
	, m_ShowAll(FALSE)
	, m_LogName(_T(""))
{
}

CTeachDlg::~CTeachDlg()
{
}

void CTeachDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEACH_EPOCH_PROGRESS, m_VectorProgress);
	DDX_Control(pDX, IDC_TEACH_PROGRESS, m_EpochProgress);
	DDX_Control(pDX, IDC_TEACH_LOGGER, m_LoggerBox);
	DDX_Check(pDX, IDC_TEACH_SHOWALL, m_ShowAll);
}

BEGIN_MESSAGE_MAP(CTeachDlg, CDialog)
	ON_BN_CLICKED(IDC_TEACH_SHOWALL, OnShowAll)
	ON_BN_CLICKED(IDC_TEACH_SHOWLOG, OnShowLog)
	ON_BN_CLICKED(IDC_TEACH_RANDOM, OnRandom)
	ON_BN_CLICKED(IDC_TEACH_OK, OnTeachOk)
	ON_BN_CLICKED(IDC_TEACH_CANCEL, OnTeachCancel)
END_MESSAGE_MAP()

int __stdcall OnStart(CTraining *Training, int Vectors, void *Parameter)
{
	CTeachDlg *TeachDlg = (CTeachDlg*)Parameter;
	return TeachDlg->DoOnStart(Training, Vectors);
}

int __stdcall OnStop(CTraining *Training, void *Parameter)
{
	CTeachDlg *TeachDlg = (CTeachDlg*)Parameter;
	TeachDlg->DoOnStop(Training);
	return 0;
}

int __stdcall OnBeforeVector(CTraining *Training, CVector *Vector, void *Parameter)
{
	CTeachDlg *TeachDlg = (CTeachDlg*)Parameter;
	return TeachDlg->DoOnBeforeVector(Training, Vector);
}

int __stdcall OnAfterVector(CTraining *Training, CVector *Vector, void *Parameter)
{
	CTeachDlg *TeachDlg = (CTeachDlg*)Parameter;
	return TeachDlg->DoOnAfterVector(Training, Vector);
}

int __stdcall OnBeforeEpoch(CTraining *Training, int Epoch, void *Parameter)
{
	CTeachDlg *TeachDlg = (CTeachDlg*)Parameter;
	return TeachDlg->DoOnBeforeEpoch(Training, Epoch);
}

int __stdcall OnAfterEpoch(CTraining *Training, int Epoch, void *Parameter)
{
	CTeachDlg *TeachDlg = (CTeachDlg*)Parameter;
	return TeachDlg->DoOnAfterEpoch(Training, Epoch);
}

int __stdcall OnErms(CTraining *Training, float Erms, void *Parameter)
{
	CTeachDlg *TeachDlg = (CTeachDlg*)Parameter;
	return TeachDlg->DoOnErms(Training, Erms);
}

// CTeachDlg message handlers

void CTeachDlg::OnOK()
{
	if (!theApp.Training.IsRunning())
	{
		theApp.Training.OnStart = OnStart;
		theApp.Training.OnStop = OnStop;
		theApp.Training.OnBeforeVector = OnBeforeVector;
		theApp.Training.OnAfterVector = OnAfterVector;
		theApp.Training.OnBeforeEpoch = OnBeforeEpoch;
		theApp.Training.OnAfterEpoch = OnAfterEpoch;
		theApp.Training.OnErms = OnErms;

		UpdateData(FALSE);

		if (theApp.Training.StartTraining(&theApp.Net, this) >= 0)
		{
			theApp.MainFrame->m_NetModified = true;
		}
	}
	else
	{
		if (theApp.Training.StopTraining(false, false) >= 0)
		{
		}
	}
}

void CTeachDlg::OnCancel()
{
	if (theApp.Training.IsRunning())
		return;

	if (theApp.MainFrame->DoTeachEndDialog(IDOK) >= 0)
		DestroyWindow();
}

void CTeachDlg::Log(CString Text, bool AddTime, bool Display)
{
	CString Output;

	if (AddTime)
	{
		CTime Time = CTime::GetCurrentTime();
		Output.Format("[%s] %s", Time.Format("%H:%M:%S"), Text);
	}
	else
	{
		Output = Text;
	}

	if (Display)
	{
		if (m_LoggerBox.GetCount() > 128)
			m_LoggerBox.DeleteString(0);

		m_LoggerBox.SetCaretIndex(m_LoggerBox.AddString(Output));
	}

	fprintf(m_Log, "%s\n", Output);
}

void CTeachDlg::SetVector(int Vector)
{
	int Max, Min;
	CString Text;

	m_VectorProgress.SetPos(Vector);
	m_VectorProgress.GetRange(Min, Max);

	Text.Format(IDS_TEACH_VECTOR, Vector + 1, Max);
	GetDlgItem(IDC_TEACH_EPOCH_STATUS)->SetWindowText(Text);
}

void CTeachDlg::SetEpoch(int Epoch)
{
	CString Text;
	int Max, Min;

	m_EpochProgress.SetPos(Epoch);
	m_EpochProgress.GetRange(Min, Max);

	Text.Format(IDS_TEACH_EPOCH, Epoch + 1, Max);
	GetDlgItem(IDC_TEACH_STATUS)->SetWindowText(Text);
}

void CTeachDlg::SetAlfa(float Alfa)
{
	CString Text;
	
	Text.Format("%.4f", Alfa);
	GetDlgItem(IDC_TEACH_ALFA)->SetWindowText(Text);
}

void CTeachDlg::SetEta(float Eta)
{
	CString Text;
	
	Text.Format("%.4f", Eta);
	GetDlgItem(IDC_TEACH_ETA)->SetWindowText(Text);
}

void CTeachDlg::SetErms(float Erms)
{
	CString Text;
	
	Text.Format("%.4f", Erms);
	GetDlgItem(IDC_TEACH_ERMS)->SetWindowText(Text);
}

void CTeachDlg::SetMaxErms(float MaxErms)
{
	CString Text;
	
	Text.Format("%.4f", MaxErms);
	GetDlgItem(IDC_TEACH_MAXERMS)->SetWindowText(Text);
}

int CTeachDlg::DoOnStart(CTraining *Training, int Vectors)
{
	char *Ext;
	CString Text;

	m_LogName = theApp.MainFrame->m_NetFile;
	
	Ext = strrchr(m_LogName.GetBuffer(), '.');
	
	if (Ext != NULL)
	{
		m_LogName.Replace(Ext, ".log");
	}
	else
		m_LogName += ".log";

	m_Log = fopen(m_LogName, "a");

	if (m_Log == NULL)
		return -1;

	Log("#", false, false);
	Log("# NeuralNet 1.0", false, false);
	Log("# Miroslaw Szajner (noozy@tlen.pl)", false, false);
	Log("# Lukasz Wojsa", false, false);
	Log("# 2FD 2004/2005", false, false);
	Log("#", false, false);
	Text.Format(IDS_LOG_NETFILE, theApp.MainFrame->m_NetFile);
	Log(Text, false, false);
	Log("#", false, false);
	Text.Format(IDS_LOG_NETTYPE);
	Log(Text, false, false);
	Text.Format(IDS_LOG_NETINPUTS, theApp.Net.GetInputs());
	Log(Text, false, false);
	Text.Format(IDS_LOG_NETOUTPUTS, theApp.Net.GetOutputs());
	Log(Text, false, false);
	Text.Format(IDS_LOG_NETLAYERS, theApp.Net.Layers.GetCount() + 1);
	Log(Text, false, false);
	Text.Format(IDS_LOG_LAYERNEURONS, 0, theApp.Net.GetInputs());
	Log(Text, false, false);
	for (int i = 0; i < theApp.Net.Layers.GetCount(); i++)
	{
		Text.Format(IDS_LOG_LAYERNEURONS, i + 1, theApp.Net.Layers[i].GetNeurons());
		Log(Text, false, false);
	}
	LPCACTFUNCTION func = GetFunction(theApp.Net.GetFunction());
	Text.Format(IDS_LOG_NETFUNCTION, func->Name, theApp.Net.Beta);
	Log(Text, false, false);
	Log("#", false, false);

	Text.Format(IDS_LOG_TRAININGFILE, theApp.MainFrame->m_TrainingFile);
	Log(Text, false, false);
	Log("#", false, false);
	Text.Format(IDS_LOG_TRAININGALGORITHM);
	Log(Text, false, false);
	Text.Format(IDS_LOG_TRAININGERROR);
	Log(Text, false, false);
	if (Training->GetChangeEta())
		Text.Format(IDS_LOG_CHANGEETA, theApp.Net.Eta, Training->GetEndEta());
	else
		Text.Format(IDS_LOG_CONSTETA, theApp.Net.Eta);
	Log(Text, false, false);
	if (Training->GetChangeAlfa())
		Text.Format(IDS_LOG_CHANGEALFA, theApp.Net.Alfa, Training->GetEndAlfa());
	else
		Text.Format(IDS_LOG_CONSTALFA, theApp.Net.Alfa);
	Log(Text, false, false);
	Text.Format(IDS_LOG_EPOCHS, Training->GetEpochs());
	Log(Text, false, false);
	if (Training->GetBreakErms())
	{
		Text.Format(IDS_LOG_MAXERMS, Training->GetMaxErms());
		Log(Text, false, false);
	}
	Text.Format(IDS_LOG_REPEATS, Training->GetRepeats());
	Log(Text, false, false);
	Text.Format(IDS_LOG_WIDTH, Training->GetWidth());
	Log(Text, false, false);
	Text.Format(IDS_LOG_HEIGHT, Training->GetHeight());
	Log(Text, false, false);
	Log("#", false, false);
	Log("#", false, false);

	Text.LoadString(IDS_TEACH_START);
	Log(Text);
	
	Text.Format(IDS_TEACH_VECTORS, Vectors);
	Log(Text);

	Text.LoadString(IDS_STOP_TRAINING);
	GetDlgItem(IDC_TEACH_OK)->SetWindowText(Text);
	GetDlgItem(IDC_TEACH_CANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEACH_RANDOM)->EnableWindow(FALSE);

	m_VectorProgress.SetRange32(0, Vectors);
	m_EpochProgress.SetRange32(0, Training->GetEpochs());

	SetVector(0);
	SetEpoch(0);

	m_StartTime = time(NULL);

	return 0;
}

int CTeachDlg::DoOnStop(CTraining *Training)
{
	CString Text;

	Text.LoadString(IDS_START_TRAINING);
	GetDlgItem(IDC_TEACH_OK)->SetWindowText(Text);
	GetDlgItem(IDC_TEACH_CANCEL)->EnableWindow(TRUE);
	GetDlgItem(IDC_TEACH_SHOWLOG)->EnableWindow(TRUE);
	GetDlgItem(IDC_TEACH_RANDOM)->EnableWindow(TRUE);

	Text.LoadString(IDS_TEACH_STOP);
	Log(Text);

	SetVector(0);
	SetEpoch(0);
	fclose(m_Log);

	return 0;
}

int CTeachDlg::DoOnBeforeVector(CTraining *Training, CVector *Vector)
{
	CString Name;
	CString Text;
		
	Vector->GetName(Name);
	Text.Format(IDS_TEACH_VECTOR_NAME, Name);
	Log(Text, true, m_ShowAll ? true : false);

	return 0;
}

int CTeachDlg::DoOnAfterVector(CTraining *Training, CVector *Vector)
{
	CString Text;

	SetVector(m_VectorProgress.GetPos() + 1);

	m_TeachTime = time(NULL) - m_StartTime;

	Text = m_TeachTime.Format("%H:%M:%S");

	if (m_EndTime != 0)
	{
		Text += " / " + m_EndTime.Format("%H:%M:%S");
	}

	GetDlgItem(IDC_TEACH_TIME)->SetWindowText(Text);

	return 0;
}

int CTeachDlg::DoOnBeforeEpoch(CTraining *Training, int Epoch)
{
	CString Text;

	Text.Format(IDS_TEACH_EPOCH_NUMBER, Epoch + 1);
	Log(Text);

	SetVector(0);

	return 0;
}

int CTeachDlg::DoOnAfterEpoch(CTraining *Training, int Epoch)
{
	CString Text;

	SetEpoch(m_EpochProgress.GetPos() + 1);

	SetAlfa(theApp.Net.Alfa);
	SetEta(theApp.Net.Eta);
	SetErms(theApp.Net.Erms);

	Text.Format(IDS_TEACH_ERMS, Epoch + 1, theApp.Net.Erms);
	Log(Text, true, m_ShowAll ? true : false);

	m_EndTime = (time_t)(((double)(time(NULL) - 
		m_StartTime) / (double)(Epoch + 1)) * (double)Training->GetEpochs());

	return 0;
}

int CTeachDlg::DoOnErms(CTraining *Training, float Erms)
{
	int Epoch = m_EpochProgress.GetPos();
	CString Text, End;

	End.Format((Epoch == 1) ? IDS_FORMAT_EPOCH : IDS_FORMAT_EPOCHS);
	Text.Format(IDS_TEACH_ONERMS, Erms, Epoch, End);

	Log(Text);

	return 0;
}

void CTeachDlg::OnShowAll()
{
	UpdateData(TRUE);
}

void CTeachDlg::OnShowLog()
{
	if (!theApp.Training.IsRunning())
		ShellExecute(NULL, "open", m_LogName, NULL, NULL, SW_SHOW);
}

void CTeachDlg::OnRandom()
{
	// random weights
	theApp.Net.Random();
}

BOOL CTeachDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetAlfa(theApp.Net.Alfa);
	SetEta(theApp.Net.Eta);
	SetMaxErms(theApp.Training.GetMaxErms());
	SetErms(0.0f);

	return TRUE;
}

void CTeachDlg::OnTeachOk()
{
	OnOK();
}

void CTeachDlg::OnTeachCancel()
{
	OnCancel();
}
