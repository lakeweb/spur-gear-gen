//
// devDoc.cpp : implementation of the CdevDoc class
//

#include "stdafx.h"
#include "dev.h"

#include "devDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CdevDoc, CDocument)
BEGIN_MESSAGE_MAP(CdevDoc, CDocument)
END_MESSAGE_MAP()

CdevDoc::CdevDoc()
{
}

CdevDoc::~CdevDoc()
{
}

BOOL CdevDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CdevDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	reinterpret_cast<CEditView*>( m_viewList.GetHead( ) )->SerializeRaw( ar );
}

#ifdef _DEBUG
void CdevDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CdevDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
