//
// NeuralNet.h
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Plik nag³ówkowy klasy aplikacji.
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// main symbols
#include "resource.h"

// global classes
#include "Net.h"
#include "Training.h"

// windows
#include "MainFrm.h"

//
// CNeuralNetApp
//
class CNeuralNetApp : public CWinApp
{
public:
	// net object
	CNet Net;

	// training object
	CTraining Training;

	// main frame
	CMainFrame *MainFrame;

public:
	CNeuralNetApp(); // default constructor

public:
	virtual BOOL InitInstance(); // initialize instance

	DECLARE_MESSAGE_MAP() // message map

	afx_msg void OnAppAbout();
};

// global application object
extern CNeuralNetApp theApp;
