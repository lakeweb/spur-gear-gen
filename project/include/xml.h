//
// xml.h, but of course!
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef XML_H
#define XML_H

#ifndef GLOBALS_H
#include <globals.h>
#endif

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

#ifndef _HE_GLOBALS
// SLPVOID: smart pointer for buffers that must be deleted .............
// TODO depreciate this, use boost::shared_ptr
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

//Tiny specific, not used at the app level.
#ifndef TINYXML_INCLUDED
class TiXmlNode;
class TiXmlDocument;
#endif //TINYXML_INCLUDED

class CXML;
class XMLNODESET;

//new for unicode .................
#define LPCXSTR const char*
#define _X(x)      x
#define _tcscmp2 strcmp
#define _tstol2 atol
#ifdef UNICODE
#define XMLUNICODE
// !!!!! this is not thread safe !!!!!
HE_XML_EXT_CLASS const wchar_t* utf8_to_uni( const char* pIn );
#define XML_TOUNICODE( x ) utf8_to_uni( x )
// !!!!! this is not thread safe !!!!!
HE_XML_EXT_CLASS const char* uni_t0_utf8( const wchar_t* pIn );
#define XML_TOUTF8( x ) uni_t0_utf8( x )
#else
#define XML_TOUNICODE
#define XML_TOUTF8
#endif

//.................................


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
	XMLNODE( CXML& ixml );
	operator CXML& ( );
	//void operator = ( XMLNODE& inNode );
	virtual ~XMLNODE( ){ }
	CXML& GetXML( ) { return *xml; }
	//after a node= funct(...), if no create, was node found
	bool IsValid( ) { return node != NULL; }
	//Gets name of this node
	LPCXSTR GetTag( );
	//Compare name of this node, true if matches
	bool CompareTag( LPCXSTR inTag ) { return ! _tcscmp2(  GetTag( ), inTag ); }
	//return if the TEXT child.value, else emptyStr
	LPCTSTR GetValue( );
	//sets the text value of this node.
	bool SetValue( LPCTSTR aStr );
	//returns attribute value by name
	LPCTSTR GetAttribute( LPCXSTR tag );
	//returns attribute of type count
	size_t GetAttributeSize( LPCXSTR tag ) { return _tstol( GetAttribute( tag ) ); }
	//returns attribute value, 'true' or 'false'
	bool GetAttributeBool( LPCXSTR tag );
	//True if the current child tag mathches.
	bool CompareChildTag( LPCXSTR inTag );
	//setting, adding values
	// set/add an attribute
	void SetAttribute( LPCXSTR tag, LPCTSTR value );
	// set/add attribute long
	void SetAttribute( LPCXSTR tag, long value );
	// Always adds, even if duplicate
	XMLNODE AddChild( LPCXSTR tag= NULL );
	// delete all the children of this node
	void DeleteChildren( );
	// delete the current child, return false if last child or fail
	bool DeleteCurrentChild( );
	//Sets the child node of this and returns the child.
	XMLNODE SetChild( LPCXSTR tag, bool bCanCreate= false );
	//Sets the child by tag, attribute name (atr), and attribute value (val)
	XMLNODE SetChildByAttribute( LPCXSTR tag, LPCXSTR atr, LPCTSTR val, bool bCanCreate= false );
	//Set child by index. Returns false if out of range.
	bool SetCurrentChild( long index= 0 );
	//returns an xmlnode representing the current child.
	XMLNODE GetCurrentChild( ) { return XMLNODE( xml, currentChild ); }
	//Copy node and all children recursivly, Source node can be from different CMLX object
	bool InsertChild( XMLNODE inNode );

	XMLNODE GetElement( LPCXSTR tag, bool bDontCreate= false );
	XMLNODE GetParent( );

	//An element of a node is considered a child. Creates nodes if doc enabled.
	XMLNODE SetElementValue( LPCXSTR tagName, LPCTSTR value );
	XMLNODE SetElementValue( LPCXSTR tagName, bool value );
	XMLNODE SetElementValue( LPCXSTR tagName, long value );
	XMLNODE SetElementValue( LPCXSTR tagName, double& value );
	XMLNODE GetElementValue( LPCXSTR tagName, std::tstring& value, LPCTSTR defaultVal= NULL );
	XMLNODE GetElementValue( LPCXSTR tagName, CString& value, LPCTSTR defaultVal= NULL );
	XMLNODE GetElementValue( LPCXSTR tagName, bool& value, bool defaultVal= false );
	XMLNODE GetElementValue( LPCXSTR tagName, long& value, long defaultVal= 0 );
	XMLNODE GetElementValue( LPCXSTR tagName, double& value, double defaultVal= 0.0 );
