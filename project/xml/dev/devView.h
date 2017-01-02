// devView.h : interface of the CdevView class
//


#pragma once
#include <xml.h>

class CdevView : public CEditView
{
protected: // create from serialization only
	CdevView();
	DECLARE_DYNCREATE(CdevView)

	CXML xml;

	void WriteOutput( LPCSTR pszOutput, ... );

// Attributes
public:
	CdevDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CdevView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // debug version in devView.cpp
inline CdevDoc* CdevView::GetDocument() const
   { return reinterpret_cast<CdevDoc*>(m_pDocument); }
#endif

