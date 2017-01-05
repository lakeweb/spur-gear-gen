

#include "stdafx.h"
#include <std_share.h>
#include <globals.h>
#include <xml.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef UNICODE
const wchar_t* utf8_to_uni( const char* pIn )
{
	static std::wstring hold;
	hold= boost::locale::conv::utf_to_utf<wchar_t>( pIn );
	return hold.c_str( );
}

const char* uni_t0_utf8( const wchar_t* pIn )
{
	static std::string hold;
	hold= boost::locale::conv::utf_to_utf<char>( pIn );
	return hold.c_str( );
}

#endif


// windows stuff...
#ifdef _WINDOWS
void XMLNODE::GetXMLWindowPlacement( WINDOWPLACEMENT* place, LPCXSTR name )
{
	XMLNODE win_node= SetChild( name, true );
	win_node.GetElementValue( _X("length"), (long&)place->length );
	win_node.GetElementValue( _X("flags"), (long&)place->flags, 0xffff );
	win_node.GetElementValue( _X("showCmd"), (long&)place->showCmd );
	win_node.GetElementValue( _X("ptMinPosition"), place->ptMinPosition );
	win_node.GetElementValue( _X("ptMaxPosition"), place->ptMaxPosition );
	win_node.GetElementValue( _X("rcNormalPosition"), place->rcNormalPosition );
	xml->UpNode( );
}

void XMLNODE::SetXMLWindowPlacement( WINDOWPLACEMENT* place, LPCXSTR name )
{
	XMLNODE win_node= SetChild( name, true );
	win_node.SetElementValue( _X("length"), (long)place->length );
	win_node.SetElementValue( _X("flags"), (long)place->flags );
	win_node.SetElementValue( _X("showCmd"), (long)place->showCmd );
	win_node.SetElementValue( _X("ptMinPosition"), place->ptMinPosition );
	win_node.SetElementValue( _X("ptMaxPosition"), place->ptMaxPosition );
	win_node.SetElementValue( _X("rcNormalPosition"), place->rcNormalPosition );
	xml->UpNode( );
}
#endif //_WINDOWS

// XMLNODE ............................................................................
std::tstring XMLNODE::emptyStr;
//XMLNODE CXML::emptyNode( NULL, NULL );

XMLNODE::XMLNODE( CXML* s_xml, TiXmlNode* s_node )
	:xml( s_xml )
	,node( s_node )
	,currentChild( NULL )
{
	if( s_node != NULL )
	{
		ASSERT( s_node->Type( ) == TiXmlNode::ELEMENT );
		currentChild= s_node->FirstChildElement( );
	}
}

XMLNODE::XMLNODE( XMLNODE& inNode )
	:node( inNode.node )
	,currentChild( inNode.currentChild )
	,xml( inNode.xml )
{ }

XMLNODE::XMLNODE( CXML& ixml )
	:xml( &ixml )
	,node( ixml.currentNode )
	,currentChild( NULL )
{ }

//void XMLNODE::operator = ( XMLNODE& inNode )
//{
//	xml= inNode.xml;
//	node= inNode.node;
//	currentChild= inNode.currentChild;
//}

LPCXSTR XMLNODE::GetTag( )
{
	if( node == NULL )
		return (LPCXSTR)emptyStr.c_str( );

	return (LPCXSTR)node->ToElement( )->Value( );
}

bool XMLNODE::CompareChildTag( LPCXSTR inTag )
{
	if( currentChild == NULL
		|| currentChild->ToElement( )->Value( ) == NULL )
		return false;

	return ! _tcscmp2( currentChild->ToElement( )->Value( ), inTag );
}

bool XMLNODE::GetAttributeBool( LPCXSTR tag )
{
	LPCTSTR val= GetAttribute( tag );
	if( ! _tcscmp( val, _T("true") ) )
		return true;

	else if( ! _tcscmp( val, _T("false") ) )
		return false;

	ASSERT( false );
	return false;
}

LPCTSTR XMLNODE::GetAttribute( LPCXSTR tag )
{
	LPCXSTR t= node->ToElement( )->Attribute( tag );
	if( t )
		return XML_TOUNICODE( t );
	return emptyStr.c_str( );
}

LPCTSTR XMLNODE::GetValue( )
{
	if( node == NULL )
		return emptyStr.c_str( );

	for( TiXmlNode* it= node->FirstChild( ); it; it= it->NextSibling( ) )
		if( it->Type( ) == TiXmlNode::TEXT )
			return XML_TOUNICODE( it->Value( ) );
		return emptyStr.c_str( );
}

bool XMLNODE::SetCurrentChild( long index )
{
	if( node == NULL )
		return false;

	TiXmlElement* it;
	if( true ) ///bUseChildren )
		it= node->FirstChildElement( );
	else
		it= node->NextSiblingElement( );

	for( long i= 0; it; it= it->NextSiblingElement( ), ++i )
	{
		if( i == index )
		{
			currentChild= it;
			return true;
		}
	}
	currentChild= NULL;
	return false;
}

bool XMLNODE::DeleteCurrentChild( )
{
	if( currentChild )
	{
		node->RemoveChild( currentChild );
		return true;
	}
	return false;
}

