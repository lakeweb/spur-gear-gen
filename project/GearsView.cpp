
// GearsView.cpp : implementation of the CGearsView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.

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

#include "GearsDoc.h"
#include "GearsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SHOW_PARAMS
// ..................................................................
IMPLEMENT_DYNCREATE( CGearsView, CView )
BEGIN_MESSAGE_MAP( CGearsView, CView )
	// Standard printing commands
	ON_COMMAND( ID_FILE_PRINT, &CView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_DIRECT, &CView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, &CGearsView::OnFilePrintPreview )
	ON_WM_CONTEXTMENU( )
	ON_WM_RBUTTONUP( )
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL( )
	ON_WM_HSCROLL( )
	ON_WM_SIZE( )
	ON_WM_TIMER( )
	ON_MESSAGE( VIEW_INFO_NOTIFY, &CGearsView::OnFrmNotify )
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_TOOLS_ANIMATE, &CGearsView::OnToolsAnimate)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ANIMATE, &CGearsView::OnUpdateToolsAnimate)
END_MESSAGE_MAP( )

// ..................................................................
CGearsView::CGearsView( )
	:winScale( 140 )
	,offsetx( 500 )
	,offsety( 500 )
	,d_ext( rectS, offsetx, offsety, winScale )
	,bStatusMouseChanged( true )
	//todo
	,bAnimate( false )
{
}

// ..................................................................
CGearsView::~CGearsView( )
{
}

LRESULT CGearsView::OnFrmNotify( WPARAM wp, LPARAM lp )
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
CString CGearsView::GetStatusInfo( )
{
	if( bStatusMouseChanged )
	{
		strStatus.Format( _T(" x: %.5f y: %.5f")
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
BOOL CGearsView::PreCreateWindow( CREATESTRUCT& cs )
{
#ifdef USING_CONSOLE
	Create_STD_OUT_Console( );
#endif
	cs.style|= WS_HSCROLL | WS_VSCROLL;
	scroller.AttachWnd( this );

	return CView::PreCreateWindow( cs );
}

// ..................................................................
void CGearsView::OnDraw(CDC* poDC )
{
	CGearsDoc* pDoc = GetDocument( );
	ASSERT_VALID( pDoc );
	if( ! pDoc )
		return;

    offsetx= -scroller.GetScrollPos( ).cx + 400;
    offsety= -scroller.GetScrollPos( ).cy + 400;
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
	CString strT;

	crect.top= crect.bottom - 80;
	strT.Format( L"view size: %d %d", scroller.GetDisplaySize( ).cx, scroller.GetDisplaySize( ).cy );
	pDC->DrawText( strT, crect, DT_LEFT | DT_NOPREFIX );
	crect.top-= 30;
	strT.Format( L"page size: %d %d", scroller.GetPageSize( ).cx, scroller.GetPageSize( ).cy );
	pDC->DrawText( strT, crect, DT_LEFT | DT_NOPREFIX );
	crect.top-= 20;
	strT.Format( L"winscale %f", winScale );
	pDC->DrawText( strT, crect, DT_LEFT | DT_NOPREFIX );
}

// ..................................................................
void CGearsView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

// ..................................................................
BOOL CGearsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CGearsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

// ..................................................................
void CGearsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

// ..................................................................
void CGearsView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

// ..................................................................
void CGearsView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// ............................................................................
void CGearsView::OnMouseMove(UINT nFlags, CPoint point)
{
	if( ptLastMouse != point )
		bStatusMouseChanged= true;
	else
		return;

	ptLastMouse= point;
	CView::OnMouseMove(nFlags, point);
}

// ..................................................................
BOOL CGearsView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
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
void CGearsView::OnUpdate( CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	//This gets called after the doc has a PCB loaded.
	SetDisplaySize( );

	info_notify_t notice( info_notify_t::output );
	notice.basic_info= *GetDocument( )->GetInfoStr( );
	::SendMessage( *GetParentFrame( ), APP_INFO_NOTIFY, (WPARAM)&notice, 0 );

	notice.the_call= info_notify_t::layer;
	const layer_set_t& layers= GetDocument( )->GetLayers( );
	::SendMessage( *GetParentFrame( ), APP_INFO_NOTIFY, (WPARAM)&notice, (LPARAM)&layers );
}

// ............................................................................
void CGearsView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//VIEWTRACE( "VScroll pos: %d  code: %d\n", nPos, nSBCode );
	scroller.OnVScroll( nSBCode, nPos, pScrollBar );
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
#ifdef SHOW_PARAMS
	CRect crect;
	GetClientRect( crect );
	crect.top= crect.bottom - 120;
	InvalidateRect( crect );
#endif
}

// ............................................................................
void CGearsView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	scroller.OnHScroll( nSBCode, nPos, pScrollBar );
	CView::OnHScroll( nSBCode, nPos, pScrollBar );
#ifdef SHOW_PARAMS
	CRect crect;
	GetClientRect( crect );
	crect.top= crect.bottom - 100;
	InvalidateRect( crect );
#endif
}

// ............................................................................
void CGearsView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );
	scroller.OnSize( nType, cx, cy );
}

