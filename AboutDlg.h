//
// AboutDlg.h
//
// Autorzy:       Miros�aw Szajner (noozy@tlen.pl)
//                �ukasz Wojsa
//
// Rok studi�w:   2FD 2004/2005
//
// Plik nag��wkowy dialogu informacji o programie.
//

#pragma once

//
// CAboutDlg dialog
//
class CAboutDlg : public CDialog
{
public:
	CAboutDlg(); // standard constructor

	// Dialog Data
	enum { IDD = IDD_ABOUTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	DECLARE_MESSAGE_MAP()
};
