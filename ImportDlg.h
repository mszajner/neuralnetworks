//
// ImportDlg.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy dialogu importu wektorów.
//

#pragma once

#include "afxwin.h"
#include "atlimage.h"
#include "afxcmn.h"

//
// CImportDlg dialog
//
class CImportDlg : public CDialog
{
	DECLARE_DYNAMIC(CImportDlg)

private:
	CNet       *m_Net;          // neural net
	CTraining  *m_Training;     // training
	CSliderCtrl m_BiasCtrl;     // bias slider
	CComboBox   m_OutputCtrl;   // output select combo
	CString     m_FileName;     // full file name
	CString     m_Name;         // vector name
	CImage      m_Display;      // image
	int         m_Monochrome;   // != 0 if vector has to be monochrome
	int         m_Black;        // != 0 if vector has to be black active
	int         m_Testing;      // != 0 if vector has to be testing
	int         m_Bias;         // color bias
	int         m_Output;       // active output

public:
	CImportDlg(CNet *Net, CTraining *Training, CWnd* pParent = NULL); // standard constructor
	virtual ~CImportDlg(); // destructor

	// Dialog Data
	enum { IDD = IDD_IMPORTDLG };

	virtual INT_PTR DoModal(int Output = -1, bool BrowseFile = false); // do modal

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	virtual BOOL OnInitDialog(); // initialize dialog
	void UpdateDisplay(void); // update display
	INT_PTR DoImport(CString FileName, bool Reset);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnImportMonochrome();
	afx_msg void OnImportOutput();
	afx_msg void OnImportAll();
	afx_msg void OnImportSkip();
	afx_msg void OnImportOk();
	afx_msg void OnImportCancel();
};
