// Filename: ScrollHelper.h
// S.Chan, 01 Jul 2005
// http://www.codeproject.com/Articles/10902/Add-Scrolling-to-a-CWnd-or-CDialog-using-a-C-Helpe

#ifndef SCROLLER_H
#define SCROLLER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void GetClientRectSB( CWnd* pWnd, CRect& rect );

class WinScroller
{
    CWnd*  m_attachWnd;
    CSize  m_pageSize;
    CSize  m_displaySize;
    CSize  m_scrollPos;

public:
// ............................................................................
	WinScroller( )
		:m_attachWnd( NULL )
		,m_pageSize( 0, 0 )
		,m_displaySize( 0, 0 )
		,m_scrollPos( 0, 0 )
	{ }

	~WinScroller( ) { DetachWnd( ); }

    // Attach/detach a CWnd or CDialog.
	void AttachWnd( CWnd* pWnd ) { m_attachWnd = pWnd; }
	void DetachWnd( ) { m_attachWnd = NULL; }


    // Set/get the virtual display size. When the dialog or window
    // size is smaller than the display size, then that is when
    // scrollbars will appear. Set either the display width or display
    // height to zero if you don't want to enable the scrollbar in the
    // corresponding direction.
    void SetDisplaySize( int displayWidth, int displayHeight );
	const CSize& GetDisplaySize() const { return m_displaySize; }

    // Get current scroll position. This is needed if you are scrolling
    // a custom CWnd which implements its own drawing in OnPaint().
	const CSize& GetScrollPos() const { return m_scrollPos; }

	void OffsetScrollPos( CSize& in ) { m_scrollPos+= in; }

    // Get current page size. Useful for debugging purposes.
	const CSize& GetPageSize() const { return m_pageSize; }

    // Scroll back to top, left, or top-left corner of the window.
    void ScrollToOrigin(bool scrollLeft, bool scrollTop);

    // Message handling.
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    BOOL OnMouseWheel( CSize ratio, CPoint ptMouse );//, CPoint pt, CSize size );
	void OnSize( UINT, int, int ) { UpdateScrollInfo( ); }
    bool UpdateScrollInfo( );

private:
    int    Get32BitScrollPos(int bar, CScrollBar* pScrollBar);
    void   UpdateScrollBar(int bar, int windowSize, int displaySize,
                           LONG& pageSize, LONG& scrollPos, LONG& deltaPos);

};

#endif // SCROLLER_H

// END