#ifdef _WINDOWS
	XMLNODE SetElementValue( LPCXSTR tagName, POINT value );
	XMLNODE SetElementValue( LPCXSTR tagName, RECT value );
	XMLNODE SetElementValue( LPCXSTR tagName, COleCurrency& value );
	XMLNODE SetElementValue( LPCXSTR tagName, COleDateTime& value );
	//XMLNODE SetElementValue( LPCXSTR tagname, LOGFONT& value );
	XMLNODE SetElementValue( LPCXSTR tagname, COLORREF& value );
	void SetXMLWindowPlacement( WINDOWPLACEMENT* place, LPCXSTR name= _X("WindowPlacement") );

	XMLNODE GetElementValue( LPCXSTR tagName, POINT& value, POINT defaultVal= CPoint( 0, 0 ) );
	XMLNODE GetElementValue( LPCXSTR tagName, RECT& value, RECT defaultVal= CRect( 0, 0, 0, 0 ) );
	XMLNODE GetElementValue( LPCXSTR tagName, COleCurrency& value );
	XMLNODE GetElementValue( LPCXSTR tagName, COleDateTime& value );
	//XMLNODE GetElementValue_x( LPCXSTR tagName, LOGFONT& value );
	XMLNODE GetElementValue( LPCXSTR tagName, COLORREF& value, COLORREF defaultVal= RGB( 255, 255, 255 ) );
	void GetXMLWindowPlacement( WINDOWPLACEMENT* place, LPCXSTR name= _X("WindowPlacement") );
#endif
#ifdef UNICODE
	XMLNODE GetElementValue( LPCTSTR tagName, long& value, long dval= 0 ) { return GetElementValue( XML_TOUTF8( tagName ), value, dval ); }
	XMLNODE GetElementValue( LPCTSTR tagName, DWORD& value, DWORD dval= 0 ) { return GetElementValue( XML_TOUTF8( tagName ), value, dval ); }
	XMLNODE GetElementValue( LPCTSTR tagName, RECT& value, RECT dval= CRect( 0, 0, 0, 0 ) ) { return GetElementValue( XML_TOUTF8( tagName ), value, dval ); }
	XMLNODE SetElementValue( LPCTSTR tagName, long value ) { return SetElementValue( XML_TOUTF8( tagName ), value ); }
	XMLNODE SetElementValue( LPCTSTR tagName, DWORD value ) { return SetElementValue( XML_TOUTF8( tagName ), (long)value ); }
	XMLNODE SetElementValue( LPCTSTR tagName, RECT value ) { return SetElementValue( XML_TOUTF8( tagName ), value ); }
	XMLNODE GetElementValue( LPCTSTR tagName, LPBYTE* ppByte, DWORD* pCount );
	XMLNODE SetElementValue( LPCTSTR tagName, LPBYTE ppByte, DWORD count );
#endif // UNICODE
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
	bool CompareCurrentTag( LPCXSTR inTag );
	LPCTSTR GetCurrentAttribute( LPCXSTR inAt );
	XMLNODE SetCurrentNodeByAttribute( LPCXSTR name, LPCXSTR tag, LPCTSTR value, bool canCreate= false );
	bool GetElementValue( LPCXSTR tag, CString& value, LPCTSTR defaultVal= _T("") );
	bool GetElementValue( LPCXSTR tag, long& value, long defaultVal= 0 );
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
	std::string sFileName;
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
	void CreateNodes( LPCXSTR xpattern );

public:
	operator XMLNODE ( ) { return XMLNODE( this, currentNode ); }
	void SetCanCreate( bool bCan= true ) { bCanCreate= bCan; }
	unsigned long GetLastXPathCount( ) { return lastXPathCount; }
	HRESULT GetLastXPathError( ) { return lastXPathError; }
