// devDoc.h : interface of the CdevDoc class
//


#pragma once

class CdevDoc : public CDocument
{
protected: // create from serialization only
	CdevDoc();
	DECLARE_DYNCREATE(CdevDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CdevDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


