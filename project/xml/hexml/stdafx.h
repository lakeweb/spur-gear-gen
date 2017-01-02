// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define HAS_ITERATOR_DEBUGGING 0

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
/*
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif
*/
#ifdef _WINDOWS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#ifndef __AFXDISP_H__
#include <afxdisp.h>
#endif //__AFXDISP_H__
#endif // _WINDOWS
#include <afxcmn.h>

#include <vector>
#include <string>
#include <map>
#include <boost/locale.hpp>


#ifdef _DEBUG
#define TINYXPATH_DEBUG
#endif

#include "../tinyxml/tinyxml.h"
#include "../tinyxml/lex_util.h"
#include "../tinyxml/xpath_processor.h"