//File stuff...
	bool CreateEmptyDoc( LPCXSTR root= _X("Root") );
	void Clear( );
	bool ReadFile( LPCTSTR filename );
	bool WriteFile( LPCTSTR filename );
	//Open and Close to maintain file connection
	bool Open( LPCTSTR filename, LPCXSTR root= _X("Root"), int openMode= modeRead );
	bool Open( LPCTSTR pstrFilePathName, OpenFlags openMode= modeRead );

	//This is the xml object so this replaces the root if there is previous data
	bool ReadString( LPCXSTR pstrInStr );
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
	XMLNODE GetNode( LPCXSTR xpattern= NULL, bool bCanCreate= false );
#ifdef UNICODE
	XMLNODE GetNode( LPCTSTR xpattern= NULL, bool bCanCreate= false ) { return GetNode( XML_TOUTF8( xpattern ), bCanCreate ); }
#endif
	//Get the currentNode
	XMLNODE GetCurrentNode( ) { return XMLNODE( this, currentNode ); }
	//XMLNODE GetFirstChild( ) { return XMLNODE( this, GetFirstElement( ) ); }
	XMLNODE GetLastChild( ) { return XMLNODE( this, currentChild ); }
	//Only works if new node is child for now.
	bool SetCurrentNode( XMLNODE& aNode );
	//if xpattern is null, sets to root
	bool SetCurrentNode( LPCXSTR xpattern= NULL );
	//Move CXML currentNode to parent. Fails if presently at root
	bool UpNode( );
	// delete the current node and all childern, new current node becomes parent
	// fails if current node is document.
	bool DeleteCurrentNode( );
	// if xpattern is NULL, returns empty set
	XMLNODESET GetNodeSet( LPCXSTR xpattern= NULL );

//element stuff, elements are the children of the current node.
	XMLNODE GetElement( LPCXSTR tag, bool bDontCreate= false );

	XMLNODE GetElementByAtribute( LPCXSTR tag, LPCXSTR atr, LPCTSTR value );
	bool SetElementValue( LPCXSTR tag, LPCTSTR value );
	bool SetElementValue( LPCXSTR tag, bool value )
		{ return SetElementValue( tag, value ? _T("true") : _T("false") ); }
	bool SetElementValue( LPCXSTR tag, long value )
		{ CString t; t.Format( _T("%d"), value ); return SetElementValue( tag, t ); }
	bool SetElementValue( LPCXSTR tag, double value )
		{ CString t; t.Format( _T("%f"), value ); return SetElementValue( tag, t ); }
	bool GetElementValue( LPCXSTR tag, CString& value, LPCTSTR defaultVal= _T("") );
	bool GetElementValue( LPCXSTR tag, std::tstring& value, LPCTSTR defaultVal= _T("") );
	bool GetElementValue( LPCXSTR tag, bool& value, bool defaultVal= false );
	bool GetElementValue( LPCXSTR tag, long& value, long defaultVal= 0 );
	bool GetElementValue( LPCXSTR tag, double& value, double defaultVal= 0.0 );
	//new 12/12/13 pcb
	bool SetElementValue( LPCXSTR tag, LPBYTE value, DWORD count );
	bool GetElementValue( LPCXSTR tag, LPBYTE* value, DWORD* pCount );//count is safety check


#ifdef _WINDOWS
	bool SetElementValue( LPCXSTR tagName, POINT value );
	bool SetElementValue( LPCXSTR tagName, RECT value );
	bool SetElementValue( LPCXSTR tagName, COleCurrency& value )
		{ return SetElementValue( tagName, value.Format( ) ); }
	bool SetElementValue( LPCXSTR tagName, COleDateTime& value )
		{ return SetElementValue( tagName, value.Format( ) ); } 
	bool SetElementValue( LPCXSTR tagName, LOGFONTA& value );
	bool SetElementValue( LPCXSTR tagName, COLORREF value );

	bool GetElementValue( LPCXSTR tagName, POINT& value, POINT defaultVal= CPoint( 0, 0 ) );
	bool GetElementValue( LPCXSTR tagName, RECT& value, RECT defaultVal= CRect( 0, 0, 0, 0 ) );
	bool GetElementValue( LPCXSTR tagName, COleCurrency& value );
	bool GetElementValue( LPCXSTR tagName, COleDateTime& value );
	bool GetElementValue( LPCXSTR tagName, LOGFONTA& value, LOGFONT& defaultVal= _LOGFONT_defaultVal );
	bool GetElementValue( LPCXSTR tagName, COLORREF& value, COLORREF defaultVal= RGB( 255, 255, 255 ) );
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

#endif //XML_H
