// CC_ToolView.cpp : Implementation of CCC_ToolView class
//

#include "stdafx.h"
#include "CC_Tool.h"

#include "CC_ToolDoc.h"
#include "CC_ToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCC_ToolView

IMPLEMENT_DYNCREATE(CCC_ToolView, CFormView)

BEGIN_MESSAGE_MAP(CCC_ToolView, CFormView)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_GRAPH		, &CCC_ToolView::OnCbnSelchangeComboGraph)
	ON_BN_CLICKED(IDC_BUTTON_EXP_DOWN			, &CCC_ToolView::OnBnClickedButtonExpDown)
	ON_BN_CLICKED(IDC_BUTTON_EXP_UP				, &CCC_ToolView::OnBnClickedButtonExpUp)
	ON_LBN_SELCHANGE(IDC_LIST_SERIAL				, &CCC_ToolView::OnLbnSelchangeListSerial)
	ON_BN_CLICKED(IDC_BUTTON_EXP_UPDATE		, &CCC_ToolView::OnBnClickedButtonExpUpdate)
	ON_LBN_SELCHANGE(IDC_LIST_DATA				, &CCC_ToolView::OnLbnSelchangeListData)
	ON_BN_CLICKED(IDC_BUTTON_GET_DATA			, &CCC_ToolView::OnBnClickedButtonGetData)
	ON_BN_CLICKED(IDC_BUTTON_DEL					, &CCC_ToolView::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_REF_DATA			, &CCC_ToolView::OnBnClickedButtonRefData)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_CSV			, &CCC_ToolView::OnBnClickedButtonSaveCsv)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_REFSAMPLE		, &CCC_ToolView::OnBnClickedButtonRefsample)
	ON_CBN_SELCHANGE(IDC_COMBO_AVERAGE	, &CCC_ToolView::OnCbnSelchangeComboAverage)
	ON_BN_CLICKED(IDC_BUTTON_REDETECT			, &CCC_ToolView::OnBnClickedButtonRedetect)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_PLAN				, &CCC_ToolView::OnBnClickedButtonPlan)
	ON_COMMAND(ID_DO_DARK								, &CCC_ToolView::OnDoDark)
	ON_UPDATE_COMMAND_UI(ID_DO_DARK			, &CCC_ToolView::OnUpdateDoDark)
END_MESSAGE_MAP()

// CCC_ToolView Construction/Destruction

#define	CSTEP	50

CCC_ToolView::CCC_ToolView()
	: CFormView(CCC_ToolView::IDD)
{
	int	i, j;

	for( i = 0 ; i < MAX_STEP ; i++){
		m_crColors[PEN_BROWN ][ i] = RGB( 141 - i * CSTEP	,071 - i * CSTEP	,071 - i * CSTEP);				// Brown
		m_crColors[PEN_RED	 ][ i] = RGB( 255 - i * CSTEP	,  0				,			0);					// Red
		m_crColors[PEN_ORANGE][ i] = RGB( 255 - i * CSTEP	,128 - i * CSTEP	,			0);					// Orange
		m_crColors[PEN_YELLOW][ i] = RGB( 255 - i * CSTEP	,255 - i * CSTEP	,			0);					// Yellow	
		m_crColors[PEN_GREEN ][ i] = RGB(   0				,255 - i * CSTEP	,			0);					// Green
		m_crColors[PEN_BLUE  ][ i] = RGB(  60				, 60				,255 - i * CSTEP);				// Blue
		m_crColors[PEN_PURPLE][ i] = RGB( 128 - i * CSTEP	,  0				,255 - i * CSTEP);				// Purple
		m_crColors[PEN_GRAY  ][ i] = RGB( 192 - i * CSTEP	,192 - i * CSTEP	,192 - i * CSTEP);				// Gray
		m_crColors[PEN_PINK  ][ i] = RGB( 255 - i * CSTEP	,128 - i * CSTEP	,255 - i * CSTEP);				// Pink
		m_crColors[PEN_BLACK ][ i] = RGB(   0				,  0				,			0);					// Black
		m_crColors[PEN_WHITE ][ i] = RGB( 255 - i * CSTEP	,255 - i * CSTEP	,255 - i * CSTEP);				// WHITE
	}

	for( i = 0 ; i < MAX_COLOR ; i++){
		for( j = 0 ; j < MAX_STEP; j++){
			m_pLinec[i][ j]				= new CPen(PS_SOLID,1,m_crColors[i][ j]);
			m_pLinec[i+MAX_COLOR][ j]	= new CPen(PS_SOLID,3,m_crColors[i][ j]);
		}
	}
	MakeBarLine();
	MakeXYZTable();
	m_strExePath	= GetExePath();
	m_nGraphKind	= AfxGetApp()->GetProfileInt( "graph", "kind", 0);

	m_nGMax			= AfxGetApp()->GetProfileInt("graph", "max", 65535);
	m_nGMin			= AfxGetApp()->GetProfileInt("graph", "min", 0);
	m_nRMax			= AfxGetApp()->GetProfileInt("ratio", "max", 120);
	m_nRMin			= AfxGetApp()->GetProfileInt("ratio", "min", 0);

	m_nSelect	= 0;
	m_nPixel = MAX_PIXEL;
	for (j = 0; j < MAX_DEVICE; j++) {
		m_hCom[ j] = NULL;
		m_bDetectCH340E[ j] = FALSE;
		for (i = 0; i < MAX_WAVE; i++) {
			m_dData[j][i] = i * 100;
		}
	}
	m_nSelect		= 0;
	m_bFirstOnly	= TRUE;
	m_bExposure		= FALSE;
	m_bGetData		= FALSE;
	m_bRepeat		= FALSE;

	m_bMonitor		= TRUE;
	m_nRefSample	= -1;

	m_bC14384MA		= FALSE;

	m_bNotFirst = FALSE;

	m_nIntervalTime = AfxGetApp()->GetProfileInt("measure", "interval", 1000);
	m_nInterval = 0;
	for (i = 0; i < MAX_DEVICE; i++) {
		m_dVer[i] = 0.0;
	}
	m_bDoDark = FALSE;
}

CCC_ToolView::~CCC_ToolView()
{
	int	i, j;

	AfxGetApp()->WriteProfileInt( "graph", "kind", m_nGraphKind);
	for( i = 0 ; i < MAX_COLOR ; i++){
		for( j = 0 ; j < MAX_STEP; j++){
			if( m_pLinec[i][ j] != NULL)	delete m_pLinec[i][ j];
			if( m_pLinec[i+MAX_COLOR][ j])	delete m_pLinec[i+MAX_COLOR][ j];
		}
	}
	for ( i = 0; i < MAX_WAVE; i++) {
		if( m_pLinew[i] != NULL) delete m_pLinew[i];
	}
	AfxGetApp()->WriteProfileInt("graph", "max", m_nGMax);
	AfxGetApp()->WriteProfileInt("graph", "min", m_nGMin);
	AfxGetApp()->WriteProfileInt("ratio", "max", m_nRMax);
	AfxGetApp()->WriteProfileInt("ratio", "min", m_nRMin);
	AfxGetApp()->WriteProfileInt("measure", "interval", m_nIntervalTime);
}


void CCC_ToolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAPH, m_ctlGraph);
	DDX_Control(pDX, IDC_COMBO_GRAPH, m_ctlGraphKind);
	DDX_Control(pDX, IDC_LIST_SERIAL, m_ctlSerial);
	DDX_Control(pDX, IDC_EDIT_EXPTIME, m_ctlExpTime);
	DDX_Control(pDX, IDC_BUTTON_EXP_DOWN, m_ctlExpDown);
	DDX_Control(pDX, IDC_BUTTON_EXP_UP, m_ctlExpUp);
	DDX_Control(pDX, IDC_BUTTON_EXP_UPDATE, m_ctlExpUpdate);
	DDX_Control(pDX, IDC_LIST_DATA, m_ctlData);
	DDX_Control(pDX, IDC_BUTTON_GET_DATA, m_ctlGetData);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_ctlDelData);
	DDX_Control(pDX, IDC_BUTTON_REF_DATA, m_ctlSetRef);
	DDX_Control(pDX, IDC_BUTTON_SAVE_CSV, m_ctlCSVSave);
	DDX_Control(pDX, IDC_BUTTON_REFSAMPLE, m_ctlRefSample);
	DDX_Control(pDX, IDC_COMBO_AVERAGE, m_ctlAverage);
	DDX_Control(pDX, IDC_BUTTON_REDETECT, m_ctlReDetect);
	DDX_Control(pDX, IDC_STATIC_EXPNAME, m_ctlExpName);
	DDX_Control(pDX, IDC_STATIC_AVGNAME, m_ctlAvgName);
	DDX_Control(pDX, IDC_STATIC_DLIST_NAME, m_ctlDataListName);
	DDX_Control(pDX, IDC_CHECK_REPEAT, m_ctlRepeat);
	DDX_Control(pDX, IDC_EDIT_REPEATTIME, m_ctlRepeatTime);
	DDX_Control(pDX, IDC_BUTTON_PLAN, m_ctlPlan);
	DDX_Control(pDX, IDC_EDIT_AVG_INTERVAL, m_ctlAvgInterval);
}

