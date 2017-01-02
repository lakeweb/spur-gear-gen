//
// xml.h, but of course!
//
//#include <std_share.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#ifndef XML_H
//#define XML_H

#pragma inline_depth ( 2 )

#if defined( _LIB )
#define HE_XML_EXT_CLASS
#elif defined( HEXML_ASLIB )
#define HE_XML_EXT_CLASS
#elif ! defined( HEXML_RAW )
#define HE_XML_EXT_CLASS AFX_EXT_CLASS
#else
#define HE_XML_EXT_CLASS
#endif

#ifndef _VECTOR_
#include <vector>
#endif //_VECTOR_

#ifdef _WINDOWS //DEFAULTS
AFX_STATIC_DATA LOGFONT _LOGFONT_defaultVal= {
	0,
	0,
	0,
	0,
	FW_NORMAL,
	FALSE,
	FALSE,
	FALSE,
	ANSI_CHARSET,
	OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS,
	DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_DONTCARE,
	_T("Times New Roman")
};

#endif// _WINDOWS //DEFAULTS

//#ifndef __ATLSTR_H__
//#include <atlstr.h>
//typedef void* LPVOID;
//typedef char* LPTSTR;
//typedef const char* LPCTSTR;
//#undef HE_XML_EXT_CLASS
//#define HE_XML_EXT_CLASS
//#endif


#ifndef _HE_GLOBALS
#ifndef LPVOID
typedef void* LPVOID;
#endif
#ifndef LPTSTR
typedef TCHAR* LPTSTR;
#endif
#ifndef LPCTSTR
typedef const TCHAR* LPCTSTR;
#endif

// SLPVOID: smart pointer for buffers that must be deleted .............
struct SLPVOID
{
	LPVOID ptr;
public:
	SLPVOID( LPVOID p= 0 ): ptr( p ) { }
	operator LPVOID ( ) { return ptr; }
	operator LPTSTR ( ) { return (LPTSTR)ptr; }
	LPVOID* operator & ( ) { return &ptr; }
	~SLPVOID( ) { delete ptr; }
};
#endif //_HE_GLOBALS

//Tiny specific, not used at the higher level.
#ifndef TINYXML_INCLUDED
class TiXmlNode;
class TiXmlDocument;
#endif //TINYXML_INCLUDED

class CXML;
class XMLNODESET;

class HE_XML_EXT_CLASS XMLNODE
{
protected:
	friend CXML;
	friend XMLNODESET;
	CXML* xml;
	TiXmlNode* node; //this
	TiXmlNode* currentChild;
	//tinyxml does not provide for an empty TEXT string
	static std::tstring emptyStr;
	//protected constructors
	XMLNODE( )
		:node( NULL )
		,currentChild( NULL )
	{ }
	XMLNODE( CXML* s_xml, TiXmlNode* s_node );

public:
	XMLNODE( XMLNODE& inNode );
	operator CXML& ( );
	//void operator = ( XMLNODE& inNode );
	virtual ~XMLNODE( ){ }
	CXML& GetXML( ) { return *xml; }
	bool IsValid( ) { return node != NULL; }
	//Gets name of this node
	LPCTSTR GetTag( );
	//Compare name of this node, true if matches
	bool CompareTag( LPCTSTR inTag ) { return !_tcscmp( GetTag( ), inTag ); }
	//return if the TEXT child.value, else emptyStr
	LPCTSTR GetValue( );
	//sets the text value of this node.
	bool SetValue( LPCTSTR aStr );
	//returns attribute value by name
	LPCTSTR GetAttribute( LPCTSTR tag );
	//returns attribute of type count
	size_t GetAttributeSize( LPCTSTR tag ) { return _tstol( GetAttribute( tag ) ); }
	//returns attribute value, 'true' or 'false'
	bool GetAttributeBool( LPCTSTR tag );
	// set/add an attribute
	void SetAttribute( LPCTSTR tag, LPCTSTR value );
	// set/add attribute long
	void SetAttribute( LPCTSTR tag, long value );
	// Always adds, even if duplicate
	XMLNODE AddChild( LPCTSTR tag= NULL );
	// delete all the children of this node
	void DeleteChildren( );
	// delete the current child, return false if last child or fail
	bool DeleteCurrentChild( );
	//Sets the child node of this and returns the child.
	XMLNODE SetChild( LPCTSTR tag, bool bCanCreate= false );
	//Sets the child by tag, attribute name (atr), and attribute value (val)
	XMLNODE SetChildByAttribute( LPCTSTR tag, LPCTSTR atr, LPCTSTR val, bool bCanCreate= false );
	//True if the current child tag mathches.
	bool CompareChildTag( LPCTSTR inTag );
	//Set child by index. Returns false if out of range.
	bool SetCurrentChild( long index= 0 );
	//returns an xmlnode representing the current child.
	XMLNODE GetCurrentChild( ) { return XMLNODE( xml, currentChild ); }
	//Copy node and all children recursivly, Source node can be from different CMLX object
	bool InsertChild( XMLNODE inNode );

