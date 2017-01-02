
#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "Gears.h"
#endif

#include "Geometry.h"
#include "objects.h"
//#include "cnc.h"
#include "shared.h"
#include "Drawing.h"
#include "CADDoc.h"
#include "dxf.h"

// ..........................................................................
IMPLEMENT_DYNCREATE( CADDoc, CDocument )
BEGIN_MESSAGE_MAP( CADDoc, CDocument )
	ON_COMMAND( ID_FILE_TEST, &CADDoc::OnFileTest )
	ON_COMMAND( ID_FILE_G_CODE, &CADDoc::OnFileGCode )
	//	ON_NOTIFY( ID_GEAR_PARAM_CHANGED, 1, &CADDoc::OnGearParamChanged )
END_MESSAGE_MAP( )

// ............................................................................
//void CADDoc::OnGearParamChanged( NMHDR*, LRESULT* )
//{
//}

// ..........................................................................
BOOL CADDoc::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	//if ( HIWORD(nCode) == WM_NOTIFY )
	//{
	//	LPNMHDR pHdr= ((AFX_NOTIFY*)pExtra)->pNMHDR;
	//	switch( LOWORD( nCode ) )
	//	{
	//	case ID_GEAR_PARAM_CHANGED:
	//		auto pg= ( pgear_params_t )((PNMHDROBJECT)pHdr)->pObject;
	//		test( );
	//		UpdateAllViews( nullptr, ID_GEAR_PARAM_CHANGED );
	//		NMHDR* pnm= ((AFX_NOTIFY*)pExtra)->pNMHDR;
	//		TRACE( "doc: %x - %x - %x low: %x\n", nID, nCode, pnm->code, LOWORD( nCode ) );
	//	}
	//}
	return CDocument::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

// ..........................................................................
CADDoc::CADDoc( )
	:drawobj( layers )
{
}

// ..........................................................................
CADDoc::~CADDoc( )
{
}

// ..........................................................................
void CADDoc::OnFileGCode( )
{
	//testing g-code............
	//sp_gcitem_t test( boost::make_shared< gc0 >( ) );
	//gc.push_back( test );
	//gc.push_back( sp_gcitem_t( new gc0 ) );
}

template< typename Type >
inline SP_BaseItem  make_sp( Type& obj ) { return boost::make_shared< Type >( obj ); }
// ..........................................................................
void CADDoc::test( )
{
	ObjectSet set;
	SP_PointItem pt( PointItem( bg_point( 1, 1 ) ).get_SP( ) );
	set.push_back( pt );
	SP_LineItem lt( LineItem( bg_point( -1, -.5 ), bg_point( 0, 0 ) ).get_SP( ) );
	set.push_back( lt );
	bg_box box= get_rect_hull( set.get_set( ) );
	set.push_back( RectItem( box ).get_SP( ) );
	RectItem ri( box );
	GEO_NUM bx= ri.width( ) / 4;
	ri.get_min_point( ).x-= bx;
	ri.get_max_point( ).x+= bx;
	GEO_NUM by= ri.height( ) / 4;
	ri.get_min_point( ).y-= by;
	ri.get_max_point( ).y+= by;
	draw_extents= ri.get_bg_box( );
	// the view will get the 'draw_extents'
	UpdateAllViews( nullptr, ID_UPDATE_VIEW_TOFIT );
}

// ..........................................................................
BOOL CADDoc::OnNewDocument( )
{
	if( ! CDocument::OnNewDocument( ) )
		return FALSE;

	test( );

	return TRUE;
}

// ..........................................................................
BOOL CADDoc::OnSaveDocument( LPCTSTR lpszPathName )
{
	return CDocument::OnSaveDocument( lpszPathName );
}

// ..........................................................................
bool StartDXF_write( DL_Dxf* dxf, DL_WriterA* dw );
bool EndDXF_write( DL_Dxf* dxf, DL_WriterA* dw );

// ..........................................................................
LRESULT CADDoc::OnLayerEnable(WPARAM, LPARAM )
{
	return 0;
}

void CADDoc::OnFileTest( )
{
	DXF_WriteFile( bfs::path( "test2.dxf"), drawobj );
	return;

}

// ..........................................................................
void CADDoc::Serialize(CArchive& ar)
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
void CADDoc::AssertValid() const
{
	CDocument::AssertValid();
}

// ..........................................................................
void CADDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

#ifdef SHARED_HANDLERS

// ..........................................................................
// Support for thumbnails
void CADDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CADDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

// ..........................................................................
void CADDoc::SetSearchContent(const CString& value)
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

