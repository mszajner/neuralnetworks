//
// NeuralNet.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Implementacja klasy CNeuralNet.
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "MainFrm.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only CNeuralNetApp object

CNeuralNetApp theApp;

//
// CNeuralNetApp
//
CNeuralNetApp::CNeuralNetApp()
{
}

BOOL CNeuralNetApp::InitInstance()
{
	InitCommonControls();

	CWinApp::InitInstance();

	// Standard initialization
	SetRegistryKey(_T("NeuralNet"));

	// initialize for SHBrowseForFolder
	CoInitialize(NULL);

	// sets current directory
	// SetCurrentDirectory();

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;

	// if frame create fails
	if (!pFrame)
		// return false -- error
		return FALSE;

	// store pointer to main frame
	m_pMainWnd = MainFrame = pFrame;

	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW 
		| FWS_ADDTOTITLE, NULL, NULL);

	// set main frame icon
	pFrame->SetIcon(LoadIcon(IDR_MAINFRAME), FALSE);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_MAXIMIZE);

	// update main frame
	pFrame->UpdateWindow();

	// return true -- ok
	return TRUE;
}

BEGIN_MESSAGE_MAP(CNeuralNetApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()

// CNeuralNetApp message handlers

void CNeuralNetApp::OnAppAbout()
{
	CAboutDlg AboutDlg;
	AboutDlg.DoModal();
}