	bool WriteString( std::string& str );
	bool ReadString( LPCTSTR pstrInStr );

	XMLNODE GetElement( LPCTSTR tag, bool bDontCreate= false );
	XMLNODE GetParent( );

	//An element of a node is considered a child. Creates nodes if doc enabled.
	XMLNODE SetElementValue( LPCTSTR tagName, LPCTSTR value );
	XMLNODE SetElementValue( LPCTSTR tagName, bool value );
	XMLNODE SetElementValue( LPCTSTR tagName, long value );
	XMLNODE SetElementValue( LPCTSTR tagName, double& value );
	XMLNODE GetElementValue( LPCTSTR tagName, std::string& value, LPCTSTR defaultVal= NULL );
	XMLNODE GetElementValue( LPCTSTR tagName, CString& value, LPCTSTR defaultVal= NULL );
	XMLNODE GetElementValue( LPCTSTR tagName, bool& value, bool defaultVal= false );
	XMLNODE GetElementValue( LPCTSTR tagName, long& value, long defaultVal= 0 );
	XMLNODE GetElementValue( LPCTSTR tagName, double& value, double defaultVal= 0.0 );
#ifdef _WINDOWS
	XMLNODE SetElementValue( LPCTSTR tagName, POINT value );
	XMLNODE SetElementValue( LPCTSTR tagName, RECT value );
	XMLNODE SetElementValue( LPCTSTR tagName, COleCurrency& value );
	XMLNODE SetElementValue( LPCTSTR tagName, COleDateTime& value );
	XMLNODE SetElementValue( LPCTSTR tagname, LOGFONT& value );
	XMLNODE SetElementValue( LPCTSTR tagname, COLORREF& value );
	void SetXMLWindowPlacement( WINDOWPLACEMENT* place, LPCTSTR name= _T("WindowPlacement") );

	XMLNODE GetElementValue( LPCTSTR tagName, POINT& value, POINT defaultVal= CPoint( 0, 0 ) );
	XMLNODE GetElementValue( LPCTSTR tagName, RECT& value, RECT defaultVal= CRect( 0, 0, 0, 0 ) );
	XMLNODE GetElementValue( LPCTSTR tagName, COleCurrency& value );
	XMLNODE GetElementValue( LPCTSTR tagName, COleDateTime& value );
	XMLNODE GetElementValue_x( LPCTSTR tagName, LOGFONT& value );//, LOGFONT& defaultVal= _LOGFONT_defaultVal );
	XMLNODE GetElementValue( LPCTSTR tagName, COLORREF& value, COLORREF defaultVal= RGB( 255, 255, 255 ) );
	//XMLNODE GetElementValue( LPCTSTR tagName, LOGFONT& value, LOGFONT& defaultVal= _LOGFONT_defaultVal );
	void GetXMLWindowPlacement( WINDOWPLACEMENT* place, LPCTSTR name= _T("WindowPlacement") );
#endif
};

