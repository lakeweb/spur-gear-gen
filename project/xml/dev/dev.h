// dev.h : main header file for the dev application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CdevApp:
// See dev.cpp for the implementation of this class
//

class CdevApp : public CWinApp
{
public:
	CdevApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CdevApp theApp;