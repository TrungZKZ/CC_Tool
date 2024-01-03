// EditNumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CC_Tool.h"
#include "EditNumDlg.h"


// CEditNumDlg dialog

IMPLEMENT_DYNAMIC(CEditNumDlg, CDialog)

CEditNumDlg::CEditNumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditNumDlg::IDD, pParent)
{

}

CEditNumDlg::~CEditNumDlg()
{
}

void CEditNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NUM, m_ctlNum);
}


BEGIN_MESSAGE_MAP(CEditNumDlg, CDialog)
END_MESSAGE_MAP()


// CEditNumDlg message handler

BOOL CEditNumDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString	str;

	str.Format( "%d", num);
	m_ctlNum.SetWindowText( str);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Exception: OCX property pages always return FALSE.
}

void CEditNumDlg::OnOK()
{
	CString	str;

	m_ctlNum.GetWindowText( str);
	num = atoi( str);

	CDialog::OnOK();
}