BOOL CCC_ToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify CREATESTRUCT cs at this location to set Window class or style.
	//  Please correct.

	return CFormView::PreCreateWindow(cs);
}

enum	{
	graph_spectro = 0,
	graph_spectro_bar,
	graph_spectro_ref,
	graph_xyz,
};



void CCC_ToolView::OnInitialUpdate()
{
	int			exp, avg, interval;
	CString	str;
	int			i;
	CRect	rect;

	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();

//	m_nWaveNum		= GetDocument()->m_nWaveNum;
//	m_nStartWave	= GetDocument()->m_nStartWave;

	m_ctlGraphKind.ResetContent();
	m_ctlGraphKind.AddString( "spectrum (line)");
	m_ctlGraphKind.AddString( "spectrum (color)");
	m_ctlGraphKind.AddString( "Spectrum (comparison)");
	m_ctlGraphKind.AddString( "chromaticity diagram");
	m_ctlAverage.ResetContent();
	for( i = 0 ; i < 100; i++){
		str.Format( "%3d", i+1);
		m_ctlAverage.AddString( str);
	}
	if( m_bFirstOnly){
		GetDocument()->m_nSpectro		= GetDevice();
		m_bFirstOnly					= FALSE;
		for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
			GetDocument()->m_strSerial[ i] = GetSerial( i);
			GetDocument()->m_strSensor[ i] = GetSensor( i);
			CheckPara( i);
		}
	}
	if( GetDocument()->m_nSpectro){
		m_ctlSerial.SetCurSel( m_nSelect);
		exp = GetExposure( m_nSelect);
		str.Format( "%d", exp);
		m_ctlExpTime.SetWindowText( str);
		for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
			if( GetDocument()->m_strSensor[ i] == "C14384MA-01"){
				GetDocument()->m_nWaveNum	= 1050 - 640 +1;
				GetDocument()->m_nStartWave	= 640;
				m_bC14384MA	= TRUE;
			} else {
				GetDocument()->m_nWaveNum	= MAX_WAVE;
				GetDocument()->m_nStartWave	= START_WAVE;
				m_bC14384MA	= FALSE;
			}
		}
		avg = GetAverage(m_nSelect);
		m_ctlAverage.SetCurSel(avg - 1);
		interval = GetInterval(m_nSelect);
		str.Format("%d", interval);
		m_ctlAvgInterval.SetWindowText(str);
	}
 else {
 m_ctlSerial.ResetContent();
 for (i = 0; i < GetDocument()->m_nSpectro; i++) {
	 m_ctlSerial.AddString(GetDocument()->m_strSerial[i]);
	 if (GetDocument()->m_strSensor[i] == "C14384MA-01") {
		 GetDocument()->m_nWaveNum = 1050 - 640 + 1;
		 GetDocument()->m_nStartWave = 640;
		 m_bC14384MA = TRUE;
	 }
	 else {
		 GetDocument()->m_nWaveNum = MAX_WAVE;
		 GetDocument()->m_nStartWave = START_WAVE;
		 m_bC14384MA = FALSE;
	 }
 }
	}
	if (GetDocument()->m_nDataNum) {
		RefreshDataList();
	}
	SetButton(GetDocument()->m_nSpectro);
	if (m_nGraphKind == graph_spectro_ref) {
		if (GetDocument()->m_nRefSelect < 0) {
			m_nGraphKind = graph_spectro;
		}
		else if (GetDocument()->m_nRefSelect >= GetDocument()->m_nDataNum) {
			m_nGraphKind = graph_spectro;
		}
	}
	m_ctlGraphKind.SetCurSel(m_nGraphKind);
	if (m_bMonitor) {					// monitor start
		SetTimer(1, 100, NULL);
	}
	//	m_ctlGraph.GetClientRect( m_rectGraph);
	m_bNotFirst = TRUE;
	GetClientRect(rect);							// At first, OnSize does not start (it does, but the variables are not initialized), so adjust the drawing here

	SetSize(rect.Width(), rect.Height());

	str.Format("%d", m_nIntervalTime);
	m_ctlRepeatTime.SetWindowText(str);
}


// CCC_ToolView Diagnostics

