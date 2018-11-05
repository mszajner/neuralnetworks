//
// AboutDlg.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy dialogu informacji o programie.
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
