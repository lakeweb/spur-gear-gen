
// Gears.h : main header file for the Gears application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGearsApp:
// See Gears.cpp for the implementation of this class
//

class CGearsApp : public CWinAppEx
{
public:
	CGearsApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
};

extern CGearsApp theApp;
