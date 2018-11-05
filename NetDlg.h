//
// NetDlg.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy dialogu kreatora sieci.
//

#pragma once

#include "Net.h"

// CNetDlg dialog

class CNetDlg : public CDialog
{
	DECLARE_DYNAMIC(CNetDlg)

private:
	float m_Alfa;
	float m_Beta;
	float m_Eta;
	int m_Inputs;
	int m_Outputs;
	int m_Neurons;
	int m_Width;
	int m_Height;
	CComboBox m_Function;
	CListBox m_Layers;
	BOOL m_Perceptron;
	BOOL m_Ebp;
	BOOL m_Quadra;
	CListBox m_Categories;
	CEdit m_Category;
	CNet *m_Net;

public:
	bool m_Edit;

public:
	CNetDlg(CWnd* pParent = NULL); // standard constructor
	virtual ~CNetDlg(); // standard destructor

	// Dialog Data
	enum { IDD = IDD_NETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

	virtual BOOL OnInitDialog(); // initialize dialog

	virtual void OnOK();
	virtual void OnCancel();

private:
	void SelectFunction(int Id); // select function in combo box

public:
	void CreateNet(CNet *Net); // creates new net
	void EditNet(CNet *Net); // edits specified net

protected:
	DECLARE_MESSAGE_MAP() // message map for dialog

public:
	afx_msg void OnNetOk();
	afx_msg void OnNetCancel();
	afx_msg void OnNetAdd();
	afx_msg void OnNetDelete();
	afx_msg void OnInputsSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOutputsSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNeuronsSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLayersSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnWidthSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHeightSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeOutputs();
	afx_msg void OnChangeCategories();
	afx_msg void OnChangeCategory();
};
