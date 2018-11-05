//
// NetDlg.cpp
//
// Autorzy:       Miros³aw Szajner (noozy@tlen.pl)
//                £ukasz Wojsa
//
// Rok studiów:   2FD 2004/2005
//
// Implemetnacja klasy CNetDlg.
//

#include "stdafx.h"
#include "NeuralNet.h"
#include "NetDlg.h"

// CNetDlg dialog

IMPLEMENT_DYNAMIC(CNetDlg, CDialog)
CNetDlg::CNetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetDlg::IDD, pParent)
	, m_Alfa(0.4f)
	, m_Beta(3.0f)
	, m_Eta(0.2f)
	, m_Inputs(1)
	, m_Outputs(1)
	, m_Neurons(1)
	, m_Edit(false)
	, m_Width(1)
	, m_Height(1)
	, m_Perceptron(0)
	, m_Ebp(0)
	, m_Quadra(0)
{
}

CNetDlg::~CNetDlg()
{
}

void CNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_NET_FUNCTION, m_Function);
	DDX_Control(pDX, IDC_NET_LAYERS, m_Layers);
	DDX_Text(pDX, IDC_NET_ALFA, m_Alfa);
	DDX_Text(pDX, IDC_NET_BETA, m_Beta);
	DDX_Text(pDX, IDC_NET_ETA, m_Eta);
	DDX_Text(pDX, IDC_NET_INPUTS, m_Inputs);
	DDX_Text(pDX, IDC_NET_OUTPUTS, m_Outputs);
	DDX_Text(pDX, IDC_NET_NEURONS, m_Neurons);
	DDX_Text(pDX, IDC_NET_WIDTH, m_Width);
	DDX_Text(pDX, IDC_NET_HEIGHT, m_Height);
	DDX_Radio(pDX, IDC_NET_PERCEPTRON, m_Perceptron);
	DDX_Radio(pDX, IDC_NET_EBP, m_Ebp);
	DDX_Radio(pDX, IDC_NET_QUADRA, m_Quadra);
	DDX_Control(pDX, IDC_NET_CATEGORIES, m_Categories);
	DDX_Control(pDX, IDC_NET_CATEGORY, m_Category);
}

BOOL CNetDlg::OnInitDialog()
{
	unsigned int Id = 0;
	LPCACTFUNCTION lpFunction;

	// call ancestor function
	CDialog::OnInitDialog();

	// for each function
	while (lpFunction = GetFunction(Id++))
	{
		// add function name to combo box
		int Item = m_Function.AddString(lpFunction->Name);
		
		// if added
		if (Item >= 0)
			// set item data as function id
			m_Function.SetItemData(Item, lpFunction->Id);
	}

	// select default
	SelectFunction(FUNCTION_DEFAULT);

	// update controls
	UpdateData(FALSE);

	// set focus to this dialog
	SetFocus();

	return TRUE;
}