#ifdef _DEBUG
void CCC_ToolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCC_ToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CCC_ToolDoc* CCC_ToolView::GetDocument() const // Non-debug versions are inline.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCC_ToolDoc)));
	return (CCC_ToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CCC_ToolView message handler

void CCC_ToolView::OnTimer(UINT_PTR nIDEvent)
{
	CString	str;
	int		exp;
	int		i;

	if (nIDEvent == 1) {														// initial timer
		KillTimer(nIDEvent);
		DispGraph(m_nSelect);
		if (GetDocument()->m_nSpectro) {
			SetTimer(2, 10, NULL);
		}
	}
	else if (nIDEvent == 2) {												// monitor timer
		KillTimer(nIDEvent);
		if (m_bMonitor) {
			for (i = 0; i < GetDocument()->m_nSpectro; i++) {		// get data all device
				GetData(i);
				m_dTemp[i] = GetTemperature(i);
			}
			DispGraph(m_nSelect);
			if (m_bExposure) {													// change exposure ?
				m_ctlExpTime.GetWindowText(str);
				exp = atoi(str);
				SetExposure(m_nSelect, exp);
				m_bExposure = FALSE;
			}
			if (m_nAverage != m_nOldAverage) {							// change average ?
				SetAverage(m_nSelect, m_nAverage);
				m_nOldAverage = m_nAverage;
			}
			if (m_dVer[m_nSelect] >= 1.5) {
				m_ctlAvgInterval.GetWindowText(str);
				m_nInterval = atoi(str);
				if (m_nInterval != m_nOldInterval) {
					m_nOldInterval = m_nInterval;
					SetInterval(m_nSelect, m_nInterval);
				}
			}
			if (m_bGetData) {													// get data ?
				if (m_ctlRepeat.GetCheck()) {
					if ((GetTickCount() - m_nStartTime) > m_nIntervalTime) {
						m_nStartTime += m_nIntervalTime;
						PutData();													// data save
					}
				} else {
					PutData();														// data save
					m_bGetData = FALSE;
					m_ctlGetData.SetWindowText("Data acquisition");
				}
			}
			if (m_bDoDark) {
				DoDark(m_nSelect);
				m_bDoDark = FALSE;
				AfxMessageBox("Ended");
			}
			SetTimer(nIDEvent, 50, NULL);								// 200 -> 50...for smooth monitor
		}
	}

	CFormView::OnTimer(nIDEvent);
}

#define	XYZ_TABLE	90

const	double	xyz[ XYZ_TABLE][ 4] = {
	380.0	,2.69E-03,	2.00E-04,	1.23E-02,
	385.0	,5.31E-03,	3.96E-04,	2.42E-02,
	390.0	,1.08E-02,	8.00E-04,	4.93E-02,
	395.0	,2.08E-02,	1.55E-03,	9.51E-02,
	400.0	,3.80E-02,	2.80E-03,	1.74E-01,
	405.0	,6.32E-02,	4.66E-03,	2.90E-01,
	410.0	,9.99E-02,	7.40E-03,	4.61E-01,
	415.0	,1.58E-01,	1.18E-02,	7.32E-01,
	420.0	,2.29E-01,	1.75E-02,	1.07E+00,
	425.0	,2.81E-01,	2.27E-02,	1.31E+00,
	430.0	,3.11E-01,	2.73E-02,	1.47E+00,
	435.0	,3.31E-01,	3.26E-02,	1.58E+00,
	440.0	,3.33E-01,	3.79E-02,	1.62E+00,
	445.0	,3.17E-01,	4.24E-02,	1.57E+00,
	450.0	,2.89E-01,	4.68E-02,	1.47E+00,
	455.0	,2.60E-01,	5.21E-02,	1.37E+00,
	460.0	,2.33E-01,	6.00E-02,	1.29E+00,
	465.0	,2.10E-01,	7.29E-02,	1.24E+00,
	470.0	,1.75E-01,	9.10E-02,	1.11E+00,
	475.0	,1.33E-01,	1.13E-01,	9.42E-01,
	480.0	,9.19E-02,	1.39E-01,	7.56E-01,
	485.0	,5.70E-02,	1.70E-01,	5.86E-01,
	490.0	,3.17E-02,	2.08E-01,	4.47E-01,
	495.0	,1.46E-02,	2.58E-01,	3.41E-01,
	500.0	,4.85E-03,	3.23E-01,	2.64E-01,
	505.0	,2.32E-03,	4.05E-01,	2.06E-01,
	510.0	,9.29E-03,	5.03E-01,	1.54E-01,
	515.0	,2.93E-02,	6.08E-01,	1.09E-01,
	520.0	,6.38E-02,	7.10E-01,	7.66E-02,
	525.0	,1.11E-01,	7.95E-01,	5.62E-02,
	530.0	,1.67E-01,	8.62E-01,	4.14E-02,
	535.0	,2.28E-01,	9.15E-01,	2.94E-02,
	540.0	,2.93E-01,	9.54E-01,	2.00E-02,
	545.0	,3.62E-01,	9.80E-01,	1.33E-02,
	550.0	,4.36E-01,	9.95E-01,	8.78E-03,
	555.0	,5.15E-01,	1.00E+00,	5.86E-03,
	560.0	,5.97E-01,	9.95E-01,	4.05E-03,
	565.0	,6.81E-01,	9.79E-01,	2.92E-03,
	570.0	,7.64E-01,	9.52E-01,	2.28E-03,
	575.0	,8.44E-01,	9.16E-01,	1.97E-03,
	580.0	,9.16E-01,	8.70E-01,	1.81E-03,
	585.0	,9.77E-01,	8.16E-01,	1.54E-03,
	590.0	,1.02E+00,	7.57E-01,	1.23E-03,
	595.0	,1.05E+00,	6.95E-01,	1.12E-03,
	600.0	,1.06E+00,	6.31E-01,	9.06E-04,
	605.0	,1.04E+00,	5.67E-01,	6.95E-04,
	610.0	,9.92E-01,	5.03E-01,	4.29E-04,
	615.0	,9.29E-01,	4.42E-01,	3.18E-04,
	620.0	,8.43E-01,	3.81E-01,	2.56E-04,
	625.0	,7.40E-01,	3.21E-01,	1.57E-04,
	630.0	,6.33E-01,	2.65E-01,	9.77E-05,
	635.0	,5.34E-01,	2.17E-01,	6.89E-05,
	640.0	,4.41E-01,	1.75E-01,	5.12E-05,
	645.0	,3.55E-01,	1.38E-01,	3.60E-05,
	650.0	,2.79E-01,	1.07E-01,	2.42E-05,
	655.0	,2.15E-01,	8.17E-02,	1.69E-05,
	660.0	,1.62E-01,	6.10E-02,	1.19E-05,
	665.0	,1.18E-01,	4.43E-02,	8.15E-06,
	670.0	,8.58E-02,	3.20E-02,	5.60E-06,
	675.0	,6.31E-02,	2.35E-02,	3.95E-06,
	680.0	,4.58E-02,	1.70E-02,	2.79E-06,
	685.0	,3.21E-02,	1.19E-02,	1.92E-06,
	690.0	,2.22E-02,	8.21E-03,	1.31E-06,
	695.0	,1.56E-02,	5.77E-03,	9.15E-07,
	700.0	,1.11E-02,	4.10E-03,	6.48E-07,
	705.0	,7.92E-03,	2.93E-03,	4.64E-07,
	710.0	,5.65E-03,	2.09E-03,	3.33E-07,
	715.0	,4.00E-03,	1.48E-03,	2.38E-07,
	720.0	,2.83E-03,	1.05E-03,	1.70E-07,
	725.0	,1.99E-03,	7.40E-04,	1.22E-07,
	730.0	,1.40E-03,	5.20E-04,	8.71E-08,
	735.0	,9.70E-04,	3.61E-04,	6.15E-08,
	740.0	,6.68E-04,	2.49E-04,	4.32E-08,
	745.0	,4.61E-04,	1.72E-04,	3.04E-08,
	750.0	,3.21E-04,	1.20E-04,	2.16E-08,
	755.0	,2.26E-04,	8.46E-05,	1.55E-08,
	760.0	,1.60E-04,	6.00E-05,	1.12E-08,
	765.0	,1.13E-04,	4.24E-05,	8.09E-09,
	770.0	,7.95E-05,	3.00E-05,	5.83E-09,
	775.0	,5.61E-05,	2.12E-05,	4.21E-09,
	780.0	,3.95E-05,	1.50E-05,	3.04E-09,
	785.0	,2.79E-05,	1.06E-05,	2.19E-09,
	790.0	,1.96E-05,	7.47E-06,	1.58E-09,
	795.0	,1.38E-05,	5.26E-06,	1.13E-09,
	800.0	,9.67E-06,	3.70E-06,	8.16E-10,
	805.0	,6.79E-06,	2.61E-06,	5.86E-10,
	810.0	,4.77E-06,	1.84E-06,	4.21E-10,
	815.0	,3.36E-06,	1.30E-06,	3.03E-10,
	820.0	,2.35E-06,	9.11E-07,	2.18E-10,
	825.0	,1.64E-06,	6.36E-07,	1.55E-10,
};


#define	ORGX	87+1
#define	ORGY	507+1
#define	LENX0_7	397
#define	LENY0_8	454

void CCC_ToolView::DispGraph( int no)
{
	int		i, j;
	float	x, y;
	float	xx, yy;
	CString	str;
	CRect	rect, rect_org;
	CDC		*pDC;
	CRect	round;
	CPen*	oldPen;
	int		div, rdiv;
	int		n;
	CBitmap *oldbmp;
	CDC		bmpDC;
	CBitmap	*cbmp;
	CImage	img;
	float	*pData;
	float	*pRef;

	pDC		= m_ctlGraph.GetDC();

	m_ctlGraph.GetClientRect( &rect_org);
	pDC->Rectangle( rect_org);

	if(( m_nGraphKind == graph_xyz) && !m_bC14384MA){
		m_Hr = img.Load( m_strExePath + "xyz.bmp");
		if( m_Hr == S_OK){
			cbmp	= CBitmap::FromHandle( img);
			bmpDC.CreateCompatibleDC( pDC);
			oldbmp = bmpDC.SelectObject( cbmp);
			if( oldbmp != NULL){
				pDC->SetStretchBltMode( STRETCH_HALFTONE);
				pDC->SetBrushOrg( 0, 0);
				pDC->StretchBlt( 1, 1, img.GetWidth(), img.GetHeight(), &bmpDC, 0, 0, img.GetWidth(), img.GetHeight(), SRCCOPY);
				bmpDC.SelectObject( oldbmp);
				if( GetDocument()->m_nSpectro){
					GetXY( no, &xx, &yy);
					round	= CRect( -5, -5, 5, 5);
					round.left	+= ORGX	+ xx * LENX0_7/0.7;
					round.right	+= ORGX	+ xx * LENX0_7/0.7;
					round.top	+= ORGY	- yy * LENY0_8/0.8;
					round.bottom+= ORGY	- yy * LENY0_8/0.8;;
					pDC->RoundRect( &round, CPoint( round.Width(), round.Height()));
					str.Format( "X,Y = %5.3f,%5.3f", xx, yy);
					pDC->TextOut( rect_org.left+400, rect_org.top+3, str);
				}
			}
			cbmp->DeleteObject();
			bmpDC.DeleteDC();
		}
	} else {
		rect.left	= rect_org.left + 55;
		rect.top	= rect_org.top + 10;
		rect.right	= rect_org.right - 35;
		rect.bottom	= rect_org.bottom - 25;
		pDC->Rectangle( rect);
		oldPen = pDC->SelectObject(m_pLinec[PEN_GRAY][0]);
		if( m_bC14384MA){
			for (i = 0; i < (GetDocument()->m_nWaveNum - 1); i++) {										// vertical memory line
				if (((i + GetDocument()->m_nStartWave) % 50) == 0) {
					pDC->MoveTo(i + rect.left, rect.top);
					pDC->LineTo(i + rect.left, rect.bottom);
					str.Format("%d", i + GetDocument()->m_nStartWave);
					pDC->TextOut(rect.left + i - 10, rect.bottom + 2, str);
				}
			}
		} else {
			for (i = 0; i < (GetDocument()->m_nWaveNum - 1); i++) {										// vertical memory line
				if (((i + GetDocument()->m_nStartWave) % 50) == 0) {
					pDC->MoveTo(i + rect.left, rect.top);
					pDC->LineTo(i + rect.left, rect.bottom);
					str.Format("%d", i + GetDocument()->m_nStartWave);
					pDC->TextOut(rect.left + i - 10, rect.bottom + 2, str);
				}
			}
		}
		if ((m_nGMax - m_nGMin) <= 1000) {
			div = 100;
		}
		else if ((m_nGMax - m_nGMin) <= 2000) {
			div = 500;
		}
		else if ((m_nGMax - m_nGMin) < 10000) {
			div = 1000;
		}
		else {
			div = 5000;
		}
		n = 0;
		for (i = m_nGMin; i < m_nGMax; i++) {											// horizontal memory line
			if ((i % div) == 0) {
				if (i) {
					n++;
				}
			}
		}
		m_nRMin = m_nGMin * m_nRMax / m_nGMax;

		if (n) {
			rdiv = (m_nRMax - m_nRMin) / n;
		}
		else {
			rdiv = (m_nRMax - m_nRMin);
		}
		if( m_nGraphKind != graph_spectro_ref){
			for (i = m_nGMin; i < m_nGMax; i++) {													// horizontal memory line
				if ((i % div) == 0) {
					pDC->MoveTo(rect.left, rect.bottom - (i - m_nGMin) * rect.Height() / (m_nGMax - m_nGMin));
					pDC->LineTo(rect.right, rect.bottom - (i - m_nGMin) * rect.Height() / (m_nGMax - m_nGMin));
					str.Format("%4d", i);
					pDC->TextOut(rect.left - 50, rect.bottom - (i - m_nGMin) * rect.Height() / (m_nGMax - m_nGMin) - 10, str);
				}
			}
		} else {
			for ( i = m_nRMin; i < m_nRMax; i++) {													// horizontal memory line
				if ((i % 10) == 0) {
					pDC->MoveTo(rect.left, rect.bottom - (i - m_nRMin) * rect.Height() / (m_nRMax - m_nRMin));
					pDC->LineTo(rect.right, rect.bottom - (i - m_nRMin) * rect.Height() / (m_nRMax - m_nRMin));
					str.Format("%3.0f", (float)i);
					pDC->TextOut(rect.left -50, rect.bottom - (i - m_nRMin) * rect.Height() / (m_nRMax - m_nRMin) - 10, str);
				}
			}
		}
		pDC->SelectObject(oldPen);
		if( GetDocument()->m_nSpectro){
			if( m_nGraphKind == graph_spectro){
				for( j = 0 ; j < GetDocument()->m_nSpectro; j++){
					if( m_bMonitor){
						pData = m_dData[ j];
					} else {
						pData = GetDocument()->m_dData[ j][ m_nDocNo];
					}
					if( no == j){
						oldPen = pDC->SelectObject(m_pLinec[j+MAX_COLOR][0]);
					} else {
						oldPen = pDC->SelectObject(m_pLinec[j][0]);
					}
					for( i = 0; i < GetDocument()->m_nWaveNum; i++) {
						x = i;
						y = ( pData[i] - m_nGMin) * rect.Height() / (m_nGMax - m_nGMin);
						if (y < 0) y = 0;
						if (y > rect.Height()) y = rect.Height();
						if (i == 0) {
							pDC->MoveTo(x + rect.left, rect.bottom - y);
						}
						else {
							pDC->LineTo(x + rect.left, rect.bottom - y);
						}
					}
					pDC->SelectObject(oldPen);
				}
			}  else if(( m_nGraphKind == graph_spectro_bar) && !m_bC14384MA){
				if( m_bMonitor){
					pData = m_dData[ m_nSelect];
				} else {
					pData = GetDocument()->m_dData[ m_nSelect][ m_nDocNo];
				}
				for( i = 0; i < GetDocument()->m_nWaveNum; i++) {
					oldPen = pDC->SelectObject( m_pLinew[i]);
					x = i;
					y = (pData[i] - m_nGMin) * rect.Height() / (m_nGMax - m_nGMin);
					if (y < 0) y = 0;
					if (y > rect.Height()) y = rect.Height();
					pDC->MoveTo(x + rect.left, rect.bottom);
					pDC->LineTo(x + rect.left, rect.bottom - y);
					pDC->SelectObject(oldPen);
				}
			}  else if( m_nGraphKind == graph_spectro_ref){
				for( j = 0 ; j < GetDocument()->m_nSpectro; j++){
					if( m_bMonitor){
						pData = m_dData[ m_nSelect];
					} else {
						pData = GetDocument()->m_dData[ j][ m_nDocNo];
					}
					pRef = GetDocument()->m_dData[ j][ GetDocument()->m_nRefSelect];
					if( no == j){
						oldPen = pDC->SelectObject(m_pLinec[j+ MAX_COLOR][0]);
					} else {
						oldPen = pDC->SelectObject(m_pLinec[j][0]);
					}
					for( i = 0; i < GetDocument()->m_nWaveNum; i++) {
						x = i;
						y = (( pData[i]/pRef[ i])*100.0- m_nRMin) * rect.Height() / (m_nRMax - m_nRMin);
						if (y < 0) y = 0;
						if (y > rect.Height()) y = rect.Height();
						if (i == 0) {
							pDC->MoveTo(x + rect.left, rect.bottom - y);
						}
						else {
							pDC->LineTo(x + rect.left, rect.bottom - y);
						}
					}
					pDC->SelectObject(oldPen);
					if(( m_nRefSample >= 0) && ( m_nRefSample < GetDocument()->m_nDataNum)){
						oldPen = pDC->SelectObject(m_pLinec[j][0]);
						pData = GetDocument()->m_dData[ j][ m_nRefSample];
						for( i = 0; i < GetDocument()->m_nWaveNum; i++) {
							x = i;
							y = (( pData[i]/pRef[ i])*100.0- m_nRMin) * rect.Height() / (m_nRMax - m_nRMin);
							if (y < 0) y = 0;
							if (y > rect.Height()) y = rect.Height();
							if (i == 0) {
								pDC->MoveTo(x + rect.left, rect.bottom - y);
							}
							else {
								pDC->LineTo(x + rect.left, rect.bottom - y);
							}
						}
						pDC->SelectObject(oldPen);
					}
				}
			}
			if( m_bMonitor){
				str.Format("%4.1f", m_dTemp[no]);
			} else {
				str.Format("%4.1f", GetDocument()->m_dTemp[no][ m_nDocNo]);
			}
			pDC->TextOut(rect.right - 100, rect.top + 2, str);
		}
	}
	ReleaseDC( pDC);
}


CString CCC_ToolView::GetExePath(void)
{
	char	path[ MAX_PATH];
	int		i;
	CString	str;

	str = "";
	GetModuleFileName( NULL, path, MAX_PATH );
	str	= path;
	for( i = str.GetLength()-1; i > 0; i--){
		if( str.GetAt( i) == '\\'){
			str = str.Left( i+1);
			break;
		}
	}
	return str;
}

void CCC_ToolView::OnCbnSelchangeComboGraph()
{
	int	sel;

	sel = m_ctlGraphKind.GetCurSel();
	if( sel == graph_spectro_ref){
		if( GetDocument()->m_nRefSelect < 0){
			AfxMessageBox( "Please select reference data first");
			m_ctlGraphKind.SetCurSel( m_nGraphKind);
		} else {
			m_nGraphKind	= sel;
		}
	} else {
		if( m_bC14384MA){
			m_nGraphKind	= 0;
		} else {
			m_nGraphKind	= sel;
		}
		m_ctlGraphKind.SetCurSel( m_nGraphKind);
	}
	DispGraph( m_nSelect);
}


HANDLE CCC_ToolView::OpenComm(CString com)
{
	HANDLE	hPort;											// Communication device handle

	hPort = CreateFile( com, GENERIC_READ | GENERIC_WRITE,	//　Access mode: Specify both reading and writing as it is normally used for both sending and receiving.
							0,								//　Share mode: Normally set to 0, reopening prohibited
						    NULL,							//	Security attribute: Usually set to NULL for communication
							OPEN_EXISTING,					//　Create distribution: Usually the COM port already exists, so set it to OPEN_EXISTING.
							FILE_ATTRIBUTE_NORMAL,			//　Attribute: For communication, there is no attribute, so specify FILE_ATTRIBUTE_NORMAL (no attribute)
							NULL							//　Template handle: Not relevant for communication, usually specify NULL
			);
	return hPort;
}


bool CCC_ToolView::CheckColorCompass(int no, CString com)
{
	bool	ret;

	ret = false;

	ret = CheckVPID( "VID_0C2B&PID_0820",com);
	if ( ret) {
		return ret;
	} else {
		ret = CheckVPID("VID_1A86&PID_7523", com);	// CH340E
		if (ret) {
			m_bDetectCH340E[ no] = TRUE;
		}
		else {
			m_bDetectCH340E[no] = FALSE;
		}
	}
	return ret;
}

int CCC_ToolView::GetDevice(void)
{
	int		i;
	CString	com;
	HANDLE	h;
	int		num;
	CString	str, ver;

	num = 0;
	for( i = 1; i < 100; i++){
		com.Format( "\\\\.\\COM%d", i);
		h = OpenComm( com);
		if( h != INVALID_HANDLE_VALUE){
			com.Format("(COM%d)", i);
			if( CheckColorCompass( num, com)){
				m_hCom[ num]	= h;
				if ( m_bDetectCH340E[ num]) {
					SetBaudRate(m_hCom[num], 921600);
				}
				ver = GetVer(num);	m_dVer[num] = atof(ver);
				str = GetSerial( num);
				m_ctlSerial.AddString( str + "(" + com + ") "+ver);
				num++;
			} else {
				CloseHandle( h);
			}
		}
	}
	return num;
}

bool CCC_ToolView::SetBaudRate(HANDLE h, int baudrate)
{
	DCB		dcb;					// Structure containing serial port configuration information
	CString	str;

	GetCommState(h, &dcb);

	dcb.ByteSize = 8;				// Data length
	dcb.Parity = NOPARITY;			// parity
	dcb.StopBits = ONESTOPBIT;		// stop bit length
	dcb.fOutxCtsFlow = FALSE;			// CTS flow when sending
	dcb.BaudRate = baudrate;
	SetCommState(h, &dcb);					// Write the changed setting values
//	str.Format( "%d,%d,%d,%d", dcb.BaudRate, dcb.ByteSize, dcb.Parity, ONESTOPBIT);
//	m_ctlList.AddString( str);
	return false;
}

#include <winioctl.h>
#include <SetupAPI.h>

bool CCC_ToolView::CheckVPID(CString vpid, CString comport)
{
	BOOL	ret	= FALSE;

	DWORD			dwIndex;
	TCHAR			pszName[256];
	DWORD			dwSize;
	DWORD			dwRegType;
	BOOL			bRet;
	HDEVINFO		hDevInfo;
	SP_DEVINFO_DATA	sDevInfo;
	CString			fn, hd;

	hDevInfo = ::SetupDiGetClassDevs( &GUID_DEVINTERFACE_COMPORT, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if( hDevInfo == INVALID_HANDLE_VALUE) return ret;

	dwIndex = 0;
	::ZeroMemory( &sDevInfo,sizeof( SP_DEVINFO_DATA));
	sDevInfo.cbSize = sizeof( SP_DEVINFO_DATA);
	while( 1){
		bRet = ::SetupDiEnumDeviceInfo( hDevInfo, dwIndex++, &sDevInfo);
		if( bRet == FALSE) break;

		// Get device name (description)...USB serial device
		dwSize = 0;
		bRet = ::SetupDiGetDeviceRegistryProperty( hDevInfo, &sDevInfo, SPDRP_DEVICEDESC, &dwRegType, NULL, 0, &dwSize);
		if( dwSize > sizeof( pszName)) dwSize = sizeof(pszName);
		bRet = ::SetupDiGetDeviceRegistryProperty( hDevInfo, &sDevInfo, SPDRP_DEVICEDESC, &dwRegType, (BYTE*)pszName, dwSize, &dwSize);
		if( bRet == FALSE){
			continue;
		}
		// Get friendly name...(USB Serial Device(COM43)) etc.
		// Don't continue even if you can't get it because it's often not possible to get it
		dwSize = 0;
		bRet = ::SetupDiGetDeviceRegistryProperty( hDevInfo, &sDevInfo, SPDRP_FRIENDLYNAME, &dwRegType, NULL,0, &dwSize);
		if( dwSize > sizeof( pszName)) dwSize = sizeof(pszName);
		bRet = ::SetupDiGetDeviceRegistryProperty( hDevInfo, &sDevInfo, SPDRP_FRIENDLYNAME, &dwRegType, (BYTE*)pszName, dwSize, &dwSize);
		if( bRet){
			fn = pszName;
		} else {
			fn = "";
		}
		// Get hardware ID　...USB\VID_0C2B&PID_0804&REV_0200
		dwSize = 0;
		bRet = ::SetupDiGetDeviceRegistryProperty( hDevInfo, &sDevInfo, SPDRP_HARDWAREID, &dwRegType, NULL,0, &dwSize);
		if( dwSize > sizeof( pszName)) dwSize = sizeof(pszName);
		bRet = ::SetupDiGetDeviceRegistryProperty( hDevInfo, &sDevInfo, SPDRP_HARDWAREID, &dwRegType, (BYTE*)pszName, dwSize, &dwSize);
		if( bRet){
			hd = pszName;
		} else {
			hd = "";
		}
		if(( fn.Find( comport) >= 0) && ( hd.Find( vpid) >= 0)){
			ret = TRUE;
			break;
		}
	}
	::SetupDiDestroyDeviceInfoList( hDevInfo);

	return ret;
}

bool CCC_ToolView::SetUSBTool( HANDLE h, int cmd, int index, char* p, int len)
{
	DWORD	wn, rn;
	USB_CMD	usb;
	int		i;
	char	rec[ 2];

	usb.cmd		= cmd;
	usb.length	= len;
	usb.index	= index;
	for( i = 0 ; i < len; i++){
		usb.buf[ i] = *p++;
	}
	WriteFile( h, (char*)&usb, 4+len, &wn, NULL);
	if( wn != ( 4+len)){
		return FALSE;
	}
	ReadFile( h, (LPVOID)rec, sizeof( rec), &rn, NULL);
	if( rn != sizeof( rec)){
		return FALSE;
	}
	return TRUE;
}


bool CCC_ToolView::GetUSBTool(HANDLE h, int cmd, int index, char* p, int len)
{
	DWORD	wn, rn;
	USB_CMD	usb;

	usb.cmd		= cmd;
	usb.length	= len;
	usb.index	= index;
	WriteFile( h, (char*)&usb, 4, &wn, NULL);
	if( wn != 4){
		return FALSE;
	}
	ReadFile( h, (LPVOID)p, len, &rn, NULL);
	if( rn != len){
		return FALSE;
	}
	return TRUE;
}

bool CCC_ToolView::GetUSBToolData(HANDLE h, int cmd, int index, int no, char* p, int len)
{
	DWORD	wn, rn;
	USB_CMD	usb;
	long	*pl;

	usb.cmd		= cmd;
	usb.length	= len;
	usb.index	= index;
	pl			= (long*)usb.buf;
	*pl			= no;
	WriteFile( h, (char*)&usb, 8, &wn, NULL);
	if( wn != 8){
		return FALSE;
	}
	ReadFile( h, (LPVOID)p, len, &rn, NULL);
	if( rn != len){
		return FALSE;
	}
	return TRUE;
}

CString CCC_ToolView::GetName(int no)
{
	CString	str = "";
	char	name[32];

	GetUSBTool( m_hCom[no], CMD_GET_NAME, 0, name, sizeof(name));
	str = name;
	return str;
}

CString CCC_ToolView::GetVer(int no)
{
	CString	str = "";
	char	ver[16];

	GetUSBTool( m_hCom[no], CMD_GET_VER, 0, ver, sizeof(ver));
	str = ver;
	return str;
}

int CCC_ToolView::GetKind(int no)
{
	int		kind;

	GetUSBTool( m_hCom[no], CMD_GET_KIND, 0, (char*)&kind, sizeof(kind));
	return kind;
}

CString CCC_ToolView::GetSerial(int no)
{
	CString	str = "";
	char	serial[16];

	GetUSBTool( m_hCom[no], CMD_GET_SERIAL, 0, serial, sizeof(serial));
	str = serial;
	return str;
}

CString CCC_ToolView::GetSensor(int no)
{
	CString	str = "";
	char	sensor[16];

	GetUSBTool( m_hCom[no], CMD_GET_SENSOR, 0, sensor, sizeof(sensor));
	str = sensor;
	return str;
}


void CCC_ToolView::CheckPara(int no)
{
	int	exp, avg;

	exp = GetExposure( no);
	if( exp > 1000000){
		exp = 1000000;
		SetExposure( no, exp);
	}
	avg = GetAverage( no);
	if( avg > 100){
		avg = 1;
		SetAverage( no, avg);
	}
}


void CCC_ToolView::GetData(int no)
{
	int		i;

	if (m_bDetectCH340E[ no]) {
		for (i = 0; i < 32; i++) {
			if (i == 31) {
				GetUSBTool(m_hCom[no], CMD_GET_DATA, i, (char*)&m_dData[no][i * 16], 60);
			}
			else {
				GetUSBTool(m_hCom[no], CMD_GET_DATA, i, (char*)&m_dData[no][i * 16], 64);
			}
		}
	}
	else {
		GetUSBTool(m_hCom[no], CMD_GET_DATA, 0, (char*)m_dData[no], GetDocument()->m_nWaveNum * 4);
	}
}

float CCC_ToolView::GetTemperature(int no)
{
	float	t;

	GetUSBTool(m_hCom[no], CMD_GET_TEMP, 0, (char*)&t, sizeof(t));
	return t;
}

int CCC_ToolView::GetExposure(int no)
{
	int	exp;

	GetUSBTool(m_hCom[no], CMD_GET_EXP, 0, (char*)&exp, sizeof(exp));
	return exp;
}

void CCC_ToolView::SetExposure(int no, int exp)
{
	SetUSBTool( m_hCom[no], CMD_SET_EXP, 0, (char*)&exp, sizeof( exp));
}

int CCC_ToolView::GetAverage(int no)
{
	int	avg;

	GetUSBTool(m_hCom[no], CMD_GET_AVG, 0, (char*)&avg, sizeof(avg));
	return avg;
}

int CCC_ToolView::GetInterval(int no)
{
	int	inerval = 0;

	if (m_dVer[no] >= 1.5) {
		GetUSBTool(m_hCom[no], CMD_GET_INTERVAL, 0, (char*)&inerval, sizeof(inerval));
	}
	return inerval;
}

void CCC_ToolView::SetAverage(int no, int avg)
{
	if (m_dVer[no] >= 1.5) {
		SetUSBTool(m_hCom[no], CMD_SET_AVG, 0, (char*)&avg, sizeof(avg));
	}
}


void CCC_ToolView::SetInterval(int no, int interval)
{
	if (m_dVer[no] >= 1.5) {
		SetUSBTool(m_hCom[no], CMD_SET_INTERVAL, 0, (char*)&interval, sizeof(interval));
	}
}

void CCC_ToolView::DoDark(int no)
{
	if (m_dVer[no] >= 1.5) {
		SetUSBTool(m_hCom[no], CMD_DO_DARK, 0, 0, 0);
	}
}

int CCC_ToolView::GetCPUID(int no)
{
	long id;

	GetUSBTool(m_hCom[no], CMD_GET_CPUID, 0, (char*)&id, sizeof(id));
	return id;
}

const	double	wave_rgb_table[45 + 27][4] = {	// Color table by wavelength (10nm unit)
	{ 340 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 350 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 360 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 370 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 380 ,0.06076 ,0.00000 ,0.11058 }
   ,{ 390 ,0.08700 ,0.00000 ,0.16790 }
   ,{ 400 ,0.13772 ,0.00000 ,0.26354 }
   ,{ 410 ,0.20707 ,0.00000 ,0.39852 }
   ,{ 420 ,0.31129 ,0.00000 ,0.60684 }
   ,{ 430 ,0.39930 ,0.00000 ,0.80505 }
   ,{ 440 ,0.40542 ,0.00000 ,0.87684 }
   ,{ 450 ,0.34444 ,0.00000 ,0.88080 }
   ,{ 460 ,0.11139 ,0.00000 ,0.86037 }
   ,{ 470 ,0.00000 ,0.15233 ,0.77928 }
   ,{ 480 ,0.00000 ,0.38550 ,0.65217 }
   ,{ 490 ,0.00000 ,0.49412 ,0.51919 }
   ,{ 500 ,0.00000 ,0.59271 ,0.40008 }
   ,{ 510 ,0.00000 ,0.69549 ,0.25749 }
   ,{ 520 ,0.00000 ,0.77773 ,0.00000 }
   ,{ 530 ,0.00000 ,0.81692 ,0.00000 }
   ,{ 540 ,0.00000 ,0.82625 ,0.00000 }
   ,{ 550 ,0.00000 ,0.81204 ,0.00000 }
   ,{ 560 ,0.47369 ,0.77626 ,0.00000 }
   ,{ 570 ,0.70174 ,0.71523 ,0.00000 }
   ,{ 580 ,0.84922 ,0.62468 ,0.00000 }
   ,{ 590 ,0.94726 ,0.49713 ,0.00000 }
   ,{ 600 ,0.99803 ,0.31072 ,0.00000 }
   ,{ 610 ,1.00000 ,0.00000 ,0.00000 }
   ,{ 620 ,0.95520 ,0.00000 ,0.00000 }
   ,{ 630 ,0.86620 ,0.00000 ,0.00000 }
   ,{ 640 ,0.76170 ,0.00000 ,0.00000 }
   ,{ 650 ,0.64495 ,0.00000 ,0.00000 }
   ,{ 660 ,0.52857 ,0.00000 ,0.00000 }
   ,{ 670 ,0.41817 ,0.00000 ,0.00000 }
   ,{ 680 ,0.33202 ,0.00000 ,0.00000 }
   ,{ 690 ,0.25409 ,0.00000 ,0.00000 }
   ,{ 700 ,0.19695 ,0.00000 ,0.00000 }
   ,{ 710 ,0.15326 ,0.00000 ,0.00000 }
   ,{ 720 ,0.11902 ,0.00000 ,0.00000 }
   ,{ 730 ,0.09063 ,0.00000 ,0.00000 }
   ,{ 740 ,0.06898 ,0.00000 ,0.00000 }
   ,{ 750 ,0.05150 ,0.00000 ,0.00000 }
   ,{ 760 ,0.04264 ,0.00000 ,0.00000 }
   ,{ 770 ,0.03666 ,0.00000 ,0.00794 }
   ,{ 780 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 790 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 800 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 810 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 820 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 830 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 840 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 850 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 860 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 870 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 880 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 890 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 900 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 910 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 920 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 930 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 940 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 950 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 960 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 970 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 980 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 990 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 1000 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 1010 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 1020 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 1030 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 1040 ,0.00000 ,0.00000 ,0.00000 }
   ,{ 1050 ,0.00000 ,0.00000 ,0.00000 }
};

void CCC_ToolView::MakeBarLine(void)
{
	int		i;

	for (i = 0; i < MAX_WAVE; i++) {
		m_pLinew[i] = new CPen(PS_SOLID, 1, RGB(255.0 * wave_rgb_table[i / 10][1], 255.0 * wave_rgb_table[i / 10][2], 255.0 * wave_rgb_table[i / 10][3]));
	}
}

void CCC_ToolView::MakeXYZTable(void)
{
	int	i;

	for( i = 0 ; i < MAX_WAVE; i++){
		if( i < ( 380 - 340)){
			m_dXYZ[ i][ 0] = 0.0;
			m_dXYZ[ i][ 1] = 0.0;
			m_dXYZ[ i][ 2] = 0.0;
		} else if( i < ( 825 - 340)){
			m_dXYZ[ i][ 0] = xyz[ (i - ( 380 - 340))/5][ 1] + (xyz[ (i - ( 380 - 340))/5+1][ 1]- xyz[ (i - ( 380 - 340))/5][ 1]) * ( i % 5)/5 ;
			m_dXYZ[ i][ 1] = xyz[ (i - ( 380 - 340))/5][ 2] + (xyz[ (i - ( 380 - 340))/5+1][ 2]- xyz[ (i - ( 380 - 340))/5][ 2]) * ( i % 5)/5 ;
			m_dXYZ[ i][ 2] = xyz[ (i - ( 380 - 340))/5][ 3] + (xyz[ (i - ( 380 - 340))/5+1][ 3]- xyz[ (i - ( 380 - 340))/5][ 3]) * ( i % 5)/5 ;
		} else {
			m_dXYZ[ i][ 0] = 0.0;
			m_dXYZ[ i][ 1] = 0.0;
			m_dXYZ[ i][ 2] = 0.0;
		}
	}
}

void CCC_ToolView::GetXY( int no, float *x, float *y)
{
	int		start, end;
	float	dx, dy, dz;
	int		i;
	float	*pData;

	if( m_bMonitor){
		pData = m_dData[ no];
	} else {
		pData = GetDocument()->m_dData[ no][ m_nDocNo];
	}
	start	= 380 - 340;
	end		= 825 - 340;
	dx = dy= dz = 0.0;
	for( i = start; i < end; i++){
		dx += pData[ i] * m_dXYZ[ i][ 0];
		dy += pData[ i] * m_dXYZ[ i][ 1];
		dz += pData[ i] * m_dXYZ[ i][ 2];
	}
	*x = dx/( dx + dy + dz);
	*y = dy/( dx + dy + dz);
}

void CCC_ToolView::OnBnClickedButtonExpDown()
{
	int		exp;
	CString	str;

	m_ctlExpTime.GetWindowText( str);
	exp = atoi( str);
	if( exp >= 200000){
		exp -= 100000;
	} else if( exp >= 20000){
		exp -= 10000;
	} else if( exp >= 2000){
		exp -= 1000;
	} else if( exp >= 200){
		exp -= 100;
	} else {
		exp -= 10;
	}
	str.Format( "%5d", exp);
	m_ctlExpTime.SetWindowText( str);
	m_bExposure = TRUE;
}

void CCC_ToolView::OnBnClickedButtonExpUp()
{
	int		exp;
	CString	str;

	m_ctlExpTime.GetWindowText( str);
	exp = atoi( str);
	if( exp < 1000){
		exp += 100;
	} else if( exp < 10000){
		exp += 1000;
	} else if( exp < 100000){
		exp += 10000;
	} else {
		exp += 100000;
	}
	str.Format( "%5d", exp);
	m_ctlExpTime.SetWindowText( str);
	m_bExposure = TRUE;
}

void CCC_ToolView::OnLbnSelchangeListSerial()
{
	int		exp, avg, interval;
	CString	str;

	m_nSelect = m_ctlSerial.GetCurSel();
	exp = GetExposure( m_nSelect);
	str.Format( "%d", exp);
	m_ctlExpTime.SetWindowText( str);
	avg = GetAverage( m_nSelect);
	m_ctlAverage.SetCurSel( avg -1);
	if (m_dVer[m_nSelect] >= 1.5) {
		interval = GetInterval(m_nSelect);
		str.Format("%d", interval);
		m_ctlAvgInterval.SetWindowText(str);
	}
}

void CCC_ToolView::OnBnClickedButtonExpUpdate()
{
	m_bExposure = TRUE;
}

void CCC_ToolView::OnLbnSelchangeListData()
{
	m_bMonitor = FALSE;
	m_ctlGetData.SetWindowText( "Monitor resumes");
	m_nDocNo	= m_ctlData.GetCurSel();
	SetButton( GetDocument()->m_nSpectro);
	DispGraph( m_nSelect);
}

void CCC_ToolView::OnBnClickedButtonGetData()
{
	CString	str;

	if( m_bMonitor){
		if (m_bGetData) {
			m_bGetData = FALSE;
			m_ctlGetData.SetWindowText("Data acquisition");
		}
		else {
			m_bGetData = TRUE;
			m_nStartTime = GetTickCount();
			m_ctlRepeatTime.GetWindowText(str);
			m_nIntervalTime = atoi(str);
			if (m_ctlRepeat.GetCheck()) {
				m_ctlGetData.SetWindowText("Stop acquiring");
			}
		}
	} else {
		m_bMonitor = TRUE;
		m_ctlData.SetCurSel( -1);
		m_ctlGetData.SetWindowText( "Data acquisition");
		SetButton( GetDocument()->m_nSpectro);
		SetTimer( 2, 10, NULL);
	}
}

void CCC_ToolView::SetButton( bool bDevice)
{
	m_ctlExpTime.EnableWindow( bDevice);
	m_ctlExpUp.EnableWindow( bDevice);
	m_ctlExpDown.EnableWindow( bDevice);
	m_ctlExpUpdate.EnableWindow( bDevice);
	m_ctlGetData.EnableWindow( bDevice);
	m_ctlDelData.EnableWindow( !m_bMonitor);
	m_ctlSetRef.EnableWindow( !m_bMonitor);
	m_ctlCSVSave.EnableWindow( m_ctlData.GetCount());
	m_ctlRefSample.EnableWindow( !m_bMonitor);
	m_ctlAverage.EnableWindow( bDevice);
}

void CCC_ToolView::PutData(void)
{
	int	i, j;
	int	no;
	CTime	now;

	now = now.GetCurrentTime();
	if (GetDocument()->m_nDataNum < MAX_DATA) {
		no = GetDocument()->m_nDataNum;
		for (i = 0; i < GetDocument()->m_nSpectro; i++) {
			for (j = 0; j < GetDocument()->m_nWaveNum; j++) {
				GetDocument()->m_dData[i][no][j] = m_dData[i][j];
			}
			GetDocument()->m_dTemp[i][no] = m_dTemp[i];
			GetDocument()->m_nExp[i][no] = GetExposure(i);
		}
		GetDocument()->m_dTime[no] = now;
		m_ctlData.AddString(now.Format("%Y%m%d_%H%M%S"));
		GetDocument()->m_nDataNum++;
		GetDocument()->SetModifiedFlag();
		m_ctlCSVSave.EnableWindow(m_ctlData.GetCount());
	}
}

void CCC_ToolView::PutData( CString name, int next)
{
	int	no = GetDocument()->m_nDataNum;
	int	i;

	TCHAR	path[MAX_PATH];
	CString	file;

	PutData();
	if ( name != "") {
		::GetModuleFileName(NULL, path, MAX_PATH);
		file = path;
		for (i = file.GetLength() - 1; i > 0; i--) {
			if (file.GetAt(i) == '\\') {
				break;
			}
		}
		file = file.Left(i+1);
		file += name;
		SaveNowData( file, no, next);
	}
}

void CCC_ToolView::OnBnClickedButtonDel()
{
	int	sel;

	sel = m_ctlData.GetCurSel();
	if( sel >= 0){
		GetDocument()->DeleteData( sel);
		RefreshDataList();
		if( sel < GetDocument()->m_nDataNum){
			m_ctlData.SetCurSel( sel);
		} else {
			m_ctlDelData.EnableWindow( FALSE);
			m_ctlSetRef.EnableWindow( FALSE);
		}
		GetDocument()->SetModifiedFlag();
	}
}

void CCC_ToolView::RefreshDataList(void)
{
	int	i;

	m_ctlData.ResetContent();
	for( i = 0; i < GetDocument()->m_nDataNum; i++){
		if( i == GetDocument()->m_nRefSelect){
			m_ctlData.AddString( GetDocument()->m_dTime[ i].Format( "%Y%m%d_%H%M%S [standard]"));
		} else if( i == m_nRefSample){
			m_ctlData.AddString( GetDocument()->m_dTime[ i].Format( "%Y%m%d_%H%M%S [reference]"));
		} else {
			m_ctlData.AddString( GetDocument()->m_dTime[ i].Format( "%Y%m%d_%H%M%S"));
		}
	}
	m_ctlCSVSave.EnableWindow( m_ctlData.GetCount());
}

void CCC_ToolView::OnBnClickedButtonRefData()
{
	int	sel;

	sel = m_ctlData.GetCurSel();
	if( sel >= 0){
		GetDocument()->m_nRefSelect = sel;
		RefreshDataList();
		m_ctlData.SetCurSel( sel);
		DispGraph( m_nSelect);
		GetDocument()->SetModifiedFlag();
	}	
}

void CCC_ToolView::SaveNowData(CString path, int no, int next)
{
	FILE* stream;
	CString	str, temp;
	int		i, k;
	CString	csv[511];

	stream = fopen(path, "a");
	if ( stream != NULL) {
		if (next == 0) {
			str.Format("ColorCompass Spectrum data( MFA Model)\n");
			fputs(str, stream);
			str.Format("device,");
			for (i = 0; i < GetDocument()->m_nSpectro; i++) {
				temp.Format("%s,", GetDocument()->m_strSerial[i]);
				str += temp;
			}
			str += "\n";
			fputs(str, stream);
			str.Format("date,time,");
			for (i = 0; i < GetDocument()->m_nSpectro; i++) {
				str += "temp,";
				for (k = 0; k < GetDocument()->m_nWaveNum; k++) {
					temp.Format("%3d,", 340+k);
					str += temp;
				}
			}
			str += "\n";
			fputs(str, stream);
		}
		str = "";
		str += GetDocument()->m_dTime[no].Format("%Y/%m/%d,");
		str += GetDocument()->m_dTime[no].Format("%H:%M:%S,");
		for (i = 0; i < GetDocument()->m_nSpectro; i++) {
			temp.Format("%3.1f,", GetDocument()->m_dTemp[i][no]);
			str += temp;
			for (k = 0; k < GetDocument()->m_nWaveNum; k++) {
				temp.Format("%3.1f,", GetDocument()->m_dData[i][no][k]);
				str += temp;
			}
		}
		str += "\n";
		fputs( str, stream);
		fclose( stream);
	}
}

void CCC_ToolView::OnBnClickedButtonSaveCsv()
{
	CString	path;
	FILE	*stream;
	CString	str, temp;
	int		i, j, k;
	CString	csv[ 511];

	CFileDialog dialog(	FALSE,
						_T("csv"),
						_T("*.csv"),
						OFN_FILEMUSTEXIST|OFN_SHOWHELP,
														// OPENFILENAME		OFN_CREATEPROMPT
						"csv file(*.csv)|*.csv||",
						NULL					// 
						);
	if( dialog.DoModal() == IDOK){
		path = dialog.GetPathName();
		stream = fopen( path, "w");
		if( stream != NULL){
			str.Format( "ColorCompass Spectrum data( MFA Model)\n");
			fputs( str, stream);
			str.Format( "device,");
			for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
				for( j = 0 ; j < GetDocument()->m_nDataNum; j++){
					temp.Format( "%s,", GetDocument()->m_strSerial[ i]);
					str += temp;
				}
			}
			str += "\n";
			fputs( str, stream);
			str.Format( "date,");
			for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
				for( j = 0 ; j < GetDocument()->m_nDataNum; j++){
					str += GetDocument()->m_dTime[ j].Format( "%Y/%m/%d,");
				}
			}
			str += "\n";
			fputs( str, stream);
			str.Format( "time,");
			for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
				for( j = 0 ; j < GetDocument()->m_nDataNum; j++){
					str += GetDocument()->m_dTime[ j].Format( "%H:%M:%S,");
				}
			}
			str += "\n";
			fputs( str, stream);
			str.Format( "temperature,");
			for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
				for( j = 0 ; j < GetDocument()->m_nDataNum; j++){
					temp.Format( "%3.1f,", GetDocument()->m_dTemp[ i][ j]);
					str += temp;
				}
			}
			str += "\n";
			fputs( str, stream);
			for( k = 0 ; k < GetDocument()->m_nWaveNum; k++){
				str.Format( "%d,", k + GetDocument()->m_nStartWave);
				csv[ k] = str;
			}
			for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
				for( j = 0 ; j < GetDocument()->m_nDataNum; j++){
					for( k = 0 ; k < GetDocument()->m_nWaveNum; k++){
						str.Format( "%3.1f,", GetDocument()->m_dData[ i][ j][ k]);
						csv[ k] += str;
					}
				}
			}
			for( k = 0 ; k < GetDocument()->m_nWaveNum; k++){
				csv[ k] += "\n";
				fputs( csv[ k], stream);
			}
			fclose( stream);
		}
	}
	
}

