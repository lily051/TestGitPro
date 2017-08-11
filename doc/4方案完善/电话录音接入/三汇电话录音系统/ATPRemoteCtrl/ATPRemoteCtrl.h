// ATPRemoteCtrl.h : main header file for the ATPREMOTECTRL application
//

#if !defined(AFX_ATPREMOTECTRL_H__62ED5975_5142_46A6_90AF_292DEB49E05C__INCLUDED_)
#define AFX_ATPREMOTECTRL_H__62ED5975_5142_46A6_90AF_292DEB49E05C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CATPRemoteCtrlApp:
// See ATPRemoteCtrl.cpp for the implementation of this class
//

class CATPRemoteCtrlApp : public CWinApp
{
public:
	CATPRemoteCtrlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATPRemoteCtrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CATPRemoteCtrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPREMOTECTRL_H__62ED5975_5142_46A6_90AF_292DEB49E05C__INCLUDED_)