void CNetDlg::OnOK()
{
	// get values from controls
	UpdateData(TRUE);

	// check image size
	if (m_Width * m_Height != m_Inputs)
	{
		CString Error;

		// load error message
		Error.LoadString(IDS_NET_ERROR_IMAGE_SIZE);

		// show error message
		MessageBox(Error, 0, MB_ICONERROR | MB_OK);

		// break function
		return;
	}

	// if editing net
	if (m_Edit)
	{
		CString Prompt;

		// load prompt
		Prompt.LoadString(IDS_NET_PROMPT_EDIT);

		// prompt for editing
		if (MessageBox(Prompt, 0, MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
			// if no exit
			return;
	}
	else
	{
		// prompt for save previous
		if (theApp.MainFrame->DoNetPrompt() < 0)
			// break
			return;
	}

	// set Alfa
	m_Net->Alfa = m_Alfa;

	// set Beta
	m_Net->Beta = m_Beta;

	// set Eta
	m_Net->Eta = m_Eta;

	// set Width
	m_Net->Width = m_Width;

	// set Height
	m_Net->Height = m_Height;

	// get selected function
	int Item = m_Function.GetCurSel();

	// get id of selected function
	int Id = (int)m_Function.GetItemData(Item);

	// set function
	m_Net->SetFunction(Id);

	// set number of layers
	m_Net->Layers.SetSize(m_Layers.GetCount() + 1);

	// get inputs of net
	unsigned int Current, Previous = m_Inputs;

	// for each layer
	for (Item = 0; Item < m_Layers.GetCount(); Item++)
	{
		// get number of neurons for this layer
		Current = (unsigned int)m_Layers.GetItemData(Item);

		// set number of neurons in this layer
		m_Net->Layers[Item].SetNeurons(Current);

		// set inputs of layer
		m_Net->Layers[Item].SetInputs(Previous);

		// store current number of neurons for next layer
		Previous = Current;
	}

	// set inputs
	m_Net->SetInputs(m_Inputs);

	// set outputs
	m_Net->SetOutputs(m_Outputs);

	// set inputs number for last layer
	m_Net->Layers[Item].SetInputs(Previous);

	for (Item = 0; Item < m_Outputs; Item++)
	{
		CString Text;

		// get text from categories list box
		m_Categories.GetText(Item, Text);

		// store text to net
		m_Net->Categories[Item] = Text;
	}

	// random weights
	m_Net->Random();

	// end dialog
	if (theApp.MainFrame->DoNetEndDialog(IDOK) >= 0)
		// destroy dialog
		DestroyWindow();
}

void CNetDlg::OnCancel()
{
	// end dialog
	if (theApp.MainFrame->DoNetEndDialog(IDCANCEL) >= 0)
		// destroy dialog
		DestroyWindow();
}

void CNetDlg::SelectFunction(int Id)
{
	// get functions number
	int I, Count = m_Function.GetCount();

	// for each function in combo box
	for (I = 0; I < Count; I++)
	{
		// check id match
		if (m_Function.GetItemData(I) == Id)
		{
			// select item
			m_Function.SetCurSel(I);

			// break for-loop
			break;
		}
	}
}

void CNetDlg::CreateNet(CNet *Net)
{
	// store pointer to net
	m_Net = Net;

	ASSERT(m_Net);

	// creating new net
	m_Edit = false;

	// set default value
	m_Alfa = 0.4f;
	m_Beta = 3.0f;
	m_Eta = 0.2f;
	m_Inputs = 1;
	m_Outputs = 1;
	m_Neurons = 1;
	m_Width = 1;
	m_Height = 1;

	// update controls
	UpdateData(FALSE);

	// update outputs list
	OnChangeOutputs();
}

void CNetDlg::EditNet(CNet *Net)
{
	// store pointer to net
	m_Net = Net;

	ASSERT(m_Net);

	// editing existing net
	m_Edit = true;

	// get alfa
	m_Alfa = m_Net->Alfa;

	// get Beta
	m_Beta = m_Net->Beta;

	// get Eta
	m_Eta = m_Net->Eta;

	// set Width
	m_Width = m_Net->Width;

	// set Height
	m_Height = m_Net->Height;

	// select function
	SelectFunction(m_Net->GetFunction());

	// get number of inputs
	m_Inputs = m_Net->GetInputs();

	// get number of outputs
	m_Outputs = m_Net->GetOutputs();

	// for each layer in net (without last)
	for (int i = 0; i < m_Net->Layers.GetCount() - 1; i++)
	{
		int Position;
		CString Text, Format;

		// load format text
		Format.LoadString(IDS_NET_LAYER);

		// prepare text
		Text.Format(Format, m_Net->Layers[i].GetNeurons());

		// add text to list box
		Position = m_Layers.AddString(Text);

		// if added
		if (Position >= 0)
			// set item data as number of neurons
			m_Layers.SetItemData(Position, m_Net->Layers[i].GetNeurons());
	}

	// for each output
	for (i = 0; i < m_Outputs; i++)
		// add output string
		m_Categories.AddString(m_Net->Categories[i]);

	// update controls
	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CNetDlg, CDialog)
	ON_BN_CLICKED(IDC_NET_OK, OnNetOk)
	ON_BN_CLICKED(IDC_NET_CANCEL, OnNetCancel)
	ON_BN_CLICKED(IDC_NET_ADD, OnNetAdd)
	ON_BN_CLICKED(IDC_NET_DEL, OnNetDelete)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NET_INPUTS_SPIN, OnInputsSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NET_OUTPUTS_SPIN, OnOutputsSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NET_NEURONS_SPIN, OnNeuronsSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NET_LAYERS_SPIN, OnLayersSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NET_WIDTH_SPIN, OnWidthSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_NET_HEIGHT_SPIN, OnHeightSpin)
	ON_EN_CHANGE(IDC_NET_OUTPUTS, OnChangeOutputs)
	ON_LBN_SELCHANGE(IDC_NET_CATEGORIES, OnChangeCategories)
	ON_EN_CHANGE(IDC_NET_CATEGORY, OnChangeCategory)
END_MESSAGE_MAP()

// CNetDlg message handlers

void CNetDlg::OnNetOk()
{
	OnOK();
}

void CNetDlg::OnNetCancel()
{
	OnCancel();
}

void CNetDlg::OnNetAdd()
{
	int Position;
	CString Text, Format;

	// get values from controls
	UpdateData(TRUE);

	// load format string
	Format.LoadString(IDS_NET_LAYER);

	// prepare text to add
	Text.Format(Format, m_Neurons);

	// add text to list box
	Position = m_Layers.AddString(Text);

	// if added
	if (Position >= 0)
		// set item data as number of neurons
        m_Layers.SetItemData(Position, (DWORD_PTR)m_Neurons);
}

void CNetDlg::OnNetDelete()
{
	// get number of layers
	int I, Count = m_Layers.GetCount();

	// for each item
	for (I = 0; I < Count; I++)
	{
		// check if selected
		if (m_Layers.GetSel(I))
		{
			// delete selected
			m_Layers.DeleteString(I--);

			// decrement number of layers
			Count--;
		}
	}
}

void CNetDlg::OnInputsSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// get values from controls
	UpdateData(TRUE);

	// update inputs variable
	m_Inputs -= pNMUpDown->iDelta;

	// check if inputs is less than one
	if (m_Inputs < 1)
		// set one
		m_Inputs = 1;

	// check if inputs is greater than 2^16-1
	if (m_Inputs > 65535)
		// set 2^16-1
		m_Inputs = 65535;

	// update controls
	UpdateData(FALSE);

	*pResult = 0;
}