// ............................................................................
void CGearsView::SetDisplaySize( )
{
	//Big TODO !!!!!!!!
	scroller.SetDisplaySize( 2000, 2000 );
	//testing
	Invalidate( );
}

// ..................................................................
#ifdef _DEBUG
void CGearsView::AssertValid() const
{
	CView::AssertValid();
}

// ..................................................................
void CGearsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

// ..................................................................
CGearsDoc* CGearsView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGearsDoc)));
	return (CGearsDoc*)m_pDocument;
}
#endif //_DEBUG


// roll the gears............................................
void CGearsView::OnToolsAnimate( )
{
	bAnimate= ! bAnimate;
	if( bAnimate )
	{
		SetTimer( ID_ANIMATE_TIMER, 40, nullptr );
	}
	else
		KillTimer( ID_ANIMATE_TIMER );
}

void CGearsView::OnUpdateToolsAnimate(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck( bAnimate );
}

void CGearsView::OnTimer( UINT nIDEvent )
{
	auto& set= GetDocument( )->GetDrawingObjects( );
	for( auto& gear : set.get_set( ) )
	{
		for( auto& item : gear )
		{
			if( gear.get_id( ) == 1 )
			{
				const_cast< SP_BaseItem& >( item )= rotate_object( .002, item, gear.get_offset( ) );
			}
			else if( gear.get_id( ) == 2 )
			{
				const_cast< SP_BaseItem& >( item )= rotate_object( -.002, item, gear.get_offset( ) );
			}
		}
	}
	Invalidate( );
}


//void CGearsView::OnTimer( UINT nIDEvent )
//{
//	DrawingObects newset( GetDocument( )->layers );
//	ObjectSet oseta;
//	oseta.set_id( 1 );
//	ObjectSet osetb;
//	osetb.set_id( 2 );
//	auto& set= GetDocument( )->GetDrawingObjects( );
//	osetb.set_offset( set.get_set( ).at( 1 ).get_offset( ) );
//	if( set.get_set( ).size( ) > 2 )
//		return;
//
//	for( auto& gear : set.get_set( ) )
//	{
//		for( auto& item : gear )
//		{
//			if( gear.get_id( ) == 1 )
//			{
//				//auto temp= const_cast< SP_BaseItem& >( item );
//	//			temp= rotate_object( temp, bg_point( 0, 0 ), .4 );
//				oseta.push_back( rotate_object( .02, item, gear.get_offset( ) ) );
//				//const_cast< SP_BaseItem& >( item )= temp;
//			}
//			else if( gear.get_id( ) == 2 )
//			{
//				osetb.push_back( rotate_object( -.02, item, gear.get_offset( ) ) );
//			}
//			//else assert( false );
//
//		}
//	}
//	//newset.push_back( oseta );
//	//GetDocument( )->GetDrawingObjects( ).get_set( ).clear( );
//	//GetDocument( )->GetDrawingObjects( ).get_set( ).push_back( oseta );
//	set.get_set( ).clear( );
//	set.push_back( oseta );
//	set.push_back( osetb );
//	Invalidate( );
//	//TRACE( "timer set size: %d\n", set.get_set( ).size( ) );
//	//MessageBeep(0xFFFFFFFF);   // Beep
//}
