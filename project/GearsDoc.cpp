
// GearsDoc.cpp : implementation of the CGearsDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.

#include "Geometry.h"
#include "objects.h"
#include "shared.h"
#include "cnc.h"
#include "Drawing.h"
#include "dxf.h"
#include "gear_gen.h"

#ifndef SHARED_HANDLERS
#include "Gears.h"
#endif

#include "GearsDoc.h"

//#include <propkey.h>

#include <boost/math/constants/constants.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ..........................................................................
IMPLEMENT_DYNCREATE( CGearsDoc, CDocument )
BEGIN_MESSAGE_MAP( CGearsDoc, CDocument )
	ON_COMMAND( ID_FILE_TEST, &CGearsDoc::OnFileTest )
	ON_COMMAND( ID_FILE_G_CODE, &CGearsDoc::OnFileGCode )
END_MESSAGE_MAP( )

// ..........................................................................
CGearsDoc::CGearsDoc( )
	:drawobj( layers )
{
}

// ..........................................................................
CGearsDoc::~CGearsDoc( )
{
}

// ..........................................................................
void CGearsDoc::OnFileGCode( )
{
	//testing g-code............
	sp_gcitem_t test( boost::make_shared< gc0 >( ) );
	gc.push_back( test );
	gc.push_back( sp_gcitem_t( new gc0 ) );
}

// ..........................................................................
void CGearsDoc::test( )
{
	//as if we have loaded/spec'd a gear.......

	ObjectSet visuals( 0 );

	//small hole at center
	{
		SP_ArcItem outside_cut( new ArcItem( bg_point( .2, .2 ), bg_point( .2, .2 ), bg_point( 0, 0 ) ) );
		outside_cut->SetMill( );
		visuals.push_back( outside_cut );
	}

#if 0
	visuals.set_layer( 0 );
	//spec the gear
	gear_params_t params(
		20,					//tooth count
		14.50 * PI / 180,	//presure angle in radians
		5					//diametrical pitch
	);
#else
	gear_params_t params(
		22,		//tooth count
		20 * PI / 180,	//presure angle in radians
		17		//diametrical pitch
	);
#endif

	//create a tooth
	auto tooth= gear_gen_one( params );

	//generate gear from tooth
	auto gear= gear_generate( tooth, params );
	gear.set_id( 1 );
	drawobj.push_back( gear );

	//copy gear with offset
	ObjectSet gear2;
	gear2.set_offset( bg_point( params.pd, 0 ) );
	for( const auto& item : gear )
	{
		item->SetColor( RGB( 0, 222, 222 ) );
		auto hold= transform_object( item, bg_point( params.pd, 0 ) );
		hold= rotate_object( params.of, hold, bg_point( params.pd, 0 ) );
		hold->SetColor( RGB( 255, 0, 0 ) );
		gear2.push_back( hold );
	}
	visuals.insert( gear2.begin( ), gear2.end( ) );
	gear2.set_id( 2 );
	drawobj.push_back( gear2 );

	// working ..........................................
	layers.insert( layer_t( 0, L"Zero Layer" ) );
	layers.insert( layer_t( 1, L"Milling" ) );
}

// ..........................................................................
BOOL CGearsDoc::OnNewDocument( )
{
	if( ! CDocument::OnNewDocument( ) )
		return FALSE;

	test( );

	return TRUE;
}

// ..........................................................................
BOOL CGearsDoc::OnSaveDocument( LPCTSTR lpszPathName )
{
	return CDocument::OnSaveDocument( lpszPathName );
}

// ..........................................................................
bool StartDXF_write( DL_Dxf* dxf, DL_WriterA* dw );
bool EndDXF_write( DL_Dxf* dxf, DL_WriterA* dw );

// ..........................................................................
LRESULT CGearsDoc::OnLayerEnable(WPARAM, LPARAM )
{
	return 0;
}

void CGearsDoc::OnFileTest( )
{
	DXF_WriteFile( bfs::path( "test2.dxf"), drawobj );
	return;

}

// ..........................................................................
void CGearsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

// ..........................................................................
#ifdef _DEBUG
void CGearsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

// ..........................................................................
void CGearsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// ..........................................................................
BOOL CGearsDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	//TRACE( "doc: %x\n", nID );

	return CDocument::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

#ifdef SHARED_HANDLERS

// ..........................................................................
// Support for thumbnails
void CGearsDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ..........................................................................
// Support for Search Handlers
void CGearsDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

// ..........................................................................
void CGearsDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