void XMLNODE::DeleteChildren( )
{
	for( TiXmlNode* c= node->FirstChild( ); c; )
	{
		TiXmlNode* n= c->NextSibling( );
		if( c->Type( ) == TiXmlNode::ELEMENT )
			node->RemoveChild( c );
		c= n;
	}
}

XMLNODE XMLNODE::SetChild( LPCXSTR tag, bool bCanCreate )
{
	TiXmlElement* e= node->FirstChild( tag )->ToElement( );
	if( e != NULL )
		return XMLNODE( xml, currentChild= e );

	else if( bCanCreate )
		return XMLNODE( xml, currentChild= node->InsertEndChild( TiXmlElement( tag ) ) );

	//else
		currentChild= NULL;
		return XMLNODE( );
}

bool XMLNODE::InsertChild( XMLNODE inNode )
{
	//std::vector< TiXmlNode* > stack;
	//stack.push_back( inNode.node );
	//TiXmlNode* pNode= inNode.node->Clone( );
	node->InsertEndChild( *inNode.node );

	//for( ; stack.size( ); )
	//{	TiXmlNode* node= stack.back( );
	//	if( node->FirstChildElement( ) )
	//	{
	//		stack.push_back( node->FirstChildElement( ) );
	//	}
	//}
	return true;
}

XMLNODE XMLNODE::SetChildByAttribute( LPCXSTR tag, LPCXSTR atr, LPCTSTR val, bool bCanCreate )
{

	for( TiXmlNode* e= node->FirstChild( ); e; e= e->NextSibling( ) )
		if( e->Type( ) == TiXmlNode::ELEMENT && !_tcscmp2( e->Value( ), tag ) )
		{
			LPCXSTR s= e->ToElement( )->Attribute( atr );
			if( s && !_tcscmp2( XML_TOUTF8( val ), s ) )
				return XMLNODE( xml, e );
		}
	if( bCanCreate )
	{
		TiXmlNode* e= node->InsertEndChild( TiXmlElement( tag ) );
		e->ToElement( )->SetAttribute( atr, XML_TOUTF8( val ) );
		return XMLNODE( xml, e );
	}
	//else
		return XMLNODE( );
}

void XMLNODE::SetAttribute( LPCXSTR tag, LPCTSTR value )
{
	node->ToElement( )->SetAttribute( tag, XML_TOUTF8( value ) );
}

void XMLNODE::SetAttribute( LPCXSTR tag, long value )
{
	CString strT;
	strT.Format( _T("%d"), value );
	node->ToElement( )->SetAttribute( tag, XML_TOUTF8( strT ) );
}

bool XMLNODE::SetValue( LPCTSTR aStr )
{
	if( node == NULL )
		return false;

	for( TiXmlNode* it= node->FirstChild( ); it; it= it->NextSibling( ) )
		if( it->Type( ) == TiXmlNode::TEXT )
		{
			it->ToText( )->SetValue( XML_TOUTF8( aStr ) );
			return true;
		}
		node->ToElement( )->InsertEndChild( TiXmlText( XML_TOUTF8( aStr ) ) );
	return true;
}

XMLNODE XMLNODE::AddChild( LPCXSTR tag )
{
	if( ! tag )
		tag= _X("");

	return XMLNODE( xml, currentChild= node->InsertEndChild( TiXmlElement( tag ) ) );
}

XMLNODE XMLNODE::GetElement( LPCXSTR tag, bool bDontCreate )
{
	xml->currentNode= node;
	return xml->GetElement( tag, bDontCreate );
}

XMLNODE XMLNODE::GetParent( )
{
	return XMLNODE( xml, node->Parent( ) );
}

