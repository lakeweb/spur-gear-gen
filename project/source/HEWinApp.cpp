

//todo: after creating this class I got a couple of memory leaks..

//TODO !!!!!! BIG ONE this memory was allocated in the xml dll; Must destroy there!
//and better yet, use a smart pointer that does so


#include "stdafx.h"
#include <debug.h>


SP_XML sp_xml_settings_store_global;
SP_XMLNODE sp_currentNode_settings_store_global;
CString strSysRegPath;

// This (was*) a part of the Microsoft Foundation Classes C++ library.
// Hacked for XML storage
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "HEWinApp.h"
//#include "resource.h"
//#include "CmdArgs.h"

IMPLEMENT_DYNCREATE( XMLSettingsStore, CSettingsStore )

static CString __stdcall PreparePath(LPCTSTR lpszPath)
{
	ENSURE(lpszPath != NULL);

	CString strPath = lpszPath;

	int iPathLen = strPath.GetLength();
	if (iPathLen > 0 && strPath [iPathLen - 1] == _T('\\'))
	{
		strPath = strPath.Left(iPathLen - 1);
	}

	return strPath;
}

// .................................................................
bool XMLSettingsStore::OpenXML( LPCTSTR psPathFile )
{
	ASSERT( sp_xml.use_count( ) );
	if( ! sp_xml->Open( psPathFile, CXML::modeProgFile ) )
		return false;

	*sp_currentNode= sp_xml->GetCurrentNode( );
	return sp_currentNode->IsValid( );
}

// .................................................................
XMLSettingsStore::XMLSettingsStore()
	:m_bReadOnly( FALSE )
	,m_bAdmin( FALSE )
	,m_dwUserData( 0 )
	,sp_xml( sp_xml_settings_store_global )
	,sp_currentNode( sp_currentNode_settings_store_global )
	,psRegPath( strSysRegPath )
{
}

// .................................................................
XMLSettingsStore::XMLSettingsStore(BOOL bAdmin, BOOL bReadOnly)
	:m_bReadOnly( bReadOnly )
	,m_bAdmin( bAdmin )
	,m_dwUserData( 0 )
	,sp_xml( sp_xml_settings_store_global )
	,sp_currentNode( sp_currentNode_settings_store_global )
	,psRegPath( strSysRegPath )
{
	m_reg.m_hKey = bAdmin ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
}

// .................................................................
void XMLSettingsStore::SetCurrentNode( LPCTSTR psNode )
{
	*sp_currentNode= sp_xml->GetNode( psNode, ! m_bReadOnly );
}

// .................................................................
XMLSettingsStore::~XMLSettingsStore()
{
	Close( );
}

// .................................................................
bool XMLSettingsStore::MakePath( LPCTSTR psIn, CString& strIn )
{
	bool bNoDummy= false;
	CString strT( psIn );
	CString strR( psRegPath );
	if( strT.Left( strR.GetLength( ) ) == strR )
	{
		strT.Delete( 0, strR.GetLength( ) );
		if( strT.GetLength( ) )
			strT.Insert( 0, _T("/Root/Workspace/") );
		else
			strT= _T("/Root/Workspace");
	}
	else
	{
		bNoDummy= true;
		XMLSTRACE( _T("NO DUMMY: %s\n"), psIn );
	}
	if( ! strT.IsEmpty( ) && strT[ strT.GetLength( ) - 1 ] == _T('\\') )
	{
		strT.Delete( strT.GetLength( ) - 1 );
		strT+= _T('/');
	}
	strIn= strT;
	return bNoDummy;
}

// .................................................................
BOOL XMLSettingsStore::Open( LPCTSTR lpszPath )
{
	CString strT;
	if( MakePath( lpszPath, strT ) )
		return FALSE;

	XMLSTRACE( _T("Open %s: %s\n"), strT, m_bReadOnly ? _T("false") : _T("true ") );
	*sp_currentNode= sp_xml->GetNode( strT, ! m_bReadOnly );
	return sp_currentNode->IsValid( );
}

