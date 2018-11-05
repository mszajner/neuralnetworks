#pragma once

#include "afxwin.h"

// CTestingDlg dialog

class CTestingDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestingDlg)

private:
	CListBox m_Vectors;
	CListBox m_Symbols;
	CImage m_Image;

public:
	CTestingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestingDlg();

// Dialog Data
	enum { IDD = IDD_TESTINGDLG };

protected:
	void Clear(void);
	void UpdateOutputs(void);

	int DoOnStart(CTraining *Training, int Vectors);
	int DoOnStop(CTraining *Training);
	int DoOnBeforeVector(CTraining *Training, CVector *Vector);
	int DoOnAfterVector(CTraining *Training, CVector *Vector);

	friend int __stdcall OnStartTesting(CTraining *Training, int Vectors, void *Parameter);
	friend int __stdcall OnStopTesting(CTraining *Training, void *Parameter);
	friend int __stdcall OnBeforeVectorTesting(CTraining *Training, CVector *Vector, void *Parameter);
	friend int __stdcall OnAfterVectorTesting(CTraining *Training, CVector *Vector, void *Parameter);

protected:
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnTest();
	afx_msg void OnClose();
	afx_msg void OnVectorsChange();
	afx_msg void OnSymbolsChange();
private:
	CListBox m_Outputs;
public:
	afx_msg void OnPaint();
	virtual BOOL DestroyWindow();
};
