
// CADView.cpp : implementation of the CADView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.

#include "MemDC.h"
#include "shared.h"
#include "Geometry.h"
#include "cnc.h"
#include "objects.h"
#include "Drawing.h"
#include "dxf.h"

#include "Scroller.h"

#ifndef SHARED_HANDLERS
#include "Gears.h"
#endif

#include "CADDoc.h"
#include "CADView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define SHOW_PARAMS

// ..................................................................
IMPLEMENT_DYNCREATE( CADView, CView )
BEGIN_MESSAGE_MAP( CADView, CView )
	// Standard printing commands
	ON_COMMAND( ID_FILE_PRINT, &CView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_DIRECT, &CView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, &CADView::OnFilePrintPreview )
	ON_MESSAGE( VIEW_INFO_NOTIFY, &CADView::OnFrmNotify )
	ON_WM_CONTEXTMENU( )
	ON_WM_RBUTTONUP( )
	ON_WM_MOUSEMOVE( )
	ON_WM_MOUSEWHEEL( )
	ON_WM_VSCROLL( )
	ON_WM_HSCROLL( )
	ON_WM_SIZE( )
END_MESSAGE_MAP( )

// ..................................................................
// ..................................................................
void CADView::OnUpdateToolsAnimate( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( );
	pCmdUI->SetCheck( bAnimate );
}

// ..................................................................
CADView::CADView( )
	:winScale( 140 )
	,offsetx( -1500 )
	,offsety( -1500 )
	,rectS( -1, -1, 4, 4 )
	,d_ext( rectS, offsetx, offsety, winScale )
	,bStatusMouseChanged( true )
	,bAnimate( false )
{
}

// ..................................................................
CADView::~CADView( )
{
}

LRESULT CADView::OnFrmNotify( WPARAM wp, LPARAM lp )
{
	typedef dock_notify_t dn;
	dock_notify_t& note= *reinterpret_cast< dock_notify_t* >( wp );

	switch( note.the_docker )
	{
	case dn::layer:
	{
		layer_set_t& ls= GetDocument( )->layers;
		auto it= ls.find( layer_t( note.info, NULL ) );
		if( it != ls.end( ) )
		{
			layer_t t= *it;
			ls.erase( it );
			t.enabled= ! t.enabled;
			ls.insert( t );
		}
		Invalidate( );
		break;
	}
	}
	return FALSE;
}

// ............................................................................
//When frame needs status info
CString CADView::GetStatusInfo( )
{
	if( bStatusMouseChanged )
	{
		strStatus.Format( _T(" x: %.4f y: %.4f")
			//,ptLastMouse.x
			//,ptLastMouse.y
			,(double)( ptLastMouse.x - offsetx ) / winScale
			,(double)( ptLastMouse.y - offsety ) / winScale
		);
		bStatusMouseChanged= false;
	}
	return strStatus;
}

// ..................................................................
BOOL CADView::PreCreateWindow( CREATESTRUCT& cs )
{
#ifdef USING_CONSOLE
	Create_STD_OUT_Console( );
#endif
	cs.style|= WS_HSCROLL | WS_VSCROLL;
	scroller.AttachWnd( this );

	return CView::PreCreateWindow( cs );
}

// ..................................................................
void CADView::OnDraw( CDC* poDC )
{
	CADDoc* pDoc = GetDocument( );
	ASSERT_VALID( pDoc );
	if( ! pDoc )
		return;

	offsetx= -scroller.GetScrollPos( ).cx - d_ext.rectClient.left;
	offsety= -scroller.GetScrollPos( ).cy - d_ext.rectClient.top;
	CNewMemDC pDC( poDC );

	CRect crect;
	GetClientRect( crect );

	//the drawer....................
	DrawArea drawer( pDC, d_ext );
	const DrawingObects& objs= pDoc->GetDrawingObjects( );
	drawer.DrawObjects( objs );
	//..............................
	//TRACE( "draw\n" );

	//	drawer.DrawGrid( );

#ifdef _DEBUG
	std::stringstream os;
	os << "view size: " << scroller.GetDisplaySize( ).cx << " " << scroller.GetDisplaySize( ).cy << std::endl;
	os << "page size: " << scroller.GetPageSize( ).cx << " " << scroller.GetPageSize( ).cy << std::endl;
	os << "winscale: " << winScale << std::endl;
	info_notify_t notice( info_notify_t::output );
	notice.basic_info= os.str( );
	notice.bClear= true;
	::SendMessage( *AfxGetMainWnd( ), APP_INFO_NOTIFY, (WPARAM)&notice, 0 );
#endif //_DEBUG
}

// ..................................................................
void CADView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

