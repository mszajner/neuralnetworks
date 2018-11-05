//
// ChildView.h
//
// Autorzy:       Miros�aw Szajner (noozy@tlen.pl)
//                �ukasz Wojsa
//
// Rok studi�w:   2FD 2004/2005
//
// Plik nag��wkowy okna wewn�trznego aplikacji.
//

#pragma once

//
// CChildView window
//
class CChildView : public CWnd
{
private:
	CDialog *m_Dialog;

public:
	CChildView(); // standard constructor
	virtual ~CChildView(); // destructor

	void SetDialog(CDialog *Dialog); // set dialog to manage

	bool IsDialog(void) { // check if there is a dialog
		return m_Dialog && m_Dialog->m_hWnd;
	}

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs); // precreate method

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy); // OnSize handler
};
