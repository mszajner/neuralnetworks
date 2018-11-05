#pragma once

#include <time.h>
#include "afxwin.h"
#include "afxcmn.h"

// CTeachDlg dialog

class CTeachDlg : public CDialog
{
	DECLARE_DYNAMIC(CTeachDlg)

private:
	FILE         *m_Log;
	CString       m_LogName;
	time_t        m_StartTime;
	CTimeSpan     m_TeachTime;
	CTimeSpan     m_EndTime;
	CListBox      m_LoggerBox;
	CProgressCtrl m_VectorProgress;
	CProgressCtrl m_EpochProgress;

public:
	CTeachDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTeachDlg();

	// Dialog Data
	enum { IDD = IDD_TEACHDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnOK();
	virtual void OnCancel();

	void SetVector(int Vector);
	void SetEpoch(int Epoch);
	void SetAlfa(float Alfa);
	void SetEta(float Eta);
	void SetErms(float Erms);
	void SetMaxErms(float MaxErms);

	void Log(CString Text, bool AddTime = true, bool Display = true);

private:
	int DoOnStart(CTraining *Training, int Vectors);
	int DoOnStop(CTraining *Training);
	int DoOnBeforeVector(CTraining *Training, CVector *Vector);
	int DoOnAfterVector(CTraining *Training, CVector *Vector);
	int DoOnBeforeEpoch(CTraining *Training, int Epoch);
	int DoOnAfterEpoch(CTraining *Training, int Epoch);
	int DoOnErms(CTraining *Training, float Erms);

public:
	friend int __stdcall OnStart(CTraining *Training, int Vectors, void *Parameter);
	friend int __stdcall OnStop(CTraining *Training, void *Parameter);
	friend int __stdcall OnBeforeVector(CTraining *Training, CVector *Vector, void *Parameter);
	friend int __stdcall OnAfterVector(CTraining *Training, CVector *Vector, void *Parameter);
	friend int __stdcall OnBeforeEpoch(CTraining *Training, int Epoch, void *Parameter);
	friend int __stdcall OnAfterEpoch(CTraining *Training, int Epoch, void *Parameter);
	friend int __stdcall OnErms(CTraining *Training, float Epoch, void *Parameter);

private:
	BOOL m_ShowAll;
public:
	afx_msg void OnShowAll();
	afx_msg void OnShowLog();
	afx_msg void OnRandom();
	virtual BOOL OnInitDialog();
	afx_msg void OnTeachOk();
	afx_msg void OnTeachCancel();
};