// .................................................................
BOOL XMLSettingsStore::CreateKey( LPCTSTR lpszPath )
{
	CString strT;
	if( MakePath( lpszPath, strT ) )
		return FALSE;

	XMLSTRACE( _T("Create %s: %s\n"), strT, m_bReadOnly ? _T("false") : _T("true ") );
	if( m_bReadOnly )
	{
		ASSERT( FALSE );
		return FALSE;
	}
	*sp_currentNode= sp_xml->GetNode( strT, ! m_bReadOnly );//testing
	return sp_currentNode->IsValid( );
//	return m_reg.Create(m_reg.m_hKey, PreparePath(lpszPath)) == ERROR_SUCCESS;
}

// .................................................................
void XMLSettingsStore::Close( )
{
	m_reg.Close( );
}

// .................................................................
BOOL XMLSettingsStore::Read(LPCTSTR lpszValueName, int& nValue)
{
	return Read(lpszValueName, (DWORD&)nValue);
}

// .................................................................
BOOL XMLSettingsStore::Read(LPCTSTR lpszValueName, DWORD& dwValue)
{
	sp_currentNode->GetElementValue( lpszValueName, dwValue, dwValue );
	return TRUE;
}

// .................................................................
BOOL XMLSettingsStore::Read(LPCTSTR lpszValueName, CString& strValue)
{
	ENSURE(lpszValueName != NULL);

	strValue.Empty();

	XMLNODE node= sp_currentNode->GetElement( XML_TOUTF8( lpszValueName ), !! m_bReadOnly );
	if( ! node.IsValid( ) )
		return FALSE;

	sp_currentNode->GetElementValue( XML_TOUTF8( lpszValueName ), strValue );
	return TRUE;
}

// .................................................................
BOOL XMLSettingsStore::Read(LPCTSTR lpszValueName, BYTE** ppData, UINT* pcbData)
{
	ENSURE(lpszValueName != NULL);
	ENSURE(ppData != NULL);
	ENSURE(pcbData != NULL);

	*ppData = NULL;
	*pcbData = 0;

	XMLNODE node= sp_currentNode->GetElement( XML_TOUTF8( lpszValueName ), true );
	if( ! node.IsValid( ) )
		return FALSE;

	sp_currentNode->GetElementValue( lpszValueName, ppData, (DWORD*)pcbData );
	XMLSTRACE( _T("parent: %S name: %s count: %d\n"), sp_currentNode->GetTag( ), lpszValueName, *pcbData );

	return TRUE;
}

// .................................................................
BOOL XMLSettingsStore::Read(LPCTSTR lpszValueName, CObject& obj)
{
	BOOL bSucess = FALSE;
	BYTE* pData = NULL;
	UINT uDataSize;

	if (!Read(lpszValueName, &pData, &uDataSize))
	{
		ENSURE(pData == NULL);
		return FALSE;
	}

	ENSURE(pData != NULL);

	try
	{
		CMemFile file(pData, uDataSize);
		CArchive ar(&file, CArchive::load);

		obj.Serialize(ar);
		bSucess = TRUE;
	}
	catch(CMemoryException* pEx)
	{
		pEx->Delete();
		TRACE(_T("Memory exception in XMLSettingsStore::Read()!\n"));
	}
	catch(CArchiveException* pEx)
	{
		pEx->Delete();
		TRACE(_T("CArchiveException exception in XMLSettingsStore::Read()!\n"));
	}

	//TODO !!!!!! BIG ONE this memory was allocated in the xml dll; Must destroy there!
	//and better yet, use a smart pointer that does so
	delete [] pData;
	return bSucess;
}

// .................................................................
BOOL XMLSettingsStore::Read(LPCTSTR lpszValueName, CObject*& pObj)
{
	BOOL bSucess = FALSE;
	BYTE* pData = NULL;
	UINT uDataSize;

	if (!Read(lpszValueName, &pData, &uDataSize))
	{
		ENSURE(pData == NULL);
		return FALSE;
	}

	ENSURE(pData != NULL);

	try
	{
		CMemFile file(pData, uDataSize);
		CArchive ar(&file, CArchive::load);
		ar >> pObj;

		bSucess = TRUE;
	}
	catch(CMemoryException* pEx)
	{
		pEx->Delete();
		TRACE(_T("Memory exception in XMLSettingsStore::Read()!\n"));
	}
	catch(CArchiveException* pEx)
	{
		pEx->Delete();
		TRACE(_T("CArchiveException exception in XMLSettingsStore::Read()!\n"));
	}

	//TODO !!!!!! BIG ONE this memory was allocated in the xml dll; Must destroy there!
	//and better yet, use a smart pointer that does so
	delete [] pData;
	return bSucess;
}