#include "EditNumDlg.h"

void CCC_ToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CEditNumDlg	dlg;

	point	-= CPoint( 5, 30);

	if(( point.x >= m_rectGraph.left) && ( point.x <= ( m_rectGraph.left + 50)) && ( point.y >= m_rectGraph.top) && ( point.y <= ( m_rectGraph.top + 30))){
		if( m_nGraphKind != graph_spectro_ref){
			dlg.num = m_nGMax;
			if( dlg.DoModal() == IDOK){
				m_nGMax = dlg.num;
			}
		} else {
			dlg.num = m_nRMax;
			if( dlg.DoModal() == IDOK){
				m_nRMax = dlg.num;
			}
		}
		DispGraph( m_nSelect);
	} else if(( point.x >= m_rectGraph.left) && ( point.x <= ( m_rectGraph.left + 50)) && ( point.y >= m_rectGraph.bottom-30) && ( point.y <= ( m_rectGraph.bottom))){
		if( m_nGraphKind != graph_spectro_ref){
			dlg.num = m_nGMin;
			if( dlg.DoModal() == IDOK){
				m_nGMin = dlg.num;
			}
		} else {
			dlg.num = m_nRMin;
			if( dlg.DoModal() == IDOK){
				m_nRMin = dlg.num;
			}
		}
		DispGraph( m_nSelect);
	}
	CFormView::OnLButtonDown(nFlags, point);
}