class HE_XML_EXT_CLASS XMLNODESET
{
private:
	friend CXML;
	CXML* xml;
	TiXmlNode* node;
	std::vector<TiXmlNode*> set;
	//protected constructor
	XMLNODESET( CXML* s_xml );
	XMLNODESET( );

public:
	//XMLNODESET( const XMLNODESET& inSet );
	~XMLNODESET( );
	operator CXML& ( );
	XMLNODESET& operator += ( XMLNODE& aNode );
	bool SetCurrentNode( unsigned long index );
	XMLNODE GetCurrentNode( ) { return XMLNODE( xml, node ); }
	bool CompareCurrentTag( LPCTSTR inTag );
	LPCTSTR GetCurrentAttribute( LPCTSTR inAt );
	XMLNODE SetCurrentNodeByAttribute( LPCTSTR name, LPCTSTR tag, LPCTSTR value, bool canCreate= false );
	bool GetElementValue( LPCTSTR tag, CString& value, LPCTSTR defaultVal= _T("") );
	bool GetElementValue( LPCTSTR tag, long& value, long defaultVal= 0 );
	unsigned long GetSize( ) { return (unsigned long)set.size( ); }
};

class HE_XML_EXT_CLASS CXML
{
	friend XMLNODE;
	friend XMLNODESET;
	friend HE_XML_EXT_CLASS void SetXMLWindowPlacement( XMLNODE& node, WINDOWPLACEMENT& place, LPCTSTR name );
	TiXmlDocument* documentNode;
	TiXmlNode* currentNode;
	TiXmlNode* currentChild;
	CString sFileName;
	bool bCanCreate;
	HRESULT lastXPathError;
	unsigned long lastXPathCount;
	//static XMLNODE emptyNode;

public:
	CXML::CXML( );
	CXML::~CXML( );
	//temp, debug develope
	//TiXmlDocument* GetRoot( ) { return documentNode; }
	void operator = (CXML& inXml );

	enum OpenFlags { //same as CFile
		modeRead=			(int) 0x0000,
		modeWrite=			(int) 0x0001,
		modeReadWrite=		(int) 0x0002,
		modeAddElements=	(int) 0x0010,
		modeNoInherit=		(int) 0x0080,
		modeCreate=			(int) 0x1000,
		modeNoTruncate=		(int) 0x2000,
		modeProgFile=		modeReadWrite | modeCreate | modeNoTruncate,
	};

	//return an empty node without instantiation
	//static XMLNODE& GetEmptyNode( ) { return emptyNode; }

protected:
	TiXmlNode* GetFirstElement( );
	TiXmlNode* GetLastElement( );
	void CreateNodes( LPCTSTR xpattern );

public:
	operator XMLNODE ( ) { return XMLNODE( this, currentNode ); }
	void SetCanCreate( bool bCan= true ) { bCanCreate= bCan; }
	unsigned long GetLastXPathCount( ) { return lastXPathCount; }
	HRESULT GetLastXPathError( ) { return lastXPathError; }

//File stuff...
	bool CreateEmptyDoc( LPCTSTR root= _T("Root") );
	void Clear( );
	bool ReadFile( LPCTSTR filename );
	bool WriteFile( LPCTSTR filename );
	//Open and Close to maintain file connection
	bool Open( LPCTSTR filename, LPCTSTR root= _T("Root"), int openMode= modeRead );
	bool Open( LPCTSTR pstrFilePathName, OpenFlags openMode= modeRead );

	//This is the xml object so this replaces the root if there is previous data
	bool ReadString( LPCTSTR pstrInStr );
	//xml data to string
	bool WriteString( CString& strOut );
	//with returns and leading spaces formating
	bool WriteStringFmt( CString& strOut );
	//Must close to save file!!!
	bool Close( );

