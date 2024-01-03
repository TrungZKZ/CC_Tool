#pragma once
#include "afxwin.h"

#include "CC_ToolDoc.h"
#include "CC_ToolView.h"

// CMeasurePlanDlg dialog

class CMeasurePlanDlg : public CDialog
{
	DECLARE_DYNAMIC(CMeasurePlanDlg)

public:
	CMeasurePlanDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMeasurePlanDlg();

	BOOL m_bStart;
	CCC_ToolView* pView;
	int		m_nMethod;
	int		m_nEveryTime;
	int		m_nIntervalTime;
	int		m_nOldTime;
	int		m_nTimeCount;
	int		m_nLimitClock1;
	int		m_nLimitClock2;
	int		m_nLimitClock3;
	int		m_nLimitClock4;

	CString	m_strAutoSavePath;
	int		oldDay;
	int		m_nDataCount;

	bool	m_bLimitClock;
	bool	m_bAutoSave;

//dialog data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PLAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctlTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	CButton m_ctlStart;
	CButton m_ctlStop;
	afx_msg void OnBnClickedButtonPlanStart();
	afx_msg void OnBnClickedButtonPlanStop();
	CButton m_ctlMethod1;
	afx_msg void OnBnClickedRadioMethod1();
	CButton m_ctlMethod2;
	afx_msg void OnBnClickedRadioMethod2();
	int SetButton();
	CComboBox m_ctlEveryTime;
	CEdit m_ctlIntervalTime;
	CButton m_ctlLimitClock;
	CEdit m_ctlLimitClock1;
	CEdit m_ctlLimitClock2;
	CEdit m_ctlLimitClock3;
	CEdit m_ctlLimitClock4;
	afx_msg void OnBnClickedCheckLimitClock();
	CButton m_ctlAutoSave;
	int IsInTime(CTime now);
	int IsExeTime(int time);
	CString GetFileName(CTime now);
	afx_msg void OnBnClickedCheckAutoSave();
};