void CCC_ToolView::OnBnClickedButtonRefsample()
{
	int	sel;

	sel = m_ctlData.GetCurSel();

	if( sel != GetDocument()->m_nRefSelect){
		m_nRefSample = sel;
		RefreshDataList();
	} else {
		AfxMessageBox( "Standard data cannot be selected for reference");
		return;
	}
}

void CCC_ToolView::OnCbnSelchangeComboAverage()
{
	int	sel;

	sel = m_ctlAverage.GetCurSel();
	if( sel >= 0){
		m_nAverage = sel + 1;
	}
}



void CCC_ToolView::OnBnClickedButtonRedetect()
{
	int		i;
	int		exp, avg;
	CString	str;

	for( i = 0 ; i < MAX_DEVICE; i++){
		if( m_hCom[ i] != NULL){
			CloseHandle( m_hCom[ i]);
			m_hCom[ i] = NULL;
		}
	}
	m_ctlSerial.ResetContent();
	GetDocument()->m_nSpectro		= GetDevice();
	for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
		GetDocument()->m_strSerial[ i] = GetSerial( i);
		GetDocument()->m_strSensor[ i] = GetSensor( i);
		CheckPara( i);
	}
	if( GetDocument()->m_nSpectro){
		m_ctlSerial.SetCurSel( m_nSelect);
		exp = GetExposure( m_nSelect);
		str.Format( "%d", exp);
		m_ctlExpTime.SetWindowText( str);
		for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
			if( GetDocument()->m_strSensor[ i] == "C14384MA-01"){
				GetDocument()->m_nWaveNum	= 1050 - 640 +1;
				GetDocument()->m_nStartWave	= 640;
				m_bC14384MA	= TRUE;
			} else {
				GetDocument()->m_nWaveNum	= MAX_WAVE;
				GetDocument()->m_nStartWave	= START_WAVE;
				m_bC14384MA	= FALSE;
			}
		}
		avg = GetAverage( m_nSelect);
		m_ctlAverage.SetCurSel( avg -1);
	} else {
		for( i = 0 ; i < GetDocument()->m_nSpectro; i++){
			m_ctlSerial.AddString( GetDocument()->m_strSerial[ i]);
			if( GetDocument()->m_strSensor[ i] == "C14384MA-01"){
				GetDocument()->m_nWaveNum	= 1050 - 640 +1;
				GetDocument()->m_nStartWave	= 640;
				m_bC14384MA	= TRUE;
			} else {
				GetDocument()->m_nWaveNum	= MAX_WAVE;
				GetDocument()->m_nStartWave	= START_WAVE;
				m_bC14384MA	= FALSE;
			}
		}
	}
	if( GetDocument()->m_nDataNum){
		RefreshDataList();
	}
	SetButton( GetDocument()->m_nSpectro);
	if( m_nGraphKind == graph_spectro_ref){
		if( GetDocument()->m_nRefSelect < 0){
			m_nGraphKind = graph_spectro;
		} else if( GetDocument()->m_nRefSelect >= GetDocument()->m_nDataNum){
			m_nGraphKind = graph_spectro;
		}
	}
	m_ctlGraphKind.SetCurSel( m_nGraphKind);
	if( m_bMonitor){					// monitor start
		SetTimer( 1, 100, NULL);
	}
