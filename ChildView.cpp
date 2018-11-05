//
// ChildView.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Implementacja klasy CChildView.
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
// CChildView
//

CChildView::CChildView() : m_Dialog(NULL)
{
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CChildView message handlers

void CChildView::SetDialog(CDialog *Dialog)
{
	// store pointer to dialog
	m_Dialog = Dialog;

	// update dialog
	SendMessage(WM_SIZE);
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// try to create struct
	if (!CWnd::PreCreateWindow(cs))
		// return false -- error
		return FALSE;

	// extended style
	cs.dwExStyle |= WS_EX_CLIENTEDGE;

	// exclude window border
	cs.style &= ~WS_BORDER;

	// register window class
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_BTNFACE+1), NULL);

	// return true -- ok
	return TRUE;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CRect Rect;

	// size window
	CWnd::OnSize(nType, cx, cy);

	// if there is dialog inside
	if (IsDialog())
	{
		CRect dlgSize;
		CRect wndSize;
		int Width, Height;

		// get window client rect
		GetClientRect(&wndSize);

		// get dialog rect
		m_Dialog->GetWindowRect(&dlgSize);

		// get dialog width
		Width = dlgSize.Width();

		// get dialog height
		Height = dlgSize.Height();

		// move dialog to center
		m_Dialog->MoveWindow((wndSize.Width()/2)-(Width/2),
			(wndSize.Height()/2)-(Height/2), Width, Height);
	}
}
