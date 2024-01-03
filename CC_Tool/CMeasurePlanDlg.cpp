// CMeasurePlanDlg.cpp: Implementation file
//

//#include "pch.h"
#include "stdafx.h"

#include "CC_Tool.h"
#include "CMeasurePlanDlg.h"
//#include "afxdialogex.h"


// CMeasurePlanDlg dialog

IMPLEMENT_DYNAMIC(CMeasurePlanDlg, CDialog)

CMeasurePlanDlg::CMeasurePlanDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_PLAN, pParent)
{
	pView = (CCC_ToolView*)pParent;
	m_bStart = FALSE;

	m_nMethod = AfxGetApp()->GetProfileInt("plan", "method", 0);
	m_nEveryTime = AfxGetApp()->GetProfileInt("plan", "everytime", 0);
	m_nIntervalTime = AfxGetApp()->GetProfileInt("plan", "intervaltime", 10);

	m_nLimitClock1 = AfxGetApp()->GetProfileInt("limit", "starth", 6);
	m_nLimitClock2 = AfxGetApp()->GetProfileInt("limit", "startm", 30);
	m_nLimitClock3 = AfxGetApp()->GetProfileInt("limit", "endh", 18);
	m_nLimitClock4 = AfxGetApp()->GetProfileInt("limit", "endm", 30);

	m_bLimitClock = AfxGetApp()->GetProfileInt("limit", "enable", 0);
	m_bAutoSave = AfxGetApp()->GetProfileInt("autosave", "enable", 0);

	m_nDataCount = 0;
}

CMeasurePlanDlg::~CMeasurePlanDlg()
{
	AfxGetApp()->WriteProfileInt("plan", "method", m_nMethod);
	AfxGetApp()->WriteProfileInt("plan", "everytime", m_nEveryTime);
	AfxGetApp()->WriteProfileInt("plan", "intervaltime", m_nIntervalTime);
	AfxGetApp()->WriteProfileInt("limit", "starth", m_nLimitClock1);
	AfxGetApp()->WriteProfileInt("limit", "startm", m_nLimitClock2);
	AfxGetApp()->WriteProfileInt("limit", "endh", m_nLimitClock3);
	AfxGetApp()->WriteProfileInt("limit", "endm", m_nLimitClock4);
	AfxGetApp()->WriteProfileInt("limit", "enable", m_bLimitClock);
	AfxGetApp()->WriteProfileInt("autosave", "enable", m_bAutoSave);

}

void CMeasurePlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TIME, m_ctlTime);
	DDX_Control(pDX, IDC_BUTTON_PLAN_START, m_ctlStart);
	DDX_Control(pDX, IDC_BUTTON_PLAN_STOP, m_ctlStop);
	DDX_Control(pDX, IDC_RADIO_METHOD1, m_ctlMethod1);
	DDX_Control(pDX, IDC_RADIO_METHOD2, m_ctlMethod2);
	DDX_Control(pDX, IDC_COMBO_EVERYTIME, m_ctlEveryTime);
	DDX_Control(pDX, IDC_EDIT_PLAN_INTERVAL, m_ctlIntervalTime);
	DDX_Control(pDX, IDC_CHECK_LIMIT_CLOCK, m_ctlLimitClock);
	DDX_Control(pDX, IDC_EDIT_LIMIT_CLOCK1, m_ctlLimitClock1);
	DDX_Control(pDX, IDC_EDIT_LIMIT_CLOCK2, m_ctlLimitClock2);
	DDX_Control(pDX, IDC_EDIT_LIMIT_CLOCK3, m_ctlLimitClock3);
	DDX_Control(pDX, IDC_EDIT_LIMIT_CLOCK4, m_ctlLimitClock4);
	DDX_Control(pDX, IDC_CHECK_AUTO_SAVE, m_ctlAutoSave);
}


