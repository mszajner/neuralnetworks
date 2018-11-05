#pragma once

// CTrainingDlg dialog

class CTrainingDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrainingDlg)

private:
	BOOL m_ChangeAlfa;
	BOOL m_ChangeEta;
	float m_EndAlfa;
	float m_EndEta;
	int m_Epochs;
	BOOL m_BreakErms;
	float m_MaxErms;
	BOOL m_ChangeBeta;
	float m_Beta;
	BOOL m_ChangeFunction;
	CComboBox m_Function;
	int m_Width;
	int m_Height;
	CString m_Info;
	int m_Repeats;

public:
	// default constructor
	CTrainingDlg(CWnd* pParent = NULL);

	// destructor
	virtual ~CTrainingDlg();

	// Dialog Data
	enum { IDD = IDD_TRAININGDLG };

	// editing flag
	BOOL m_bEdit;

protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()

	void SelectFunction(int Id);

public:
	// create training
	int CreateTraining(void);

	// edit training
	int EditTraining(void);

public:
	afx_msg void OnEpochsSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeAlfa();
	afx_msg void OnChangeEta();
	afx_msg void OnBreakErms();
	afx_msg void OnChangeBeta();
	afx_msg void OnChangeFunction();
	afx_msg void OnWidthSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHeightSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeSize();
	afx_msg void OnVectorsList();
	afx_msg void OnRepeatsSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRandom();
};
