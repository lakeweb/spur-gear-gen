
// GearsDoc.h : interface of the CGearsDoc class
//


#pragma once

/*
	DrawingObects
		owns a object_set_t called primatives 

	object_set_t a vector of ObjectSet;

	ObjectSet
		owns asp_obj_vect_t
			which is a vector of shared pointers SP_BaseItem, to ItemObj
*/


class CGearsDoc : public CDocument
{
protected:
	CGearsDoc( );
	DECLARE_DYNCREATE( CGearsDoc )

	layer_set_t layers;
    DrawingObects drawobj;

	//experimental, not used yet
	gc_process gc;

	std::string gear_info;

	//void gear( );
	void test( );

	void gear_org( );

public:
	DrawingObects& GetDrawingObjects( ) { return drawobj; }
	const std::string* GetInfoStr( ) const { return &gear_info; }
	const layer_set_t& GetLayers( ) const { return layers; }
public:
	virtual ~CGearsDoc( );
	virtual BOOL OnNewDocument( );
	virtual void Serialize( CArchive& ar );
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent( );
	virtual void OnDrawThumbnail( CDC& dc, LPRECT lprcBounds );
protected:
	void SetSearchContent( const CString& value );
#endif // SHARED_HANDLERS

public:
#ifdef _DEBUG
	virtual void AssertValid( ) const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

protected:
	DECLARE_MESSAGE_MAP( )

	afx_msg void OnFileTest();
	afx_msg LRESULT OnLayerEnable(WPARAM, LPARAM );
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnFileGCode();
};
