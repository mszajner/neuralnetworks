#pragma once

#include "afxwin.h"
#include "atlimage.h"

// CVectorsDlg dialog

class CVectorsDlg : public CDialog
{
	DECLARE_DYNAMIC(CVectorsDlg)

private:
	CTraining *m_Training;
	CListBox m_OutputsList;
	CListBox m_VectorsList;
	CString m_OutputName;
	CString m_VectorName;
	CImage m_Image;
	CVector *m_Vector;
	int m_VectorIndex;
	int m_Output;
	int m_OutputIndex;
	BOOL m_VectorTesting;
	CList<CVector*,CVector*> m_CopyList;

public:
	// standard constructor
	CVectorsDlg(CWnd* pParent = NULL);

	// desttructor
	virtual ~CVectorsDlg();

	// Dialog Data
	enum { IDD = IDD_VECTORSDLG };

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);

	// initialize dialog
	virtual BOOL OnInitDialog();

	// on ok message
	virtual void OnOK();

	// on cancel message
	virtual void OnCancel();

	// on destroy window
	virtual BOOL DestroyWindow();

	// message map
	DECLARE_MESSAGE_MAP()

private:
	void DoChangeOutputsList(int Item = -1);
	void DoChangeVectorsList(int Item = -1);

public:
	afx_msg void OnChangeOutputsList();
	afx_msg void OnChangeVectorsList();
	afx_msg void OnDeleteFile();
	afx_msg void OnPaint();
	afx_msg void OnClearTraining();
	afx_msg void OnVectorTesting();
	afx_msg void OnTrainingSettings();
	afx_msg void OnAddFile();
	afx_msg void OnAddFolder();
	afx_msg void OnCut();
	afx_msg void OnPaste();
	afx_msg void OnVectorName();
private:
	CEdit m_VectorEdit;
};