// .................................................................
BOOL XMLSettingsStore::Read( LPCTSTR lpszValueName, CRect& rect )
{
	assert( sp_currentNode->IsValid( ) );

	CRect rectT;
	XMLNODE child= sp_currentNode->GetElementValue( lpszValueName, rectT );
	if( ! child.IsValid( ) )
		return FALSE;

	rect= rectT;
	return TRUE;
}

// .................................................................
// .................................................................
// .................................................................
BOOL XMLSettingsStore::Write(LPCTSTR lpszValueName, int nValue)
{
	return Write(lpszValueName, (DWORD) nValue);
}

// .................................................................
BOOL XMLSettingsStore::Write(LPCTSTR lpszValueName, DWORD dwValue)
{
	if (m_bReadOnly)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	sp_currentNode->SetElementValue( lpszValueName, dwValue );
	return TRUE;
//	return m_reg.SetDWORDValue(lpszValueName, dwValue) == ERROR_SUCCESS;
}

// .................................................................
BOOL XMLSettingsStore::Write(LPCTSTR lpszValueName, LPCTSTR lpszData)
{
	if (m_bReadOnly)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	sp_currentNode->SetElementValue( XML_TOUTF8( lpszValueName ), lpszData );
	return TRUE;
//	return m_reg.SetStringValue(lpszValueName, lpszData) == ERROR_SUCCESS;
}

// .................................................................
BOOL XMLSettingsStore::Write(LPCTSTR lpszValueName, CObject& obj)
{
	if (m_bReadOnly)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	BOOL bRes = FALSE;
	try
	{
		CMemFile file;
		{
			CArchive ar(&file, CArchive::store);
			obj.Serialize(ar);
			ar.Flush();
		}
		DWORD dwDataSize = (DWORD) file.GetLength();
		LPBYTE lpbData = file.Detach();

		if (lpbData == NULL)
		{
			return FALSE;
		}
		bRes = Write(lpszValueName, lpbData, (UINT) dwDataSize);
		free(lpbData);
	}
	catch(CMemoryException* pEx)
	{
		pEx->Delete();
		TRACE(_T("Memory exception in XMLSettingsStore::Write()!\n"));
	}
	return bRes;
}

// .................................................................
BOOL XMLSettingsStore::Write(LPCTSTR lpszValueName, CObject* pObj)
{
	ASSERT_VALID(pObj);
	return Write(lpszValueName, *pObj);
}

// .................................................................
BOOL XMLSettingsStore::Write(LPCTSTR lpszValueName, const CRect& rect)
{
	assert( sp_currentNode->IsValid( ) );

	XMLNODE child= sp_currentNode->SetElementValue( lpszValueName, rect );
	if( ! child.IsValid( ) )
		return FALSE;
	return TRUE;
}

// .................................................................
BOOL XMLSettingsStore::Write(LPCTSTR lpszValueName, LPBYTE pData, UINT nBytes)
{
	if (m_bReadOnly)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	sp_currentNode->SetElementValue( lpszValueName, pData, nBytes );
	return TRUE;
//	return m_reg.SetBinaryValue(lpszValueName, pData, (ULONG) nBytes) == ERROR_SUCCESS;
}

// .................................................................
BOOL XMLSettingsStore::DeleteValue(LPCTSTR lpszValue)
{
	return m_reg.DeleteValue(lpszValue) == ERROR_SUCCESS;
}

