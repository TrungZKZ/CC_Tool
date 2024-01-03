// CC_ToolView.h : CCC_ToolView class interface
//


#pragma once
#include "afxwin.h"

#include <atlimage.h>

#define		PEN_BROWN	0
#define		PEN_RED		1
#define		PEN_ORANGE	2
#define		PEN_YELLOW	3
#define		PEN_GREEN	4
#define		PEN_BLUE	5
#define		PEN_PURPLE	6
#define		PEN_GRAY	7
#define		PEN_PINK	8
#define		PEN_BLACK	9
#define		PEN_WHITE	10

#define		MAX_COLOR	11
#define		MAX_STEP	10

#define	CMD_GET_NAME		0
#define	CMD_GET_KIND		1
#define	CMD_GET_VER			2
#define	CMD_GET_SERIAL		3
#define	CMD_GET_SENSOR		4
#define	CMD_GET_COMP		5
#define	CMD_GET_CLK			8
#define	CMD_GET_EXP			9
#define	CMD_GET_CYC			10
#define	CMD_GET_AVG			11
#define	CMD_GET_WAVE		12
#define	CMD_GET_GAIN		13
#define	CMD_GET_DARK		14
#define	CMD_GET_DATA		15
#define	CMD_GET_PIXEL		16
#define	CMD_GET_CPUID		17
#define	CMD_GET_TEMP		18
#define	CMD_GET_INTERVAL	19			// append 2022/03/13

#define	CMD_SET_EXP			132
#define	CMD_SET_CYC			133
#define	CMD_SET_AVG			134

#define	CMD_SET_INTERVAL	135			// append 2022/03/13
#define	CMD_DO_DARK			136			// append 2022/03/13

class CCC_ToolView : public CFormView
{
protected: // Create only from serialization.
	CCC_ToolView();
	DECLARE_DYNCREATE(CCC_ToolView)

public:
	enum{ IDD = IDD_CC_TOOL_FORM };

	// attribute
public:
	CCC_ToolDoc* GetDocument() const;

	// operation
public:
	COLORREF	m_crColors[ MAX_COLOR][ MAX_STEP];
	CPen*		m_pLinec[ MAX_COLOR*2][ MAX_STEP];
	CPen*		m_pLinew[MAX_WAVE];

	CBitmap		*cbmp;

	int			m_nGMax;
	int			m_nGMin;
	int			m_nRMax;
	int			m_nRMin;

	CString		m_strExePath;
	CImage		m_Img;
	HRESULT		m_Hr;
	int			m_nGraphKind;
	bool		m_bFirstOnly;

	HANDLE		m_hCom[ MAX_DEVICE];

	int			m_nPixel;
	float		m_dData[MAX_DEVICE][MAX_WAVE];
	float		m_dTemp[MAX_DEVICE];
	float		m_dVer[MAX_DEVICE];
	float		m_dXYZ[ MAX_WAVE][ 3];

	int			m_nSelect;
	bool		m_bExposure;
	bool		m_bGetData;
	bool		m_bRepeat;
	bool		m_bMonitor;
	int			m_nDocNo;
	int			m_nRefSample;

	CRect		m_rectGraph;
	int			m_nAverage;
	int			m_nOldAverage;
	int			m_nInterval;
	int			m_nOldInterval;

	BOOL		m_bC14384MA;
	BOOL		m_bDetectCH340E[ MAX_DEVICE];
	BOOL		m_bNotFirst;
	int				backup_cx, backup_cy;
	UINT			backup_type;
	int				m_nStartTime;
	int				m_nIntervalTime;

	bool			m_bDoDark;
	// override
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // Called for the first time after construction.

	// implementation
public:
	virtual ~CCC_ToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message allocation function
protected:
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctlGraph;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DispGraph( int no);
	CString GetExePath(void);
	CComboBox m_ctlGraphKind;
	afx_msg void OnCbnSelchangeComboGraph();
	bool CheckVPID(CString vpid, CString comport);
	int GetDevice(void);
	HANDLE OpenComm(CString com);
	bool CheckColorCompass(int no, CString com);
	bool GetUSBTool(HANDLE h, int cmd, int index, char* p, int len);
	bool GetUSBToolData(HANDLE h, int cmd, int index, int no, char* p, int len);
	CString GetName(int no);
	CString GetVer(int no);
	int GetKind(int no);
	CString GetSerial(int no);
	CString GetSensor(int no);
	void GetData(int no);
	CListBox m_ctlSerial;
	float GetTemperature(int no);
	int GetExposure(int no);
	int GetCPUID(int no);
	void MakeBarLine(void);
	void MakeXYZTable(void);
	void GetXY( int no, float *x, float *y);
	CEdit m_ctlExpTime;
	CButton m_ctlExpDown;
	CButton m_ctlExpUp;
	afx_msg void OnBnClickedButtonExpDown();
	afx_msg void OnBnClickedButtonExpUp();
	afx_msg void OnLbnSelchangeListSerial();
	bool SetUSBTool(HANDLE h, int cmd, int index, char* p, int len);
	void SetExposure(int no, int exp);
	CButton m_ctlExpUpdate;
	afx_msg void OnBnClickedButtonExpUpdate();
	CListBox m_ctlData;
	afx_msg void OnLbnSelchangeListData();
	CButton m_ctlGetData;
	afx_msg void OnBnClickedButtonGetData();
	void SetButton( bool bDevice);
	void PutData( void);
	void PutData(CString path, int no);
	CButton m_ctlDelData;
	afx_msg void OnBnClickedButtonDel();
	void RefreshDataList(void);
	CButton m_ctlSetRef;
	afx_msg void OnBnClickedButtonRefData();
	CButton m_ctlCSVSave;
	afx_msg void OnBnClickedButtonSaveCsv();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CButton m_ctlRefSample;
	afx_msg void OnBnClickedButtonRefsample();
	CComboBox m_ctlAverage;
	afx_msg void OnCbnSelchangeComboAverage();
	int GetAverage(int no);
	void SetAverage(int no, int avg);
	int GetInterval(int no);
	void SetInterval(int no, int interval);
	void DoDark(int no);
	void CheckPara(int no);
	afx_msg void OnBnClickedButtonRedetect();
	bool	SetBaudRate(HANDLE h, int baudrate);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetSize(int cx, int cy);
	CButton m_ctlReDetect;
	CStatic m_ctlExpName;
	CStatic m_ctlAvgName;
	CStatic m_ctlDataListName;
	// m_ctlPepeat
	CButton m_ctlRepeat;
	CEdit m_ctlRepeatTime;
	CButton m_ctlPlan;
	afx_msg void OnBnClickedButtonPlan();
	afx_msg void OnDoDark();
	afx_msg void OnUpdateDoDark(CCmdUI* pCmdUI);
	CEdit m_ctlAvgInterval;
	void SaveNowData(CString path, int no, int next);
};

#ifndef _DEBUG  // CC_ToolView.cpp Debug version of
inline CCC_ToolDoc* CCC_ToolView::GetDocument() const
   { return reinterpret_cast<CCC_ToolDoc*>(m_pDocument); }
#endif

