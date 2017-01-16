
// Gears.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "app_share.h"
#include "HEWinApp.h"

#include <ctrlext.h>
#include "Scroller.h"

#include "Geometry.h"
#include "objects.h"
#include "shared.h"
#include "cnc.h"
#include "Drawing.h"

#include "app_share.h"
#include "Gears.h"
#include "MainFrm.h"
#include "CADDoc.h"
#include "CADView.h"
#include "CADFrame.h"

#include "GearsDoc.h"
#include "GearsView.h"

#include "About.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void Create_STD_OUT_Console( );

CGearsApp theApp;

BEGIN_MESSAGE_MAP( CGearsApp, HEWinApp )
	ON_COMMAND(ID_APP_ABOUT, &CGearsApp::OnAppAbout)
	// Standard file based document commands

	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_NEW, &CGearsApp::OnFileNew)
END_MESSAGE_MAP()

// ............................................................................
CGearsApp::CGearsApp( )
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Gears.AppID.NoVersion"));
}

BOOL CGearsApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	pathUser= get_user_folder( );
	HEWinApp::InitInstance( );

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	AfxInitRichEdit2();

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CADDoc),
		RUNTIME_CLASS(CADFrame),
		RUNTIME_CLASS(CADView)
		);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	pDocTemplate = new CMultiDocTemplate(
		IDR_GEARSTYPE,
		RUNTIME_CLASS(CGearsDoc),
		RUNTIME_CLASS(CADFrame),
		RUNTIME_CLASS(CGearsView)
	);
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

#ifdef _DEBUG
	Create_STD_OUT_Console( );
#endif
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	//testing....
	//OpenDocumentFile( _T("test.hcd") );
	//OnFileNew( );

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

// ............................................................................
int CGearsApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return HEWinApp::ExitInstance();
}

// ............................................................................
void CGearsApp::OnFileNew( )
{
	POSITION pos= m_pDocManager->GetFirstDocTemplatePosition( );
	m_pDocManager->GetNextDocTemplate( pos );
	CDocTemplate* pTemplate= m_pDocManager->GetNextDocTemplate( pos );
	pTemplate->OpenDocumentFile(NULL);

	//CDocTemplate* pTemplate = (CDocTemplate*)m_pDocManager->m_templateList.GetHead();
//	CWinAppEx::OnFileNew( );
}

// ............................................................................
void CGearsApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

// ............................................................................
void CGearsApp::LoadCustomState()
{
}

// ............................................................................
void CGearsApp::SaveCustomState()
{
}

// .......................................................................
void CGearsApp::OnAppAbout( )
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal( );
}

