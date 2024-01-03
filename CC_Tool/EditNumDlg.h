#pragma once
#include "afxwin.h"


// CEditNumDlg dialog

class CEditNumDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditNumDlg)

public:
	CEditNumDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditNumDlg();

	int	num;

	// dialog data
	enum { IDD = IDD_DIALOG_EDIT_NUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_ctlNum;
protected:
	virtual void OnOK();
};
