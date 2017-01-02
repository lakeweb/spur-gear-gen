// devView.cpp : implementation of the CdevView class
//

#include "stdafx.h"
#include "dev.h"

#include "devDoc.h"
#include ".\devview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <Wbemidl.h>

#ifdef _DEBUG
CString GetErrorCode( HRESULT m_hres )
{
	IWbemStatusCodeText * pStatus = NULL;
	SCODE sc = CoCreateInstance(CLSID_WbemStatusCodeText, 0, CLSCTX_INPROC_SERVER, IID_IWbemStatusCodeText, (LPVOID *) &pStatus);

	CString str;
	if(sc == S_OK)
	{
		BSTR bstr = 0;
		sc = pStatus->GetErrorCodeText(m_hres, 0, 0, &bstr);
		if(sc == S_OK)
		{
			str+= bstr;
			SysFreeString(bstr);
			bstr = 0;
		}
		pStatus->Release();
	}
	else
		str= _T("CoCreateInstance failed");

	return str;
}
#else
CString GetErrorCode( HRESULT m_hres )
{
	return CString( "release version" );
}
#endif

//To test 'ReadString(...)
AFX_STATIC_DATA LPCTSTR STR_UPDATE_NEW_XML=
_T("<Root>\r\n\
   <Added>1/1  /2000  </Added>\r\n\
</Root>\r\n");