// .................................................................
BOOL XMLSettingsStore::DeleteKey(LPCTSTR lpszPath, BOOL bAdmin)
{
	if (m_bReadOnly)
	{
		return FALSE;
	}

	m_reg.Close();
	m_reg.m_hKey = bAdmin ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

	return m_reg.RecurseDeleteKey(PreparePath(lpszPath)) == ERROR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
// HEWinApp members
BEGIN_MESSAGE_MAP(HEWinApp, CWinAppEx)
END_MESSAGE_MAP()

HEWinApp::HEWinApp( )
	:mruCount( 0 )
{
	m_bHiColorIcons = TRUE;
}

BOOL HEWinApp::InitInstance()
{
	//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);


	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	EnableTaskbarInteraction( FALSE );

	//Command Line Handleing
	// was: LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	/*/////////////////////////////////////////////////////////////////////

	Override to xml instead of regestry on app settings

	C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include
	afxsettingsstore.h
	C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\src\mfc
	afxsettingsstore.cpp

	XMLSettingsstore simply started as exact copies of the above.

	The settings store is not call upon until the mainframe is created
	But we will initialize it here to set the xml path/file

	this was helpful; The:  CSettingsStoreSP::SetRuntimeClass(RUNTIME_CLASS(CLocalSettingsStore));
	came from there. I voted 5 for the time saver
	http://www.codeproject.com/Articles/33089/Replacing-the-CSettingsStore-Class-in-MFC-to-Store

	/////////////////////////////////////////////////////////////////////*/

	SetRegistryKey( DUMMY_REGISTRY_PATH );

	CSettingsStoreSP::SetRuntimeClass( RUNTIME_CLASS( XMLSettingsStore ) );
	sp_xml_settings_store_global= SP_XML( new CXML );
	sp_currentNode_settings_store_global= SP_XMLNODE( new XMLNODE( *sp_xml_settings_store_global ) );
	strSysRegPath= GetRegSectionPath( );
	CSettingsStoreSP regSP;
	{
		XMLSettingsStore* pReg = dynamic_cast< XMLSettingsStore* >( &regSP.Create( FALSE, TRUE ) );
		pReg->SetRegPath( GetRegSectionPath( ) );
		ASSERT_VALID( pReg );
		//pReg->SetRegPath( DUMMY_REGISTRY_PATH );
		bfs::path pathUser;

		if( pathUser.empty( ) )//we will try to load here
		{
			TCHAR szPath[ MAX_PATH ];
			if( SUCCEEDED( SHGetFolderPath(	NULL, CSIDL_APPDATA, NULL, 0, szPath ) ) )
				pathUser= szPath;
			//else
			//	;//fail? Should never.....
		}
		//in case -u also had a file name
		if( bfs::is_directory( pathUser ) )
			pathUser/= USER_SETTINGS_FILENAME;

		if( ! pReg->OpenXML( pathUser ) )
			;//don't need it to continue... but warn?

		pReg->SetCurrentNode( _T("/Root/RecentFileList") );
		if( pReg->GetCurrentNode( )->IsValid( ) )
			MRU_ReadXML( *pReg->GetCurrentNode( ) );
		else
			;//error
	}

	InitContextMenuManager( );
	InitKeyboardManager( );
	InitTooltipManager( );
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	GetTooltipManager( )->SetTooltipParams( AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS( CMFCToolTipCtrl ), &ttParams );

	return CWinAppEx::InitInstance( );
}

int HEWinApp::ExitInstance( )
{
	//Save the MRU
	XMLNODE node= sp_xml_settings_store_global->GetNode( _X("/Root/RecentFileList") );
	MRU_WriteXML( node );

	//TODO !!!! figure out why leaving this entry creates memory leak on next close !!!!
	sp_xml_settings_store_global->GetNode( _X("//MFCToolBar-59392/Buttons") );
	sp_xml_settings_store_global->DeleteCurrentNode( );
	//.......

	//Write user XML
	sp_xml_settings_store_global->Close( );

	AfxOleTerm( FALSE );

	//inhibit write of MRU to registry
	m_pCmdInfo= new CCommandLineInfo;
	m_pCmdInfo->m_nShellCommand= CCommandLineInfo::AppUnregister;

	return CWinAppEx::ExitInstance( );
}

AFX_STATIC_DATA const TCHAR _afxFileSection[] = _T("Recent File List");
AFX_STATIC_DATA const TCHAR _afxFileEntry[] = _T("File%d");
// .......................................................................
void HEWinApp::MRU_ReadXML( XMLNODE& node )
{
	ASSERT( ! m_pRecentFileList );
	node.GetElementValue( _X("maxMRU"), mruCount, 4 );
	m_pRecentFileList = new CRecentFileList( 0, _afxFileSection, _afxFileEntry, mruCount );

	for( size_t i= 0, j= 0; j < (size_t)m_pRecentFileList->GetSize( ) && node.SetCurrentChild( i ); ++i )
		if( node.CompareChildTag( _X("projMRU") ) )
			m_pRecentFileList->m_arrNames[ j++ ]= node.GetCurrentChild( ).GetValue( );
}

// .......................................................................
void HEWinApp::MRU_WriteXML( XMLNODE& node )
{
	ASSERT( m_pRecentFileList );
	node.DeleteChildren( );
	node.SetElementValue( _X("maxMRU"), mruCount );
	for( int iMRU= 0; iMRU < m_pRecentFileList->m_nSize; iMRU++ )
	{
		if( ! m_pRecentFileList->m_arrNames[ iMRU ].IsEmpty( ) )
		{
			XMLNODE child= node.AddChild( _X("projMRU") );
			child.SetValue( m_pRecentFileList->m_arrNames[ iMRU ] );
		}
	}
}

// .......................................................................
UINT HEWinApp::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault)
{
	CString strT( _T("//") );
	strT+= lpszSection;
	strT.Replace( _T('\\'), _T('/') );
	CSettingsStoreSP regSP;
	XMLSettingsStore* pReg = dynamic_cast< XMLSettingsStore* >( &regSP.Create( FALSE, TRUE ) );
	XMLNODE node= pReg->sp_xml->GetNode( strT );
	if( ! node.IsValid( ) )
		return nDefault;
	long val;
	node.GetElementValue( lpszEntry, val, nDefault );
	return val;
}