void CNetDlg::OnOutputsSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// get values from controls
	UpdateData(TRUE);

	// update outputs variable
	m_Outputs -= pNMUpDown->iDelta;

	// check if outputs is less than one
	if (m_Outputs < 1)
		// set one
		m_Outputs = 1;

	// check if output is greather than 2^16-1
	if (m_Outputs > 65535)
		// set 2^16-1
		m_Outputs = 65535;
        
	// update controls
	UpdateData(FALSE);

	// update list box
	OnChangeOutputs();

	*pResult = 0;
}

void CNetDlg::OnNeuronsSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// get values from controls
	UpdateData(TRUE);

	// update neurons in layer variable
	m_Neurons -= pNMUpDown->iDelta;

	// if neurons is less than one
	if (m_Neurons < 1)
		// set one
		m_Neurons = 1;

	// if neurons is greather than 2^16-1
	if (m_Neurons > 65536)
		// set 2^16-1
		m_Neurons = 65536;

	// update controls
	UpdateData(FALSE);

	*pResult = 0;
}

void CNetDlg::OnLayersSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// get number of layers
	int I, Count = m_Layers.GetCount();

	// check if first or last selected item reached a bounds
	if (m_Layers.GetSel(pNMUpDown->iDelta > 0 ? Count - 1 : 0))
		// exit function
		return;

	// for each item
	for (I = 0; I < Count; I++)
	{
		// calculate item
		int Item = pNMUpDown->iDelta > 0 ? (Count - I - 1) : I;

		// check if selected
		if (m_Layers.GetSel(Item))
		{
			CString Text;
			int Position;
			unsigned int Data;
			
			// get item text
			m_Layers.GetText(Item, Text);

			// get item data
			Data = (unsigned int)m_Layers.GetItemData(Item);

			// delete string
			m_Layers.DeleteString(Item);

			// insert string one up or one down
			Position = m_Layers.InsertString(Item + pNMUpDown->iDelta, Text);

			// set item data
			m_Layers.SetItemData(Position, Data);

			// select item
			m_Layers.SetSel(Position);
		}
	}

	*pResult = 0;
}

void CNetDlg::OnWidthSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// get values from controls
	UpdateData(TRUE);

	// update width
	m_Width -= pNMUpDown->iDelta;

	// if width is less than one
	if (m_Width < 1)
		// set one
		m_Width = 1;

	// update controls
	UpdateData(FALSE);

	*pResult = 0;
}

void CNetDlg::OnHeightSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// get values from controls
	UpdateData(TRUE);

	// update height
	m_Height -= pNMUpDown->iDelta;

	// if height is less than one
	if (m_Height < 1)
		// set one
		m_Height = 1;

	// update controls
	UpdateData(FALSE);

	*pResult = 0;
}

void CNetDlg::OnChangeOutputs()
{
	int Count;

	// get values from controls
	UpdateData(TRUE);

	// if outputs number is greater than current
	if (m_Outputs > (Count = m_Categories.GetCount()))
	{
		// add more
		for (int i = Count; i < m_Outputs; i++)
		{
			CString Text;

			// format category item
			Text.Format(IDS_NET_OUTPUT, i);

			// add category to listbox
			m_Categories.AddString(Text);
		}
	}
	else
	{
		// delete if there is to much outputs
		for (int i = Count; i >= m_Outputs; i--)
			// delete category
			m_Categories.DeleteString(i);
	}
}

void CNetDlg::OnChangeCategories()
{
	// get selected categories item
	int Index = m_Categories.GetCaretIndex();

	if (Index >= 0)
	{
		CString Text;

		// get category name from list box
		m_Categories.GetText(Index, Text);

		// set category edit text
		m_Category.SetWindowText(Text);
	}
}

void CNetDlg::OnChangeCategory()
{
	// get selected categories item
	int Index = m_Categories.GetCaretIndex();

	if (Index >= 0)
	{
		CString Text;

		// get category name
		m_Category.GetWindowText(Text);

		// delete previous item
		m_Categories.DeleteString(Index);

		// insert item with new text
		m_Categories.InsertString(Index, Text);

		// set caret index and select item
		m_Categories.SetCaretIndex(Index);
		m_Categories.SetSel(Index);
	}
}
