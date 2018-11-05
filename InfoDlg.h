//
// InfoDlg.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy dialogu informacji o sieci.
//

#pragma once

//
// CInfoDlg dialog
//
class CInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInfoDlg)

private:
	CNet *m_Net;
	int m_Inputs;
	int m_Outputs;
	int m_Layers;
	int m_Neurons;
	int m_Width;
	int m_Height;
	CString m_Function;
	float m_Alfa;
	float m_Beta;
	float m_Eta;

public:
	CInfoDlg(CWnd* pParent = NULL); // standard constructor
	virtual ~CInfoDlg(); // destructor

	// Dialog Data
	enum { IDD = IDD_INFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

	virtual BOOL OnInitDialog();

	virtual void OnOK(void);
	virtual void OnCancel(void);

public:
	void SetNet(CNet *Net); // sets net to be monitored
	void UpdateInfo(void); // updates controls

protected:
	DECLARE_MESSAGE_MAP() // message map
};
