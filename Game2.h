// Game2.h : main header file for the GAME2 application
//

#if !defined(AFX_GAME2_H__72273E8F_010E_40DA_AEDC_8DC9FADF7687__INCLUDED_)
#define AFX_GAME2_H__72273E8F_010E_40DA_AEDC_8DC9FADF7687__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGameApp:
// See Game2.cpp for the implementation of this class
//

class CGameApp : public CWinApp
{
public:
	CGameApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CGameApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAME2_H__72273E8F_010E_40DA_AEDC_8DC9FADF7687__INCLUDED_)