	//To completly replace the document with node
	bool ReplaceCurrentNode( XMLNODE& node );

#ifdef _WINDOWS
	bool ReadFile( CFile* file ); //For CDocument::Serialize
#endif
//node stuff
	//All this will create the nodes if the
	//can create flag is true.
	//
	//Gets the first occurance of xpattern match
	//if xpattern null, gets current node
	XMLNODE GetNode( LPCTSTR xpattern= NULL, bool bCanCreate= true );
	//Get the currentNode
	XMLNODE GetCurrentNode( ) { return XMLNODE( this, currentNode ); }
	//XMLNODE GetFirstChild( ) { return XMLNODE( this, GetFirstElement( ) ); }
	XMLNODE GetLastChild( ) { return XMLNODE( this, currentChild ); }
	//Only works if new node is child for now.
	bool SetCurrentNode( XMLNODE& aNode );
	//if xpattern is null, sets to root
	bool SetCurrentNode( LPCTSTR xpattern= NULL );
	//Move CXML currentNode to parent. Fails if presently at root
	bool UpNode( );
	// delete the current node and all childern, new current node becomes parent
	// fails if current node is document.
	bool DeleteCurrentNode( );
	// if xpattern is NULL, returns empty set
	XMLNODESET GetNodeSet( LPCTSTR xpattern= NULL );
	//new 9/14/15
	int GetErrorCode( ) const;
	int GetErrorLine( ) const;
	LPCTSTR GetErrorStr( ) const;

//element stuff, elements are the children of the current node.
	XMLNODE GetElement( LPCTSTR tag, bool bDontCreate= false );

	XMLNODE GetElementByAtribute( LPCTSTR tag, LPCTSTR atr, LPCTSTR value );
	bool SetElementValue( LPCTSTR tag, LPCTSTR value );
	bool SetElementValue( LPCTSTR tag, bool value )
		{ return SetElementValue( tag, value ? _T("true") : _T("false") ); }
	bool SetElementValue( LPCTSTR tag, long value )
		{ CString t; t.Format( _T("%d"), value ); return SetElementValue( tag, t ); }
	bool SetElementValue( LPCTSTR tag, double value )
		{ CString t; t.Format( _T("%f"), value ); return SetElementValue( tag, t ); }
	bool GetElementValue( LPCTSTR tag, CString& value, LPCTSTR defaultVal= _T("") );
	bool GetElementValue( LPCTSTR tag, std::string& value, LPCTSTR defaultVal= _T("") );
	bool GetElementValue( LPCTSTR tag, bool& value, bool defaultVal= false );
	bool GetElementValue( LPCTSTR tag, long& value, long defaultVal= 0 );
	bool GetElementValue( LPCTSTR tag, double& value, double defaultVal= 0.0 );

#ifdef _WINDOWS
	bool SetElementValue( LPCTSTR tagName, POINT value );
	bool SetElementValue( LPCTSTR tagName, RECT value );
	bool SetElementValue( LPCTSTR tagName, COleCurrency& value )
		{ return SetElementValue( tagName, value.Format( ) ); }
	bool SetElementValue( LPCTSTR tagName, COleDateTime& value )
		{ return SetElementValue( tagName, value.Format( ) ); } 
	bool SetElementValue( LPCTSTR tagName, LOGFONTA& value );
	bool SetElementValue( LPCTSTR tagName, COLORREF value );

	bool GetElementValue( LPCTSTR tagName, POINT& value, POINT defaultVal= CPoint( 0, 0 ) );
	bool GetElementValue( LPCTSTR tagName, RECT& value, RECT defaultVal= CRect( 0, 0, 0, 0 ) );
	bool GetElementValue( LPCTSTR tagName, COleCurrency& value );
	bool GetElementValue( LPCTSTR tagName, COleDateTime& value );
	bool GetElementValue( LPCTSTR tagName, LOGFONTA& value, LOGFONT& defaultVal= _LOGFONT_defaultVal );
	bool GetElementValue( LPCTSTR tagName, COLORREF& value, COLORREF defaultVal= RGB( 255, 255, 255 ) );
#endif
};

inline XMLNODE::operator CXML& ( ) { xml->currentNode= node; return *xml; }
inline XMLNODESET::operator CXML& ( ) { xml->currentNode= node; return *xml; }

// ..........................................................................
//Boost stuff
#ifdef BOOST_SHARED_PTR_HPP_INCLUDED

typedef boost::shared_ptr<CXML> SP_XML;
typedef boost::shared_ptr<XMLNODE> SP_XMLNODE;

#endif

typedef CXML HEXML;

//latter if want, but needs tiny includes....
//#include <xml.inl>

//#endif //XML_H
