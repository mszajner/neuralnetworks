#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CRecognizeDlg dialog

class CRecognizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CRecognizeDlg)

private:
	CImage m_Display;
	CString m_FileName;
	CListBox m_Outputs;
	CVector m_Vector;
	BOOL m_Monochrome;
	CSliderCtrl m_BiasCtrl;
	BOOL m_Black;
	int m_Bias;

public:
	CRecognizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRecognizeDlg();

// Dialog Data
	enum { IDD = IDD_RECOGNIZEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	void UpdateDisplay(void);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRecognize();
	afx_msg void OnRecognizeClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRecognizeMonochrome();
	afx_msg void OnPaint();
	afx_msg void OnRecognizeBrowse();
	virtual BOOL DestroyWindow();
private:
	CString m_Text;
};