void CdevView::OnInitialUpdate( )
{

//create a console for standard output
	AllocConsole( );
	SetConsoleTitle( _T("stdout") );
	HANDLE con;
    con= CreateFile( _T("CON"), GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL, 0, 0, 0 );
	FILE *stream;
    freopen_s( &stream, "CON", "wt", stdout );
    SetConsoleTextAttribute( con, FOREGROUND_GREEN );
	//
	CEditView::OnInitialUpdate( );

// Testing done here .....................................
	//WriteOutput( "testing 'test.xml'\r\n\r\n" );

/*
	if( xml.Open( ".\\test.xml", "root", CXML::modeRead ) )
	{
		CString tstr;
		//xml.DeleteCurrentNode( );

		XMLNODESET nodeset= xml.GetNodeSet( "//*test" );" */
/*		XMLNODESET* p= &nodeset;
		if( xml.GetLastXPathError( ) != ERROR_SUCCESS )
		{
			WriteOutput( "%s\r\n", GetErrorCode( xml.GetLastXPathError( ) ) );
			GetDocument( )->SetModifiedFlag( false );
			return;
		}
		for( unsigned int i= 0; nodeset.SetCurrentNode( i ); ++i )
			WriteOutput( "tag: %s uid: %s\r\n", nodeset.GetCurrentNode( ).GetTag( ), nodeset.GetCurrentAttribute( "uid" ) );

		WriteOutput( "\r\n" );

//		nodeset= xml.GetNodeSet( "//*subStuff/testing[@type=1]" );"*/
/*		if( xml.GetLastXPathError( ) != ERROR_SUCCESS )
		{
			WriteOutput( "%s\r\n", GetErrorCode( xml.GetLastXPathError( ) ) );
			GetDocument( )->SetModifiedFlag( false );
			return;
		}
		for( unsigned int i= 0; nodeset.SetCurrentNode( i ); ++i )
			WriteOutput( "tag: %s value: %s\r\n", nodeset.GetCurrentNode( ).GetTag( ), nodeset.GetCurrentNode( ).GetValue( ) );

		//WriteOutput( "Result of '%s' count: %d\r\n   tag: %s value: %s\r\n   error: %s\r\n",
		//	tstr, xml.GetLastXPathCount( ), node.GetTag( ), node.GetValue( ), GetErrorCode( xml.GetLastXPathError( ) ) );
		//WriteOutput( "\r\n" );

		tstr= ( _T("/root/General") );
		//tstr= ( _T("/root/FirstNode/SecondNode") ); //Will create nodes
		XMLNODE node= xml.GetNode( tstr );
		//WriteOutput( "Result of '%s' count: %d\r\n   tag: %s value: %s\r\n   error: %s\r\n",
		//	tstr, xml.GetLastXPathCount( ), node.GetTag( ), node.GetValue( ), GetErrorCode( xml.GetLastXPathError( ) ) );
		//WriteOutput( "\r\n" );

		//tstr= ( _T("something/some_sub") );
		////tstr= ( _T("something") );
		//node= xml.GetNode( tstr );
		//WriteOutput( "Result of '%s' count: %d\r\n   tag: %s value: %s\r\n   error: %s\r\n",
		//	tstr, xml.GetLastXPathCount( ), node.GetTag( ), node.GetValue( ), GetErrorCode( xml.GetLastXPathError( ) ) );
		//WriteOutput( "\r\n" );

		//xml.SetCurrentNode( _T("/root") );
		////Current Node is 'something'
		//tstr= ( _T("General[@type='that']") );
		//node= xml.GetNode( tstr );
		//WriteOutput( "Result of '%s' count: %d\r\n   tag: %s value: %s\r\n   error: %s\r\n",
		//	tstr, xml.GetLastXPathCount( ), node.GetTag( ), node.GetValue( ), GetErrorCode( xml.GetLastXPathError( ) ) );
		//WriteOutput( "\r\n" );

		//tstr= ( _T("//*") ); //Will debug warn.
		//node= xml.GetNode( tstr );
		//WriteOutput( "Result of '%s' count: %d\r\n   tag: %s value: %s\r\n   error: %s\r\n",
		//	tstr, xml.GetLastXPathCount( ), node.GetTag( ), node.GetValue( ), GetErrorCode( xml.GetLastXPathError( ) ) );
		//WriteOutput( "\r\n" );

		//tstr= ( _T("/*") ); //Will debug warn.
		//node= xml.GetNode( tstr );
		//WriteOutput( "Result of '%s' count: %d\r\n   tag: %s value: %s\r\n   error: %s\r\n",
		//	tstr, xml.GetLastXPathCount( ), node.GetTag( ), node.GetValue( ), GetErrorCode( xml.GetLastXPathError( ) ) );
		//WriteOutput( "\r\n" );

		//tstr= ( _T("//[report='testReport1']") ); //Will debug warn.
		//tstr= ( _T("/root/reports/report[text()='testReport3']") ); //Will debug warn.
		//node= xml.GetNode( tstr );
		//WriteOutput( "Result of '%s' count: %d\r\n   tag: %s value: %s\r\n   error: %s\r\n",
		//	tstr, xml.GetLastXPathCount( ), node.GetTag( ), node.GetValue( ), GetErrorCode( xml.GetLastXPathError( ) ) );
		//WriteOutput( "\r\n" );

		//////xml.SetCurrentNode( );
		//tstr= ( _T("/root/reports/*") );
		//XMLNODESET tns= xml.GetNodeSet( tstr );
		//WriteOutput( "Set Result of '%s'\r\n", tstr );
		//for( int i= 0; tns.SetCurrentNode( i ); ++i )
		//	WriteOutput( "   %2d tag: %s value: %s\r\n",
		//		i, tns.GetCurrentNode( ).GetTag( ), tns.GetCurrentNode( ).GetValue( ) );
		//WriteOutput( "error: %s\r\n\r\n", GetErrorCode( xml.GetLastXPathError( ) ) );

		//tstr= _T("//report");
		//tns= xml.GetNodeSet( tstr );
		//WriteOutput( "Set Result of '%s'\r\n", tstr );
		//for( int i= 0; tns.SetCurrentNode( i ); ++i )
		//	WriteOutput( "   %2d tag: %s value: %s\r\n",
		//		i, tns.GetCurrentNode( ).GetTag( ), tns.GetCurrentNode( ).GetValue( ) );
		//WriteOutput( "error: %s\r\n\r\n", GetErrorCode( xml.GetLastXPathError( ) ) );

		//tstr= _T("//something");
		//tns+= xml.GetNode( tstr );
		//WriteOutput( "Set++ Result of '%s'\r\n", tstr );
		//for( int i= 0; tns.SetCurrentNode( i ); ++i )
		//	WriteOutput( "   %2d tag: %s value: %s\r\n",
		//		i, tns.GetCurrentNode( ).GetTag( ), tns.GetCurrentNode( ).GetValue( ) );
		//WriteOutput( "error: %s\r\n\r\n", GetErrorCode( xml.GetLastXPathError( ) ) );

		//XMLNODESET tns2= tns;

		//WriteOutput( "Set= Result of '%s'\r\n", tstr );
		//for( int i= 0; tns2.SetCurrentNode( i ); ++i )
		//	WriteOutput( "   %2d tag: %s value: %s\r\n",
		//		i, tns2.GetCurrentNode( ).GetTag( ), tns2.GetCurrentNode( ).GetValue( ) );
		//WriteOutput( "error: %s\r\n\r\n", GetErrorCode( xml.GetLastXPathError( ) ) );

		//XMLNODESET can_not( &xml );
	}
	else
		WriteOutput( "could not open: 'test.xml'/r/n" );

	//xml.Clear( );

	//if( xml.Open( "testNew.xml", "root", CXML::modeReadWrite | CXML::modeCreate ) )
	//{
	//	WriteOutput( "testing 'testNew.xml'\r\n" );
	//	CString tstr( _T("TestNewNode/testnode") );
	//	XMLNODE tnode= xml.GetNode( tstr );
	//	WriteOutput( "Result of '%s' count: %d\r\n   tag: %s value: %s\r\n   error: %s\r\n",
	//		tstr, xml.GetLastXPathCount( ), tnode.GetTag( ), tnode.GetValue( ), GetErrorCode( xml.GetLastXPathError( ) ) );
	//	WriteOutput( "\r\n" );
	//}
	//else
	//	WriteOutput( "could not create: 'test.xml'/r/n" );
	//xml.Close( );
// End of testing .........................................
*/
	CString strT;
	//xml.Open( "C:/cpp/HE/xml/dev/test2.xml", CXML::modeRead );
	//xml.WriteStringFmt( strT );
//	xml.ReadString( STR_UPDATE_NEW_XML );

	xml.Open( _T("C:/cpp/HE/xml/dev/test3.xml"), CXML::modeProgFile );
	CString strTest;
	xml.GetElementValue( _X("footer"), strTest );
	xml.WriteStringFmt( strT );
	//xml.Clear( );
	//xml.ReadString( strT );
	xml.Close( );
//	WriteOutput( strTest );

	//So you don't have to answer..
	GetDocument( )->SetModifiedFlag( false );
	return;


	//DeleteFile( "testingWrite.xml" );
	//CXML xmlTest;
	//xmlTest.Open( "testingWrite.xml", _T("Root"), CXML::modeWrite | CXML::modeCreate );
	//xmlTest.SetElementValue( "test", "this is a test" );
	//xmlTest.SetElementValue( "test2", "this is a test2" );
	//LOGFONTA lTest;
	//xmlTest.GetElementValue( "LOGFONT", lTest );
	////xmlTest.SetElementValue( "LOGFONT", lTest );
	//COLORREF tc= RGB( 1, 2, 3 );
	//xmlTest.GetElementValue( _T("color"), tc, RGB( 240, 240, 240 ) );

	//XMLNODE testNode= xmlTest.GetCurrentNode( );
	//CXML xmlCopy;
	//xmlCopy.Open( "testingInsert.xml", _T("Root"), CXML::modeWrite | CXML::modeCreate );
	//xmlCopy= xmlTest;
	//xmlCopy.GetCurrentNode( ).InsertChild( testNode );
	//xmlCopy.Close( );
	//xmlTest.Close( );
}

void CdevView::WriteOutput( LPCSTR pszOutput, ... )
{
	//CString strInput;
	//va_list argList;

	//va_start( argList, pszOutput );
	//strInput.FormatV( pszOutput, argList );
	//va_end( argList );

	//int nSize = GetWindowTextLength( );
	//GetEditCtrl( ).SetSel( nSize, nSize );
	//GetEditCtrl( ).ReplaceSel( strInput );
}

IMPLEMENT_DYNCREATE(CdevView, CEditView)

BEGIN_MESSAGE_MAP(CdevView, CEditView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CdevView construction/destruction

CdevView::CdevView()
{
	// TODO: add construction code here

}

CdevView::~CdevView()
{
}

BOOL CdevView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}


// CdevView printing

BOOL CdevView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CdevView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CdevView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}


// CdevView diagnostics

#ifdef _DEBUG
void CdevView::AssertValid() const
{
	CEditView::AssertValid();
}

void CdevView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CdevDoc* CdevView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdevDoc)));
	return (CdevDoc*)m_pDocument;
}
#endif //_DEBUG