// ................................................................................
XMLNODE XMLNODE::SetElementValue( LPCXSTR tagName, LPCTSTR value )
{
	xml->currentNode= node;
	xml->SetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::SetElementValue( LPCXSTR tagName, bool value )
{
	xml->currentNode= node;
	xml->SetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::SetElementValue( LPCXSTR tagName, long value )
{
	xml->currentNode= node;
	xml->SetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::SetElementValue( LPCXSTR tagName, double& value )
{
	xml->currentNode= node;
	xml->SetElementValue( tagName, value );
	return xml->GetLastChild( );
}

#ifdef _WINDOWS
XMLNODE XMLNODE::SetElementValue( LPCXSTR tagName, COleCurrency& value )
{
	xml->currentNode= node;
	xml->SetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::SetElementValue( LPCXSTR tagName, COleDateTime& value )
{
	xml->currentNode= node;
	xml->SetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::SetElementValue( LPCXSTR tagName, POINT value )
{
	xml->currentNode= node;
	xml->SetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::SetElementValue( LPCXSTR tagName, RECT value )
{
	xml->currentNode= node;
	xml->SetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::SetElementValue( LPCXSTR tagName, COLORREF& value )
{
	xml->currentNode= node;
	xml->SetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::SetElementValue( LPCTSTR tagName, LPBYTE ppByte, DWORD count )
{
	xml->currentNode= node;
	xml->SetElementValue( XML_TOUTF8( tagName ), ppByte, count );
	return xml->GetLastChild( );
}
#endif

// .......
XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, std::tstring& value, LPCTSTR defaultVal )
{
	xml->currentNode= node;
	CString strT;
	xml->GetElementValue( tagName, strT, defaultVal );
	value= strT;
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, CString& value, LPCTSTR defaultVal )
{
	xml->currentNode= node;
	xml->GetElementValue( tagName, value, defaultVal );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, bool& value, bool defaultVal )
{
	xml->currentNode= node;
	xml->GetElementValue( tagName, value, defaultVal );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, long& value, long defaultVal )
{
	xml->currentNode= node;
	xml->GetElementValue( tagName, value, defaultVal );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, double& value, double defaultVal )
{
	xml->currentNode= node;
	xml->GetElementValue( tagName, value, defaultVal );
	return xml->GetLastChild( );
}

#ifdef _WINDOWS
XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, COleCurrency& value )
{
	xml->currentNode= node;
	xml->GetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, COleDateTime& value )
{
	xml->currentNode= node;
	xml->GetElementValue( tagName, value );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, POINT& value, POINT defaultVal )
{
	xml->currentNode= node;
	xml->GetElementValue( tagName, value, defaultVal );
	return xml->GetLastChild( );
}

XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, RECT& value, RECT defaultVal )
{
	xml->currentNode= node;
	xml->GetElementValue( tagName, value, defaultVal );
	return xml->GetLastChild( );
}

//XMLNODE XMLNODE::GetElementValue( LPCXSTR tagName, COLORREF& value, COLORREF defaultVal )
//{
//	xml->currentNode= node;
//	xml->GetElementValue( tagName, value, defaultVal );
//	return xml->GetLastChild( );
//}

XMLNODE XMLNODE::GetElementValue( LPCTSTR tagName, LPBYTE* ppByte, DWORD* pCount )
{
	xml->currentNode= node;
	xml->GetElementValue( XML_TOUTF8( tagName ), ppByte, pCount );
	return xml->GetLastChild( );
}

#endif

// XMLNODESET ............................................................................
XMLNODESET::XMLNODESET( )
{
}

XMLNODESET::XMLNODESET( CXML* s_xml )
	:xml( s_xml )
	,node( NULL )
{
	node= xml->currentNode;
}

/*
XMLNODESET::XMLNODESET( const XMLNODESET& inSet )
{
	*this= inSet;
}
*/

XMLNODESET::~XMLNODESET( )
{
}

bool XMLNODESET::CompareCurrentTag( LPCXSTR inTag )
{
	return !_tcscmp2( GetCurrentNode( ).GetTag( ), inTag );
}

LPCTSTR XMLNODESET::GetCurrentAttribute( LPCXSTR inAt )
{
	return GetCurrentNode( ).GetAttribute( inAt );
}

bool XMLNODESET::SetCurrentNode( unsigned long index )
{
	ASSERT( index <= set.size( ) ); //catch overshots
	if( index == set.size( ) )
		return false;
	node= set[index];
	return true;
}

XMLNODE XMLNODESET::SetCurrentNodeByAttribute( LPCXSTR name, LPCXSTR tag, LPCTSTR value, bool canCreate )
{
	for( ULONG i= 0; i < set.size( ); ++i )
	{
		LPCXSTR str= set[i]->ToElement( )->Attribute( tag );
		if( str != NULL && !_tcscmp2( str, XML_TOUTF8( value ) ) )
			return XMLNODE( xml, ( node= set[i] ) );
	}
	return XMLNODE( ); 
}

XMLNODESET& XMLNODESET::operator += ( XMLNODE& aNode )
{
	set.push_back( aNode.node );
	return *this;
}

bool XMLNODESET::GetElementValue( LPCXSTR tag, CString& value, LPCTSTR defaultVal )
{
	xml->currentNode= node;
	return xml->GetElementValue( tag, value, defaultVal );
}

bool XMLNODESET::GetElementValue( LPCXSTR tag, long& value, long defaultVal )
{
	xml->currentNode= node;
	return xml->GetElementValue( tag, value, defaultVal );
}

// CXML ............................................................................
CXML::CXML( )
	:bCanCreate( false )
	,currentNode( NULL )
	,currentChild( NULL )
	,lastXPathCount( 0 )
{
	documentNode= new TiXmlDocument;
	//documentNode->SetCondenseWhiteSpace( false );
}

CXML::~CXML( )
{
	delete documentNode;
}

void CXML::operator = (CXML& inXml )
{
	if( documentNode )
		delete documentNode;

	documentNode= inXml.documentNode->Clone( )->ToDocument( );
	currentNode= documentNode->RootElement( );
	//sFileName= inXml.sFileName;
}

//private
TiXmlNode* CXML::GetFirstElement( )
{
	return currentNode->FirstChildElement( );
}

TiXmlNode* CXML::GetLastElement( )
{
	TiXmlNode* last= NULL;
	for( TiXmlElement* it= currentNode->FirstChildElement( ); it; it= it->NextSiblingElement( ) )
		last= it;
	return last;
}

//public documents
bool CXML::CreateEmptyDoc( LPCXSTR root )
{
	CString strR( root );
	ASSERT( ! strR.IsEmpty( ) );
	std::string str( _X("<?xml version=\"1.0\" encoding=\"UTF-8\" ?><") );
	str+= root;
	str+= _X("/>");
	documentNode->Parse( str.c_str( ) );
	currentNode= documentNode->RootElement( );
	return true; //TODO Can't fail? mem...
}

bool CXML::Open( LPCTSTR filename, LPCXSTR root, int openMode )
{
	bool bNotErr= true;
	sFileName= XML_TOUTF8( filename );

	if( openMode & modeCreate || openMode & modeWrite )
		bCanCreate= true;

	if( openMode & modeCreate
		&& GetFileAttributes( filename ) == -1 )
	{
		CreateEmptyDoc( root );
		currentNode= documentNode->RootElement( );
		return true;
	}
	if( !( bNotErr= documentNode->LoadFile( XML_TOUTF8( filename ) ) ) && openMode & modeCreate )
	//empty or corrupt so if can, create empty doc
	{
		CreateEmptyDoc( root );
		currentNode= documentNode->RootElement( );
		return true;
	}
	if( !bNotErr )
	{
		currentNode= NULL;
        return false;
	}
	currentNode= documentNode->RootElement( );

	return currentNode != NULL;
}

bool CXML::Open( LPCTSTR filename, OpenFlags openMode )
{
	bool bNotErr= true;
	sFileName= XML_TOUTF8( filename );

	if( openMode & modeCreate || openMode & modeWrite )
		bCanCreate= true;

	if( openMode & modeCreate
		&& ( GetFileAttributes( filename ) == -1
			|| ! ( openMode & modeNoTruncate ) ) )
	{
		CreateEmptyDoc( _X("Root") );
		currentNode= documentNode->RootElement( );
		return true;
	}
	if( !( bNotErr= documentNode->LoadFile( XML_TOUTF8( filename ) ) ) && openMode & modeCreate )
	//empty or corrupt so if can, create empty doc
	{
		CreateEmptyDoc( _X("Root") );
		currentNode= documentNode->RootElement( );
		return true;
	}
	if( !bNotErr )
	{
		currentNode= NULL;
        return false;
	}
	currentNode= documentNode->RootElement( );

	return !! currentNode;
}

bool CXML::ReadFile( LPCTSTR filename )
{
	sFileName= XML_TOUTF8( filename );
	currentNode= NULL;
	if( documentNode->LoadFile( XML_TOUTF8( filename ) ) )
		currentNode= documentNode->RootElement( );
	return currentNode != NULL;
}

bool CXML::ReadString( LPCXSTR pstrInStr )
{
	Clear( );
	documentNode->Parse( pstrInStr );
	currentNode= documentNode->RootElement( );
	//TODO
	return !! currentNode;
}

bool CXML::WriteString( CString& strOut )
{
	TIXML_OSTREAM str;

	documentNode->StreamOut( &str );
	strOut= str.c_str( );
	//TODO
	return true;
}

bool CXML::WriteStringFmt( CString& strOut )
{
	TIXML_OSTREAM str;
	//little used, mostly debug so create a mem file
	//CMemFile file;
	//documentNode->Print( (FILE*)(HANDLE)file );

	documentNode->StreamOut( &str );
	strOut= str.c_str( );
	//TODO
	return true;
}

void CXML::Clear( )
{
	documentNode->Clear( );
}

// TODO fail on mem and read troubles...
//#ifdef _WINDOWS
//bool CXML::ReadFile( CFile* file )
//{
//	SLPVOID buf= malloc( ( (long)file->GetLength( ) + 2 ) * sizeof( TCHAR ) );
//	file->Read( buf, (long)file->GetLength( ) );
//	*( buf + (long)file->GetLength( ) )= '\0';
//	documentNode->Parse( buf );
//	currentNode= documentNode->RootElement( );
//	return true;
//}
//#endif

bool CXML::WriteFile( LPCTSTR filename )
{
	return documentNode->SaveFile( XML_TOUTF8( filename ) );
}

bool CXML::Close( )
{
	return documentNode->SaveFile( sFileName.c_str( ) );
}

bool CXML::DeleteCurrentNode( )
{
	if( currentNode )
	{
		//if( currentNode->ToUnknown( ) == documentNode->ToUnknown( ) )
		if( currentNode == documentNode )
			return false;
		TiXmlNode* child= currentNode;
		currentNode= currentNode->Parent( );
		return currentNode->RemoveChild( child );
	}
	return false;
}

bool CXML::ReplaceCurrentNode( XMLNODE& node )
{
	TiXmlNode* pNode= currentNode->Parent( );
	if( pNode->RemoveChild( currentNode ) )
	{
		pNode->InsertEndChild( *node.node );
		return true;
	}
	return false;

	//TiXmlNode* pChild= pNode->FirstChild( );
	//if( pNode->ToDocument( ) )//need to get past declaration
	//{
	//	for( ; pChild && ! pChild->ToElement( ); pChild= pChild->NextSibling( ) )
	//		;
	//	if( ! pChild )
	//		return false;//should never happen, but...
	//}
	//if( ! pNode->RemoveChild( pChild ) )
	//	return false;

}

// ................................................
class CXPath : protected TinyXPath::xpath_processor
{
	TiXmlNode* contextNode;
	const TiXmlNode* baseNode;
	CString xpattern;
	unsigned long count;
	HRESULT error;
	void he_er_compute_xpath( );
public:
	CXPath( const TiXmlNode* XNp_source_tree, TiXmlNode* XNp_source_context, const char * cp_xpath_expr )
		:TinyXPath::xpath_processor( XNp_source_tree, cp_xpath_expr )
		,contextNode( XNp_source_context )
		,baseNode( XNp_source_tree )
		,xpattern( cp_xpath_expr )
		,count( 0 )
	{
	// our hack........
		//can't use er_compute_xpath( ) as it sets the context node with XNp_base;
		//XNp_base is the document node in all cases.
		he_er_compute_xpath( );
	}
	virtual ~CXPath( ) { }
	unsigned long GetCount( ) { return count; }
	TiXmlNode* GetNode( unsigned long index ) { return XNp_get_xpath_node( index ); }
	HRESULT GetError( ) { return error; }
	void DumpStack( )
	{
#ifdef _DEBUG
	using namespace TinyXPath;
		UINT state= 0;
		for( int i= as_action_store.i_get_size( ) - 1; i != 0 ; --i )
		{
			xpath_construct x_const;
			int sub, var;
			TIXML_STRING str;
			as_action_store.v_get( i, (int&)x_const, sub, var, str );
			printf( "test %d action %s %d %d %s\n", i,
				TinyXPath::cp_disp_construct( x_const ), sub, var, str.c_str( ) );
		}
		printf( "\r\n" );
		
		//well, can't manipulate action stack without a hack, so:
		//TinyXPath::xpath_processor tp( baseNode, xpattern );
		
		//as_action_store.
		//v_execute_one( 
#endif
	}
};

void CXPath::he_er_compute_xpath( )
{
using namespace TinyXPath;
	try
	{
		/// This is where XEp_context is set.
		v_build_root( );
		        
		if (! XEp_root)
			// no correct initialization of the xpath_processor object
			throw execution_error (1);

		//Now we can set our own context node
		//v_set_context( contextNode->ToElement( ), false );
		v_set_context( contextNode->ToElement( ), false );

		// Decode XPath expression
		v_evaluate ();

		xs_stack.v_dump( );

		// Compute result
		v_execute_stack ();

		/// The executions stack need to contain 1 and only 1 element, otherwize it's not valid
		if (xs_stack . u_get_size () == 1)
		{
			er_result = * xs_stack . erp_top ();
			xs_stack . v_pop ();
			e_error = e_no_error;
		}
		else
		{
			expression_result er_null;
			er_result = er_null;
			e_error = e_error_stack;
		}
	}
	catch (syntax_error)
	{
		expression_result er_null;
		er_result = er_null;
		e_error = e_error_syntax;
	}
	catch (syntax_overflow)
	{
		expression_result er_null;
		er_result = er_null;
		e_error = e_error_overflow;
	}
	catch (execution_error)
	{
		expression_result er_null;
		er_result = er_null;
		e_error = e_error_execution;
	}

	v_remove_root ();
	if( er_result.e_type == TinyXPath::e_node_set )
	{
		error= ERROR_SUCCESS;
		count= er_result.nsp_get_node_set( )->u_get_nb_node_in_set( );
	}
	else
		error= ERROR_BAD_PATHNAME;
}

bool MakeStrSet( LPCXSTR p, std::vector< std::string >& set )
{
	if( *p != _T('/') )
		return false;
	LPCXSTR s= ++p;
	while( *p != _X('\0') )
	{
		//while( isalnum( *p ) )
		while( *p != _X('\0') && *p != _X('/') )
			++p;

//#ifdef UNICODE
//		std::tstring xstr( s, p - s );
//		std::string tstr( XML_TOUTF8( xstr.c_str( ) ) );
//#else
		std::string tstr( s, p - s );
//#endif
		set.push_back( tstr );
		if( *p == _T('/') )
		{
			//TODO 1/3/2017 hack.... 1/4/2017 fixed??? should be
			//ok, we commented this for regestry that ends with a '/'
			if( !isalnum( *++p ) )
				return false;
//			++p; //part of hack.....!!!!!!
		}
		else if( *p != _T('\0') )
			return false;
		s= p;
	}
	return true;
}

void CXML::CreateNodes( LPCXSTR xpattern )
{
	std::vector<std::string> set;
	VERIFY( MakeStrSet( xpattern, set ) );
	for( std::vector< std::string>::iterator it= set.begin( ); it != set.end( ); ++it )
		TRACE( _T("%s~\n"), XML_TOUNICODE( it->c_str( ) ) );
	//this is slow but will get the job done for now
	bool bIsNode= true;
	currentNode= documentNode->RootElement( );
	for( std::vector< std::string >::iterator it= set.begin( ); it != set.end( ); ++it )
	{
		if( ! it->size( ) ) //for xxx//xxx
			continue;

		std::string str( *it );
		if( it == set.begin( ) )
			str.insert( str.begin( ), _X('/') ); //gota make tiny happy.
		if( bIsNode )
		{
			CXPath xp( documentNode->RootElement( ), currentNode, str.c_str( ) );
			if( xp.GetCount( ) )
				currentNode= xp.GetNode( 0 );
			else
				bIsNode= false;
		}
		if( !bIsNode )
			currentNode= currentNode->ToElement( )->InsertEndChild( TiXmlElement( it->c_str( ) ) );
	}
}

//Node stuff
XMLNODE CXML::GetNode( LPCXSTR xpattern, bool bCanCreate_ )
{
	if( xpattern == NULL )
		return XMLNODE( this, currentNode );

	if( *xpattern == '/' && *( xpattern + 1 ) ==  '/' )
		currentNode= documentNode->RootElement( );

	CXPath xparse( documentNode->RootElement( ), currentNode, xpattern );
	lastXPathError= xparse.GetError( );
	lastXPathCount= xparse.GetCount( );

	if( xparse.GetCount( ) )
		currentNode= xparse.GetNode( 0 );

	else if( bCanCreate_ && bCanCreate && ! lastXPathCount )
	{
		CreateNodes( xpattern );
		//std::vector<CString> set;
		//VERIFY( MakeStrSet( xpattern, set ) );
		//for( std::vector<CString>::iterator it= set.begin( ); it != set.end( ); ++it )
		//	TRACE( "%s~\n", *it );
		////this is slow but will get the job done for now
		//bool bIsNode= true;
		//currentNode= documentNode->RootElement( );
		//for( std::vector<CString>::iterator it= set.begin( ); it != set.end( ); ++it )
		//{
		//	CString t( *it );
		//	if( it == set.begin( ) )
		//		t.Insert( 0, '/' ); //gota make tiny happy.
		//	if( bIsNode )
		//	{
		//		CXPath xp( documentNode->RootElement( ), currentNode, t );
		//		if( xp.GetCount( ) )
		//			currentNode= xp.GetNode( 0 );
		//		else
		//			bIsNode= false;
		//	}
		//	if( !bIsNode )
		//		currentNode= currentNode->ToElement( )->InsertEndChild( TiXmlElement( *it ) );
		//}
	}
	else
		return XMLNODE( this, NULL );

#ifdef _DEBUG
	//Debug warning as XNODE can't return a set.
	{ if( xparse.GetCount( ) > 1 ) TRACE( _T("Warning: 'GetNode' found multple instances from xpattern: '%s'\n)"), XML_TOUNICODE( xpattern ) ); }
#endif
	
	return XMLNODE( this, currentNode );
}

XMLNODESET CXML::GetNodeSet( LPCXSTR xpattern )
{
	XMLNODESET set( this );

	if( xpattern == NULL )
		return set;

	CXPath xparse( documentNode->RootElement( ), currentNode, xpattern );
	lastXPathError= xparse.GetError( );
	lastXPathCount= xparse.GetCount( );

	for( unsigned long i= 0; i < xparse.GetCount( ); ++i )
		set.set.push_back( xparse.GetNode( i ) );

	return set;
}

bool CXML::SetCurrentNode( XMLNODE& aNode )
{
	TiXmlNode* it= currentNode->FirstChildElement( );
	for( ; it; it= it->NextSiblingElement( ) )
		if( it == aNode.node )
		{
			currentNode= it;
			return true;
		}
	return false;
}

bool CXML::SetCurrentNode( LPCXSTR xpattern )
{
	if( xpattern == NULL )
	{
		currentNode= documentNode->FirstChildElement( );
		return true;
	}
	CXPath xparse( documentNode->RootElement( ), currentNode, xpattern );
	lastXPathError= xparse.GetError( );
	lastXPathCount= xparse.GetCount( );

	if( xparse.GetCount( ) )
	{
		currentNode= xparse.GetNode( 0 );
		return true;
	}
	else if( bCanCreate )
	{
		CreateNodes( xpattern );
	}
	return false;
}

bool CXML::UpNode( )
{
	if( currentNode->ToElement( ) == documentNode->RootElement( ) )
		return false;
	currentNode= currentNode->Parent( );
	return true;
}

//Return values may have been: existed, created, failed to create
bool CXML::SetElementValue( LPCXSTR tag, LPCTSTR value )
{
	TiXmlNode* node= NULL;
	if( currentNode )
		node= currentNode->FirstChild( tag );

	if( node != NULL )
	{
		TiXmlNode* it= node->FirstChild( );
		for( ; it; it= it->NextSibling( ) )
			if( it->Type( ) == TiXmlNode::TEXT )
			{
				it->SetValue( XML_TOUTF8( value ) );
				break;
			}
		if( it == NULL )
			node->InsertEndChild( TiXmlText( XML_TOUTF8( value ) ) );

		currentChild= node;
		return true;
	}
	else
	{
		TiXmlElement i( tag );
		i.InsertEndChild( TiXmlText( XML_TOUTF8( value ) ) );
		currentChild= currentNode->InsertEndChild( i );
		return false;
	}
}

#ifdef _WINDOWS
bool CXML::SetElementValue( LPCXSTR tagName, POINT value )
{
	CString t;
	t.Format( _T("%x,%x"), value.x, value.y );
	return SetElementValue( tagName, t );
}

bool CXML::SetElementValue( LPCXSTR tagName, COLORREF value )
{
	CString t;
	t.Format( _T("%d,%d,%d"), GetRValue( value ), GetGValue( value ), GetBValue( value ) );
	return SetElementValue( tagName, t );
}

bool CXML::SetElementValue( LPCXSTR tagName, RECT value )
{
	CString t;
	t.Format( _T("%x,%x,%x,%x"), value.top, value.left, value.bottom, value.right );
	return SetElementValue( tagName, t );
}

bool CXML::SetElementValue( LPCXSTR tagName, LPBYTE value, DWORD count )
{
	CString strT;
	for( size_t i= 0; i < count; ++i )
	{
		CString strI;
		strI.Format( _T("%02x,"), value[ i ] );
		strT+= strI;
	}
	if( count )
		strT.Delete( strT.GetLength( ) - 1 );

	return SetElementValue( tagName, strT );
}

//bool CXML::SetElementValue( LPCXSTR tagName, LOGFONT& value )
//{
//	XMLNODE node= GetElement( tagName );
//	node.SetElementValue( _X("lfHeight"), value.lfHeight );
//	node.SetElementValue( _X("lfWidth"), value.lfWidth );
//	node.SetElementValue( _X("lfEscapement"), value.lfEscapement );
//	node.SetElementValue( _X("lfItalic"), !! value.lfItalic );
//	node.SetElementValue( _X("lfUnderline"), !! value.lfUnderline );
//	node.SetElementValue( _X("lfStrikeOut"), !! value.lfStrikeOut );
//	node.SetElementValue( _X("lfCharSet"), (long)value.lfCharSet );
//	node.SetElementValue( _X("lfOutPrecision"), (long)value.lfOutPrecision );
//	node.SetElementValue( _X("lfClipPrecision"), (long)value.lfClipPrecision );
//	node.SetElementValue( _X("lfQuality"), (long)value.lfQuality );
//	node.SetElementValue( _X("lfPitchAndFamily"), (long)value.lfPitchAndFamily );
//	node.SetElementValue( _X("lfFaceName"), value.lfFaceName );
//
//	return true;
//}

#endif //_WINDOWS

// .....................................................
XMLNODE CXML::GetElementByAtribute( LPCXSTR tag, LPCXSTR atr, LPCTSTR value )
{
	if( currentNode )
		for( TiXmlNode* e= currentNode->FirstChild( tag ); e; e= e->NextSibling( tag ) )
		{
			LPCTSTR s= XML_TOUNICODE( e->ToElement( )->Attribute( atr ) );
			if( s && !_tcscmp( value, s ) )
				return XMLNODE( this, e );
		}
	//else
		return XMLNODE( );
}

XMLNODE CXML::GetElement( LPCXSTR tag, bool bDontCreate )
{
	TiXmlNode* node= NULL;
	if( currentNode )
		node= currentNode->FirstChild( tag );

	if( node != NULL )
		return XMLNODE( this, node );

	else if( bCanCreate && ! bDontCreate )
	{
		VERIFY( node= currentNode->InsertEndChild( TiXmlElement( tag ) ) );
		return XMLNODE( this, node );
	}
	return XMLNODE( this, NULL );
}

bool CXML::GetElementValue( LPCXSTR tag, std::tstring& value, LPCTSTR defaultVal )
{
	CString strT( value.c_str( ) );
	return GetElementValue( tag, strT, defaultVal );
}

bool CXML::GetElementValue( LPCXSTR tag, CString& value, LPCTSTR defaultVal )
{
	TiXmlNode* node= NULL;
	if( currentNode )
		node= currentNode->FirstChild( tag );

	if( node != NULL )
	{
		if( node->FirstChild( ) == NULL )
		{
			if( defaultVal != NULL  && _tcslen( defaultVal )  )
				node->InsertEndChild( TiXmlElement( XML_TOUTF8( defaultVal ) ) );
			else
				value.Empty( );

			return true;

			value= defaultVal; //ok to set CString with NULL;
		}
		else
			value= node->FirstChild( )->Value( );

		currentChild= node;
		return true;
	}
	else if( bCanCreate )
	{
		VERIFY( node= currentNode->InsertEndChild( TiXmlElement( tag ) ) );
		if( defaultVal != NULL  && _tcslen( defaultVal )  )
			node->InsertEndChild( TiXmlText( XML_TOUTF8( defaultVal ) ) );
		value= defaultVal;
		currentChild= node;
		return true;
	}
	else
		value= defaultVal;

	return false;
}

bool CXML::GetElementValue( LPCXSTR tag, bool& value, bool defaultVal )
{
	CString str;
	bool result= GetElementValue( tag, str, defaultVal ? _T("true") : _T("false") );
	value= str == _T("true");
	return result;
}

bool CXML::GetElementValue( LPCXSTR tag, long& value, long defaultVal )
{
	CString str;
	str.Format( _T("%d"), defaultVal );
	bool result= GetElementValue( tag, str, str );
	value= _tstoi( str );
	return result;
}

bool CXML::GetElementValue( LPCXSTR tag, double& value, double defaultVal )
{
	CString str;
	str.Format( _T("%f"), defaultVal );
	bool result= GetElementValue( tag, str, str );
	value= _tstof( str );
	return result;
}

#if 1
//#ifdef _WINDOWS
long GetStrValXML( LPCTSTR& p, long base= 16 ) //unsigned long
{
	TCHAR* e;
	long result= _tcstoul( p, &e, base );
	p= e + ( *e == '\0' ? 0 : 1 );
	return result;
}

bool CXML::GetElementValue( LPCXSTR tag, COleCurrency& value )
{
	CString str;
	bool bTest= GetElementValue( tag, str );
	if( !str.IsEmpty( ) )
		value.ParseCurrency( str );
	if( value.m_status == COleCurrency::invalid )
		value= COleCurrency( 0, 0 );
	return bTest;
}

bool CXML::GetElementValue( LPCXSTR tag, COleDateTime& value )
{
	CString str;
	bool bTest= GetElementValue( tag, str );
	value.ParseDateTime( str );
	return bTest;
}

bool CXML::GetElementValue( LPCXSTR tag, POINT& value, POINT defaultVal )
{
	int pos= 0;
	bool bTest;
	CString i, t;
	i.Format( _T("%x,%x"), value.x, value.y );
	if( !( bTest= GetElementValue( tag, t, i ) ) )
		return false;
	LPCTSTR p= t.GetBuffer( );
	value.x= GetStrValXML( p );
	value.y= GetStrValXML( p );
	return true;
}

bool CXML::GetElementValue( LPCXSTR tag, COLORREF& value, COLORREF defaultVal )
{
	int pos= 0;
	bool bTest;
	CString def;
	CString strT;
	def.Format( _T("%d,%d,%d"), GetRValue( defaultVal ), GetGValue( defaultVal ), GetBValue( defaultVal ) );
	if( !( bTest= GetElementValue( tag, strT, def ) ) )
		return false;

	LPCTSTR p= strT.GetBuffer( );
	long r= GetStrValXML( p, 10 );
	long g= GetStrValXML( p, 10 );
	long b= GetStrValXML( p, 10 );
	value= RGB( r, g, b );
	return true;
}

bool CXML::GetElementValue( LPCXSTR tag, RECT& value, RECT defaultVal )
{
	int pos= 0;
	bool bTest;
	CString i, t;
	i.Format( _T("%x,%x,%x,%x"), value.top, value.left, value.bottom, value.right );
	if( !( bTest= GetElementValue( tag, t, i ) ) )
		return false;
	LPCTSTR p= t.GetBuffer( );
	value.top= GetStrValXML( p );
	value.left= GetStrValXML( p );
	value.bottom= GetStrValXML( p );
	value.right= GetStrValXML( p );
	return true;
}

bool CXML::GetElementValue( LPCXSTR tag, LPBYTE* pValue, DWORD* pCount )
{
	CString strT;
	if( ! GetElementValue( tag, strT ) )
		return false;
	LPCTSTR p= strT.GetBuffer( );
	LPBYTE pB= new BYTE[ 32000 ];
	size_t i= 0;
	for( ; *p ; ++i )
		pB[ i ]= (BYTE)GetStrValXML( p );
	*pValue= pB;
	*pCount= i;
	return true;
}

//bool CXML::GetElementValue( LPCXSTR tagName, LOGFONT& value, LOGFONT& defaultVal )
//{
//	XMLNODE node= GetElement( tagName );
//	node.GetElementValue( _X("lfHeight"), value.lfHeight, defaultVal.lfHeight );
//	node.GetElementValue( _X("lfWidth"), value.lfWidth, defaultVal.lfWidth );
//	node.GetElementValue( _X("lfEscapement"), value.lfEscapement, defaultVal.lfEscapement );
//	bool bTemp;
//	node.GetElementValue( _X("lfItalic"), bTemp, !! defaultVal.lfItalic );
//	value.lfItalic= bTemp;
//	node.GetElementValue( _X("lfUnderline"), bTemp, !! defaultVal.lfUnderline );
//	value.lfUnderline= bTemp;
//	node.GetElementValue( _X("lfStrikeOut"), bTemp, !! defaultVal.lfStrikeOut );
//	value.lfStrikeOut= bTemp;
//	long lTemp;
//	node.GetElementValue( _X("lfCharSet"), lTemp, defaultVal.lfCharSet );
//	defaultVal.lfCharSet= (BYTE)lTemp;
//	node.GetElementValue( _X("lfOutPrecision"), lTemp, defaultVal.lfOutPrecision );
//	defaultVal.lfOutPrecision= (BYTE)lTemp;
//	node.GetElementValue( _X("lfClipPrecision"), lTemp, defaultVal.lfClipPrecision );
//	defaultVal.lfClipPrecision= (BYTE)lTemp;
//	node.GetElementValue( _X("lfQuality"), lTemp, defaultVal.lfQuality );
//	defaultVal.lfQuality= (BYTE)lTemp;
//	node.GetElementValue( _X("lfPitchAndFamily"), lTemp, defaultVal.lfPitchAndFamily );
//	defaultVal.lfPitchAndFamily= (BYTE)lTemp;
//	CString strT;
//	node.GetElementValue( _X("lfFaceName"), strT, defaultVal.lfFaceName );
//	
//	strcpy_s( value.lfFaceName, sizeof( defaultVal.lfFaceName ), strT ); 
//
//	return true;
//}
#endif //_WINDOWS
