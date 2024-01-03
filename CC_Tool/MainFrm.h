// MainFrm.h : CMainFrame class interface
//


#pragma once

class CMainFrame : public CFrameWnd
{
	
protected: // Create only from serialization.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// attribute
public:

// operation
public:

// override
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // Control bar member
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message allocation function
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};