// .......................................................................
BOOL HEWinApp::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
	CString strT( _T("//") );
	strT+= lpszSection;
	strT.Replace( _T('\\'), _T('/') );
	CSettingsStoreSP regSP;
	XMLSettingsStore* pReg = dynamic_cast< XMLSettingsStore* >( &regSP.Create( FALSE, FALSE ) );
	XMLNODE node= pReg->sp_xml->GetNode( strT, true );
	if( ! node.IsValid( ) )
		return FALSE;

	node.SetElementValue( lpszEntry, (long)nValue );
	return TRUE;
}

// .......................................................................

// .......................................................................
void HEWinApp::PreLoadState( )
{
}

// .......................................................................
void HEWinApp::LoadCustomState()
{
}

// .......................................................................
void HEWinApp::SaveCustomState()
{
}

// .......................................................................
void HEWinApp::XMLSaveState( CFrameImpl* pFrame )
{
}

//////////////////////////////////////////////////////////////////////////////
// XMLSettingsStoreSP - Helper class that manages "safe" XMLSettingsStore pointer
/*
CRuntimeClass* XMLSettingsStoreSP::m_pRTIDefault = NULL;

BOOL __stdcall XMLSettingsStoreSP::SetRuntimeClass(CRuntimeClass* pRTI)
{
	if (pRTI != NULL &&
		!pRTI->IsDerivedFrom(RUNTIME_CLASS(XMLSettingsStore)))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_pRTIDefault = pRTI;
	return TRUE;
}

XMLSettingsStore& XMLSettingsStoreSP::Create(BOOL bAdmin, BOOL bReadOnly)
{
	if (m_pRegistry != NULL)
	{
		ASSERT(FALSE);
		ASSERT_VALID(m_pRegistry);
		return *m_pRegistry;
	}

	if (m_pRTIDefault == NULL)
	{
		m_pRegistry = new XMLSettingsStore;
	}
	else
	{
		ASSERT(m_pRTIDefault->IsDerivedFrom(RUNTIME_CLASS(XMLSettingsStore)));
		m_pRegistry = DYNAMIC_DOWNCAST(XMLSettingsStore, m_pRTIDefault->CreateObject());
	}

	ASSERT_VALID(m_pRegistry);

	m_pRegistry->m_bReadOnly = bReadOnly;
	m_pRegistry->m_bAdmin = bAdmin;
	m_pRegistry->m_reg.m_hKey = bAdmin ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
	m_pRegistry->m_dwUserData = m_dwUserData;

	return *m_pRegistry;
}


*/

