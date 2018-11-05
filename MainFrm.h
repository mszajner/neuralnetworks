//
// MainFrm.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy g³ównego okna aplikacji.
//

#pragma once

#include "ChildView.h"
#include "NetDlg.h"
#include "InfoDlg.h"
#include "TrainingDlg.h"
#include "VectorsDlg.h"
#include "TeachDlg.h"
#include "TestingDlg.h"
#include "RecognizeDlg.h"

class CMainFrame : public CFrameWnd
{
protected: 
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame(); // default constructor
	virtual ~CMainFrame(); // destructor

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs); // precreate method
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo); // on command message

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CStatusBar m_wndStatusBar;
	CToolBar m_wndToolBar;
	CReBar m_wndReBar;
	CChildView m_wndView;
	CDialogBar m_wndDlgBar;
	CNetDlg m_NetDlg;
	CInfoDlg m_InfoDlg;
	CTrainingDlg m_TrainingDlg;
	CVectorsDlg m_VectorsDlg;
	CTeachDlg m_TeachDlg;
	CTestingDlg m_TestingDlg;
	CRecognizeDlg m_RecognizeDlg;

public:
	CString m_NetFile;
	CString m_TrainingFile;
	BOOL m_NetCreated;
	BOOL m_TrainingCreated;
	BOOL m_NetModified;
	BOOL m_TrainingModified;

public:
	int DoNetNew(void);
	int DoNetEdit(void);
	int DoNetOpen(void);
	int DoNetSave(CString FileName);
	int DoNetSaveAs(void);
	int DoNetPrompt(void);
	int DoNetEndDialog(UINT Result);
	int DoTrainingNew(void);
	int DoTrainingEdit(void);
	int DoTrainingOpen(void);
	int DoTrainingSave(CString FileName, bool SaveAs = false);
	int DoTrainingSaveAs(void);
	int DoTrainingPrompt(void);
	int DoTrainingEndDialog(UINT Result);
	int DoVectorsEndDialog(UINT Result);
	int DoTeachEndDialog(UINT Result);
	int DoTestingEndDialog(UINT Result);
	int DoRecognizeEndDialog(UINT Result);
	BOOL CheckNetCreated(void);
	BOOL CheckTrainingCreated(void);
	bool TrainingMustBeSaved(void);

private:
	void SetNetFile(CString FileName);
	void SetTrainingFile(CString FileName);
	void UpdateCaption(void);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNetNew();
	afx_msg void OnNetEdit();
	afx_msg void OnNetOpen();
	afx_msg void OnNetSave();
	afx_msg void OnNetSaveAs();
	afx_msg void OnTrainingNew();
	afx_msg void OnTrainingEdit();
	afx_msg void OnTrainingOpen();
	afx_msg void OnTrainingSave();
	afx_msg void OnTrainingSaveAs();
	afx_msg void OnVectorsFile();
	afx_msg void OnVectorsFolder();
	afx_msg void OnVectorsList();
	afx_msg void OnTeach();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTesting();
	afx_msg void OnRecognize();
};