BEGIN_MESSAGE_MAP(CMeasurePlanDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PLAN_START, &CMeasurePlanDlg::OnBnClickedButtonPlanStart)
	ON_BN_CLICKED(IDC_BUTTON_PLAN_STOP, &CMeasurePlanDlg::OnBnClickedButtonPlanStop)
	ON_BN_CLICKED(IDC_RADIO_METHOD1, &CMeasurePlanDlg::OnBnClickedRadioMethod1)
	ON_BN_CLICKED(IDC_RADIO_METHOD2, &CMeasurePlanDlg::OnBnClickedRadioMethod2)
	ON_BN_CLICKED(IDC_CHECK_LIMIT_CLOCK, &CMeasurePlanDlg::OnBnClickedCheckLimitClock)
	ON_BN_CLICKED(IDC_CHECK_AUTO_SAVE, &CMeasurePlanDlg::OnBnClickedCheckAutoSave)
END_MESSAGE_MAP()


// CMeasurePlanDlg message handler


void CMeasurePlanDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		KillTimer(nIDEvent);
		CTime	now;
		CString	str;

		now = now.GetCurrentTime();
		str = now.Format("%Y/%m/%d %H:%M:%S");

		m_ctlTime.SetWindowText(str);
		if ( now.GetMinute() != m_nOldTime) {
			if (oldDay != now.GetDay()) {
				if (m_ctlAutoSave.GetCheck()) {
					oldDay = now.GetDay();
					m_strAutoSavePath = GetFileName(now);
				}
				else {
					m_strAutoSavePath = "";
				}
			}
			m_nTimeCount++;
			m_nOldTime = now.GetMinute();
			if ( !m_ctlLimitClock.GetCheck() || IsInTime(now)) {
				if ( m_bStart) {
					if ( IsExeTime( m_nOldTime)) {
						pView->PutData(m_strAutoSavePath, m_nDataCount++);
					}
				}
			}
		}
		SetTimer( nIDEvent, 300, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}


BOOL CMeasurePlanDlg::OnInitDialog()
{
	CString	str;

	CDialog::OnInitDialog();

	SetTimer(1, 300, NULL);
	m_ctlEveryTime.ResetContent();
	m_ctlEveryTime.AddString("every 1 minutes");
	m_ctlEveryTime.AddString("every 5 minutes");
	m_ctlEveryTime.AddString("every 10 minutes");
	m_ctlEveryTime.AddString("every 20 minutes");
	m_ctlEveryTime.AddString("every 30 minutes");
	m_ctlEveryTime.AddString("every 1 hour");
	str.Format("%d", m_nLimitClock1);	m_ctlLimitClock1.SetWindowText(str);
	str.Format("%d", m_nLimitClock2);	m_ctlLimitClock2.SetWindowText(str);
	str.Format("%d", m_nLimitClock3);	m_ctlLimitClock3.SetWindowText(str);
	str.Format("%d", m_nLimitClock4);	m_ctlLimitClock4.SetWindowText(str);
	m_ctlLimitClock.SetCheck(m_bLimitClock);
	m_ctlAutoSave.SetCheck(m_bAutoSave);
	SetButton();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Exception: OCX property pages always return FALSE.
}


void CMeasurePlanDlg::OnBnClickedButtonPlanStart()
{
	CString	str;
	CTime	now;

	m_ctlIntervalTime.GetWindowText(str);
	m_nIntervalTime = atoi(str);
	m_nEveryTime = m_ctlEveryTime.GetCurSel();
	m_nTimeCount = 0;
	m_nDataCount = 0;
//	pView->OnBnClickedButtonGetData();
	m_bStart = TRUE;

	now = now.GetCurrentTime();
	if (m_ctlAutoSave.GetCheck()) {
		m_strAutoSavePath = GetFileName(now);
	}
	else {
		m_strAutoSavePath = "";
	}
	oldDay = now.GetDay();
	SetButton();
}


void CMeasurePlanDlg::OnBnClickedButtonPlanStop()
{
	if (m_bStart) {
		m_bStart = FALSE;
		SetButton();
	}
}


