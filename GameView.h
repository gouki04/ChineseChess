// GameView.h : interface of the CGameView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEVIEW_H__F63950CD_62BA_428E_BB55_8C255D7CF0A8__INCLUDED_)
#define AFX_GAMEVIEW_H__F63950CD_62BA_428E_BB55_8C255D7CF0A8__INCLUDED_

#include "DibSection.h"	// Added by ClassView
#include "MapManager.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGameView window

class CGameView : public CWnd
{
// Construction
public:
	CGameView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGameView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGameView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CMapManager manager;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEVIEW_H__F63950CD_62BA_428E_BB55_8C255D7CF0A8__INCLUDED_)
