// MainFrm.cpp : Implementing the CMainFrame class
//

#include "stdafx.h"
#include "CC_Tool.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           //status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame Construction/Destruction

CMainFrame::CMainFrame()
{
	// TODO: Add your member initialization code here.
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar.\n");
		return -1;      // Could not be created.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar.\n");
		return -1;      // Could not be created.
	}

	//TODO: If you don't want the toolbar to be dockable, remove these three lines.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CWinApp*	pApp;												// [Size restoration]
	CRect		rect;												// [Size restoration]

	pApp = AfxGetApp();
	rect.left	= pApp->GetProfileInt( "Settings", "left", 0);		// [Size restoration]
	rect.right	= pApp->GetProfileInt( "Settings", "right", 740);	// [Size restoration]
	rect.top	= pApp->GetProfileInt( "Settings", "top", 0);		// [Size restoration]
	rect.bottom = pApp->GetProfileInt( "Settings", "bottom", 640);	// [Size restoration]
	MoveWindow( rect);												// [Size restoration]

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify CREATESTRUCT cs at this location to set Window class or style.
	//  Please correct.

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;

	return TRUE;
}


// CMainFrame diagnosis

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handler




void CMainFrame::OnDestroy()
{
	WINDOWPLACEMENT	wndpl;													// [Size restoration]

	GetWindowPlacement( &wndpl);											// [Size restoration]

	CWinApp*	pApp;														// [Size restoration]
	pApp = AfxGetApp();														// [Size restoration]

	pApp->WriteProfileInt( "Settings", "left", wndpl.rcNormalPosition.left);	// [Size restoration]
	pApp->WriteProfileInt( "Settings", "right", wndpl.rcNormalPosition.right);	// [Size restoration]
	pApp->WriteProfileInt( "Settings", "top", wndpl.rcNormalPosition.top);		// [Size restoration]
	pApp->WriteProfileInt( "Settings", "bottom", wndpl.rcNormalPosition.bottom);// [Size restoration]

	CFrameWnd::OnDestroy();

	//TODO: Add your message handler code here.
}