void CMeasurePlanDlg::OnBnClickedRadioMethod1()
{
	m_nMethod = 0;
	SetButton();
}


void CMeasurePlanDlg::OnBnClickedRadioMethod2()
{
	m_nMethod = 1;
	SetButton();
}


int CMeasurePlanDlg::SetButton()
{
	CString	str;

	m_ctlStart.EnableWindow(!m_bStart);
	m_ctlStop.EnableWindow(m_bStart);
	m_ctlMethod1.SetCheck(m_nMethod == 0);
	m_ctlMethod2.SetCheck(m_nMethod == 1);
	m_ctlEveryTime.SetCurSel(m_nEveryTime);
	str.Format("%d", m_nIntervalTime);
	m_ctlIntervalTime.SetWindowText(str);
	m_ctlEveryTime.EnableWindow(m_nMethod == 0);
	m_ctlIntervalTime.EnableWindow(m_nMethod == 1);
	m_ctlLimitClock1.EnableWindow(m_ctlLimitClock.GetCheck());
	m_ctlLimitClock2.EnableWindow(m_ctlLimitClock.GetCheck());
	m_ctlLimitClock3.EnableWindow(m_ctlLimitClock.GetCheck());
	m_ctlLimitClock4.EnableWindow(m_ctlLimitClock.GetCheck());
	m_ctlLimitClock1.GetWindowText(str);	 m_nLimitClock1 = atoi(str);
	m_ctlLimitClock2.GetWindowText(str);	 m_nLimitClock2 = atoi(str);
	m_ctlLimitClock3.GetWindowText(str);	 m_nLimitClock3 = atoi(str);
	m_ctlLimitClock4.GetWindowText(str);	 m_nLimitClock4 = atoi(str);
	return 0;
}


void CMeasurePlanDlg::OnBnClickedCheckLimitClock()
{
	m_bLimitClock = m_ctlLimitClock.GetCheck();
	SetButton();
}


int CMeasurePlanDlg::IsInTime(CTime now)
{
	bool	ret_from = false;
	bool	ret_to = false;

	if (now.GetHour() > m_nLimitClock1) {
		ret_from = true;
	} else if (now.GetHour() == m_nLimitClock1) {
		if (now.GetMinute() >= m_nLimitClock2) {
			ret_from = true;
		}
	}
	if (now.GetHour() < m_nLimitClock3) {
		ret_to = true;
	} else if (now.GetHour() == m_nLimitClock3) {
		if (now.GetMinute() <= m_nLimitClock4) {
				return ret_to = true;
		}
	}
	return ret_from && ret_to;
}


int CMeasurePlanDlg::IsExeTime(int time)
{
	if (m_nMethod == 0) {
		switch (m_nEveryTime) {
		case 0:						// every minute
			return 1;
			break;
		case 1:						// every 5minute
			if ((m_nOldTime % 5) == 0) {
				return 1;
			}
			break;
		case 2:						// every 10minute
			if ((m_nOldTime % 10) == 0) {
				return 1;
			}
			break;
		case 3:						// every 20minute
			if ((m_nOldTime % 20) == 0) {
				return 1;
			}
			break;
		case 4:						// every 30minute
			if ((m_nOldTime % 30) == 0) {
				return 1;
			}
			break;
		case 5:						// every 60minute
			if ((m_nOldTime % 60) == 0) {
				return 1;
			}
			break;
		}
	}
	else {
		if ((m_nTimeCount % m_nIntervalTime) == 0) {
			return 1;
		}
	}

	return 0;
}


CString CMeasurePlanDlg::GetFileName(CTime now)
{
	CString	str;

	str.Format("%04d%02d%02d_%02d%02d%02d.csv", now.GetYear(), now.GetMonth(), now.GetDay(), now.GetHour(), now.GetMinute(), now.GetSecond());
	return str;
}


void CMeasurePlanDlg::OnBnClickedCheckAutoSave()
{
	m_bAutoSave = m_ctlAutoSave.GetCheck();
}
