
// GearsView.h : interface of the CGearsView class
//

#pragma once

class CGearsView : public CADView
{
protected: // create from serialization only
	CGearsView( );
	DECLARE_DYNCREATE( CGearsView )

// Attributes
public:
	CGearsDoc* GetDocument( ) const;

	bool bAnimate;
	public:
		virtual ~CGearsView();

//	//screen params
//	CRect rectS;
//	double winScale;
//	long offsetx;
//	long offsety;
//	//referance to screen params for Drawer
//	DrawExtent d_ext;
//
//	CPoint ptLastMouse;
//
//	WinScroller scroller;
//	//use when board size or zoom changes
//	void SetDisplaySize( );
//	//for parent frame status bar
//	bool bStatusMouseChanged;
//	CString strStatus;
//	CString GetStatusInfo( );
//
//	//this should be evolved as objects belong to doc
//
//public:
//	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
//	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
//protected:
//	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
//	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
//	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
//	virtual void OnUpdate( CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/ );
//
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnToolsAnimate();
	afx_msg void OnUpdateToolsAnimate(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	
//	afx_msg void OnFilePrintPreview();
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//
//	afx_msg LRESULT OnFrmNotify( WPARAM wp, LPARAM lp );
//
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in GearsView.cpp
inline CGearsDoc* CGearsView::GetDocument() const
   { return reinterpret_cast<CGearsDoc*>(m_pDocument); }
#endif

