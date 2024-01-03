// CC_Tool.cpp : Define class behavior for your application.
//

#include "stdafx.h"
#include "CC_Tool.h"
#include "MainFrm.h"

#include "CC_ToolDoc.h"
#include "CC_ToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCC_ToolApp

BEGIN_MESSAGE_MAP(CCC_ToolApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCC_ToolApp::OnAppAbout)
	// Standard file base document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CCC_ToolApp construction

CCC_ToolApp::CCC_ToolApp()
{
	// TODO: Add the construction code at this location.
	// Write all important initialization operations during InitInstance here.
}


// The only CCC_ToolApp object.

CCC_ToolApp theApp;

char	szInstance[] = {"colorcompassToolMFA"};

// CCC_ToolApp Initialization

BOOL CCC_ToolApp::InitInstance()
{
	// In order for the application manifest to enable visual styles,
	// If you want to specify the use of ComCtl32.dll Version 6 or later,
	// InitCommonControlsEx() is required on Windows XP. Otherwise all window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// To include all common control classes used in your application, use
	// Set this:
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize the OLE library.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// standard initialization
	// The final executable without these features
	// If you want to reduce the size, remove unnecessary initialization from below
	// Delete the routine.
	// Modify the registry key where the settings are stored.
	// TODO: to any appropriate string such as company or organization name
	// Please change this string.
	HANDLE	hPrevMutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, szInstance);	// [Multiple start prevention]

	if( hPrevMutex){														// [Multiple start prevention]
		CloseHandle( hPrevMutex);											// [Multiple start prevention]
		return FALSE;														// [Multiple start prevention]
	}
	m_hMutex = CreateMutex( FALSE, 0, szInstance);							// [Multiple start prevention]

	SetRegistryKey(_T("CC_Tool2021 application soft "));
	LoadStdProfileSettings(4);  // Load options from standard INI file (including MRU)
// Register a document template for the application. document template
// serves to combine the document, frame window and view.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCC_ToolDoc),
		RUNTIME_CLASS(CMainFrame),       // Main SDI frame window
		RUNTIME_CLASS(CCC_ToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Enable DDE Execute open.
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parses command lines for standard shell commands such as DDE, file open, etc.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// The dispatch command specified on the command line. The application
	// Returns False if started with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	//Now that the main window is initialized, it's time to display and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// Call DragAcceptFiles only if the suffix exists.
	// In SDI applications, this call must occur immediately after the ProcessShellCommand.
	// Allow drag/drop open.
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// CAboutDlg dialog used for application version information

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

//dialog data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckAutoSave();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Application command to run a dialog
void CCC_ToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CCC_ToolApp message handler