// ..................................................................
void CADView::OnContextMenu( CWnd* /* pWnd */, CPoint point )
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager( )->ShowPopupMenu( IDR_POPUP_EDIT, point.x, point.y, this, TRUE );
#endif
}

// ............................................................................
void CADView::OnMouseMove( UINT nFlags, CPoint point )
{
	if( ptLastMouse != point )
		bStatusMouseChanged= true;
	else
		return;

	ptLastMouse= point;
	CView::OnMouseMove( nFlags, point );
}

// ..................................................................
BOOL CADView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	double prevWinScale= winScale;

	if( zDelta < 0 )
		winScale-= winScale / 4;
	else
		winScale+= winScale / 4;

	TRACE( "winScale: %f\n", winScale );
	CSize ratio( (long)( prevWinScale ), (long)( winScale ) );
	scroller.OnMouseWheel( ratio, pt );

	Invalidate( );
	return FALSE;
	//	return CView::OnMouseWheel( nFlags, zDelta, pt );
}

// ............................................................................
void CADView::OnUpdate( CView* /*pSender*/, LPARAM hint, CObject* pHint )
{
	switch( hint )
	{
	case ID_GEAR_PARAM_CHANGED:
	{
		assert( false );
		auto container= GetDocument( )->GetDrawingObjects( ).get_set( );
		//Invalidate( );
		break;
	}
	case ID_UPDATE_VIEW_TOFIT:
	{
		bg_box& box= GetDocument( )->draw_extents;
		rectS.SetRect(
			(int)( box.min_corner( ).get_x( ) * winScale ),
			(int)( box.min_corner( ).get_y( ) * winScale ),
			(int)( box.max_corner( ).get_x( ) * winScale ),
			(int)( box.max_corner( ).get_y( ) * winScale ) );
		double w= RectItem( box ).width( ) / d_ext.rectClient.Width( );
		double h= RectItem( box ).height( ) / d_ext.rectClient.Height( );
		if( d_ext.rectClient.Width( ) )
			;//  still working on it...........
	}
	}//switch( .. )

	 //drawing objects extents have changed
	SetDisplaySize( );

	//info_notify_t notice( info_notify_t::output );
	//notice.basic_info= GetDocument( )->GetInfoStr( );
	//::SendMessage( *GetParentFrame( ), APP_INFO_NOTIFY, (WPARAM)&notice, 0 );

	//notice.the_call= info_notify_t::layer;
	//const layer_set_t& layers= GetDocument( )->GetLayers( );
	//::SendMessage( *GetParentFrame( ), APP_INFO_NOTIFY, (WPARAM)&notice, (LPARAM)&layers );
}

// ..................................................................
//printing
// ..................................................................
void CADView::OnFilePrintPreview( )
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview( this );
#endif
}

// ..................................................................
BOOL CADView::OnPreparePrinting( CPrintInfo* pInfo )
{
	return DoPreparePrinting( pInfo );
}

void CADView::OnBeginPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
}

// ..................................................................
void CADView::OnEndPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
}

void CADView::NotifyFrame( )
{
	//::SendMessage( *GetParent( ), ID_POST_CADVIEW_FRAME, (WPARAM)&d_ext, 0 );
}

// ............................................................................
void CADView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	TRACE( "VScroll pos: %d  code: %d\n", nPos, nSBCode );
	scroller.OnVScroll( nSBCode, nPos, pScrollBar );
	CView::OnVScroll( nSBCode, nPos, pScrollBar );
}

// ............................................................................
void CADView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	TRACE( "HScroll pos: %d  code: %d\n", nPos, nSBCode );
	scroller.OnHScroll( nSBCode, nPos, pScrollBar );
	CView::OnHScroll( nSBCode, nPos, pScrollBar );
	d_ext.bHorz= true;
//	NotifyFrame( );
}

// ............................................................................
void CADView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );
	scroller.OnSize( nType, cx, cy );
	d_ext.set_all( );
//	NotifyFrame( );
}

// ............................................................................
void CADView::SetDisplaySize( )
{
	//Big TODO !!!!!!!!
	offsetx= -rectS.left;
	offsety= -rectS.top;
	scroller.SetDisplaySize( rectS.Width( ), rectS.Height( ) );
	Invalidate( );
}

// ..................................................................
#ifdef _DEBUG
void CADView::AssertValid( ) const
{
	CView::AssertValid();
}

// ..................................................................
void CADView::Dump( CDumpContext& dc ) const
{
	CView::Dump(dc);
}

// ..................................................................
CADDoc* CADView::GetDocument( ) const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADDoc)));
	return (CADDoc*)m_pDocument;
}
#endif //_DEBUG