//	m_ctlGraph.GetClientRect( m_rectGraph);
}


void CCC_ToolView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

		backup_type = nType;
		backup_cx = cx;
		backup_cy = cy;

	if (m_bNotFirst) {
			SetSize(cx, cy);
			SetTimer(1, 100, NULL);
	}
}

#define	GRAPH_WIDTH	620

void CCC_ToolView::SetSize(int cx, int cy)
{
	int		top = 30;
	int		y;
	int		x = 5;
	int		gw = GRAPH_WIDTH;
	int		gh = cy - 50;
	int		step = 35;
	int		step2 = 45;
	int		bw = 270;
	int		bh = 30;
	int		sh = 200;												// serial box hight
	int		lh = gh - 285 - sh;									// list height

	y = top+20;
	m_ctlGraph.MoveWindow(x, y, gw, gh);
	m_rectGraph = CRect(x, y, x+gw, y+gh);		// graph area

	m_ctlGraphKind.MoveWindow(x + gw - 200, y - 32, 200, 160);

	x = x + gw + 5;
	m_ctlReDetect.MoveWindow(x, y-32, bw, bh);
	m_ctlSerial.MoveWindow(x, y, bw, sh);						y += (sh+5);
	m_ctlExpName.MoveWindow(x, y, bw / 2, bh);				y += step;

	m_ctlExpDown.MoveWindow(x, y, 40, bh);
	m_ctlExpTime.MoveWindow(x+ 50, y, 80, bh);
	m_ctlExpUpdate.MoveWindow(x + 140, y, 60, bh);
	m_ctlExpUp.MoveWindow(x + 210, y, 40, bh);					y += step2;

	m_ctlAvgName.MoveWindow(x, y+5, 80, bh);	
	m_ctlAverage.MoveWindow(x + 90, y, 70, bh*10);
	if (m_dVer[0] >= 1.5) {
		m_ctlAvgInterval.ShowWindow(TRUE);
		m_ctlAvgInterval.MoveWindow(x + 170, y, 60, bh);
	}
	else {

		m_ctlAvgInterval.ShowWindow(FALSE);
	}
																				y += step2;

	m_ctlGetData.MoveWindow(x, y, 120, bh);
	m_ctlRepeat.MoveWindow(x + 125, y, 80, bh);
	m_ctlRepeatTime.MoveWindow(x + 125+85, y, 60, bh);	y += step2;
	m_ctlDataListName.MoveWindow(x, y, bw/2, bh);

	m_ctlPlan.MoveWindow(x+bw/2+5, y-5, bw/2-5, bh);		y += (step - 5);
	m_ctlData.MoveWindow(x, y, bw, lh);							y += (lh+5);		// list height changable!

	m_ctlDelData.MoveWindow(x, y, 60, bh);
	m_ctlSetRef.MoveWindow(x + 70, y, 100, bh);
	m_ctlRefSample.MoveWindow(x+180, y, 80, bh);				y += step2;

	m_ctlCSVSave.MoveWindow(x, y, bw, bh);
}

#include "CMeasurePlanDlg.h"

void CCC_ToolView::OnBnClickedButtonPlan()
{
	CMeasurePlanDlg	dlg( this);

	dlg.DoModal();
}


void CCC_ToolView::OnDoDark()
{
	if (AfxMessageBox("Make dark settings. Is it OK", MB_OKCANCEL) == IDOK) {
		m_bDoDark = TRUE;
	}
}


void CCC_ToolView::OnUpdateDoDark(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_dVer[0] >= 1.50);
}


