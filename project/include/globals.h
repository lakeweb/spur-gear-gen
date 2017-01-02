

#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef _WINDOWS
//this is causing a strange memory leak!!??
#ifdef USING_XML

#ifdef _DEBUG
#pragma comment(lib, "hexml_10d.lib")
#pragma comment(lib, "dxfd.lib")
#else
#pragma comment(lib, "hexml_10.lib")
#pragma comment(lib, "dxf.lib")
#endif
#endif


//new typedef
typedef std::vector<CString> VCString;
typedef std::vector<CString>::iterator VCStringIt;
typedef std::vector<CString>::const_iterator VCStringCIt;

#define CLR_WHITE RGB( 255, 255, 255 )

//use this to add a vecotr of strings to another string
//			VCStringAdd add( strT );
//			std::for_each( vstr.begin( ), vstr.end( ), add );

struct VCStringAdd
{
	LPCTSTR pzTails;
	CString& strAddTo;
	VCStringAdd( CString& strAdd, LPCTSTR pzTails= NULL )
		:strAddTo( strAdd )
		,pzTails( pzTails )
	{ }
	void operator( ) ( const CString& sIn )
	{
		strAddTo+= sIn;
		if( pzTails )
			strAddTo+= pzTails;
	}
};

inline void VCStringAddToStr( CString& strIn, VCString& vstrIn, LPCTSTR pzTails= NULL )
{
	VCStringAdd add( strIn, pzTails );
	std::for_each( vstrIn.begin( ), vstrIn.end( ), add );
}

inline VCStringIt VCStringFindInSet( LPCTSTR pzIn, VCString& vstrIn )
{
	return std::find( vstrIn.begin( ), vstrIn.end( ), pzIn );
}

// Helper for loading 'new lined' strings into VCString
//		Will add to the end of the vector, won't clear( )!
//		returns count of strings added
//new 10/7/9 can spec delimiter, like, for commas
size_t SplitString( VCString& set, LPCTSTR pstrInLines, TCHAR del= _T('\n') );

// make string from VCString
CString MakeString( VCString& set, LPCTSTR lzDel= _T("\t") );


#ifdef USE_WINDOWS_EX
//Rich Stuff ..................................................................
class _afxRichEditStreamCookie
{
public:
	CArchive& m_ar;
	DWORD m_dwError;
	_afxRichEditStreamCookie(CArchive& ar) : m_ar(ar) {m_dwError=0;}
};

DWORD CALLBACK EditStreamCallBack( DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb );
BOOL SetRichText( CRichEditCtrl& re, char* buf, int format= SF_RTF );

// ..........................................................................
BOOL FileExceptionMessageBox( CFileException* exception= NULL, LPCTSTR addInfo= _T("") );

// ..........................................................................
/*
class CSFile : public CFile
{
public:
	CFileException* exception;
	CSFile( LPCTSTR fileName, int mode= CFile::modeRead, LPCTSTR message= NULL )
		:CFile( )
	{
		errorMessage= message;
		exception= new CFileException( );

		int result= Open( fileName, mode, exception );
		if( !result )
			FileExceptionMessageBox( exception, message );
	}
	~CSFile( ){ exception->Delete( ); }

	//CFile& operator &(CFile&) { return (CFile&)*this; }
	//CFile* operator *( CFile* ) { return (CFile*)this; }
	bool IsError( ){ return exception->m_cause != CFileException::none; }
	bool WriteString( const CString& string )
	{
		try{
			CFile::Write( string, string.GetLength( ) );
		}catch( CFileException* e ){
			FileExceptionMessageBox( e, errorMessage );
			e->Delete( );
			return false;
		}
		return true;
	}
	bool WriteBuf( const void* pBuf, UINT count )
	{
		try{
			CFile::Write( pBuf, count );
		}catch( CFileException* e )
		{
			FileExceptionMessageBox( e, errorMessage );
			e->Delete( );
			return false;
		}
		return true;
	}
	bool ReadBuf( void* pBuf, UINT count )
	{
		try{
			readCount= CFile::Read( pBuf, count );
		}catch( CFileException* e ){
			FileExceptionMessageBox( e, errorMessage );
			e->Delete( );
			return false;
		}
		return true;
	}
	ULONGLONG GetLengthBuf( )
	{
		ULONGLONG l;
		try{
			l= CFile::GetLength( );
		}catch( CFileException* e ){
			FileExceptionMessageBox( e, errorMessage );
			e->Delete( );
			return ULONGLONG(-1);
		}
		return l;
	}
	bool FlushBuf( )
	{
		try{
			CFile::Flush( );
		}catch( CFileException* e ){
			FileExceptionMessageBox( e, errorMessage );
			e->Delete( );
			return false;
		}
		return true;
	}
	bool CloseBuf( )
	{
		try{
			CFile::Close( );
		}catch( CFileException* e ){
			FileExceptionMessageBox( e, errorMessage );
			e->Delete( );
			return false;
		}
		return true;
	}
	UINT readCount;
protected:
	CString errorMessage;
};
*/
#endif // USE_WINDOWS_EX


// ..........................................................................
//A String to Type where Type is consectutive enum
/* use like:
AStrType< he_photo_state::state::Rotate >
	he_photo_state::state::strRotate( ASTRTYPE_DEF( he_photo_state::strsRotate ) );
*/
template < typename T >
class AStrType
{
private:
	size_t count;
	LPCTSTR* list;

public:
	AStrType( LPCTSTR* aList, int aCount )
	{
		ASSERT_POINTER( aList, LPCTSTR );
		list= aList;
		count= aCount;
	}
	//String to Type
	T stot( LPCTSTR name, bool bSetToZero= false )
	{
		for( size_t i= 0; i < count; ++i )
			if( !_tcsncmp( name, list[i], _tcslen( name ) ) )
				return (T)i;

		return bSetToZero ? (T)0 : (T)-1;
	}
	//Type to String
	LPCTSTR ttos( T i )
	{
		CString t;
		if( (unsigned int)i < count && (int)i >= 0 )
			return list[i];

		return _T("");
	}
	long GetSize( ) { return count; }

	bool ValidName( LPCTSTR name ) { return stot( name ) != -1; }

#ifdef _DEBUG //Used in ASSERT(..)
	bool ValidIndex( T i ) { return (unsigned int)i < count; }
#endif
};
#define countof( a ) ( sizeof( a )/ sizeof( a[0] ) )
#define ASTRTYPE_DEF( a ) (LPCTSTR*)a, countof( a )

// ...................................................................
// AStrTFlag new 8-3-7 use where type is enum as flags
/*
For use with:
enum Types {...}
AFX_STATIC_DATA const LPCTSTR lpsTypes[]= { "str", "str"...}
#define TYPES_SIZE countof( lpsTypes )

Declare in class or global
static AStrTFlag< Types > typeSet;

Define as:
AStrTFlag< Types > SomeClass::typeSet( (LPCTSTR*)lpsTypes, PROJECT_FILETYPE_SIZE );

or:
AStrTFlag< Types > aTypeSet( ASTRTYPE_DEF( lpsTypes ) );
*/

template < typename T >
class AStrTFlag
{
	std::map<long, LPCTSTR> map;
	typedef std::map<long, LPCTSTR>::iterator It;
	typedef std::pair<long, LPCTSTR> Pair;

public:
	AStrTFlag( LPCTSTR const* strTypes, long const sizeTypes )
	{
		for( long i= 0; i < sizeTypes; ++i )
		{
			//map.insert( Pair( longTypes[i], strTypes[i] ) );
			map.insert( Pair( T(i), strTypes[i] ) );
		}
	}
	CString GetFlagsStr( long inFlags )
	{
		CString strT;
		It it;
		if( ! inFlags )//check for null
		{
			it= map.find( inFlags );
			if( it != map.end( ) )
				strT= it->second;
			return strT;
		}
		long mask= 1;
		for( ; mask; mask= mask << 1 )
		{
			if( inFlags & mask ) //null ignored
			{
				it= map.find( ( inFlags & mask ) );
				if( it != map.end( ) )
				{
					if( ! strT.IsEmpty( ) )
						strT+= _T(',');
					strT+= it->second;
				}
			}
		}
		return strT;
	}
	LPCTSTR GetFlagsPointer( T inFlags )
	{
		It it;
		if( ! inFlags )//check for null
		{
			it= map.find( inFlags );
			ASSERT( it != map.end( ) );
			return it->second;
		}
		long mask= 1;
		for( ; mask; mask= mask << 1 )
		{
			if( inFlags & mask )
			{
				it= map.find( inFlags );
				ASSERT( it != map.end( ) );
				return it->second;
			}
		}
		ASSERT( false );
		return NULL;
	}
	T GetFlags( LPCTSTR pstrInFlags )
	{
		CString strT( pstrInFlags );
		long pos= 0;
		long flags= 0;
		for( ; pos != -1; )
		{
			long to= strT.Find( _T(','), pos );
			CString strE;
			if( to == -1 )
			{
				strE= strT.Right( strT.GetLength( ) - pos );
				--to;
			}
			else
				strE= strT.Mid( pos, to - pos );

			for( It it= map.begin( ); it != map.end( ); ++it )
			{
				if( strE == it->second )
				{
					flags|= it->first;
					break;
				}
			}
			pos= to + 1;
		}
		return static_cast<T>( flags );
	}
};

#endif// _WINDOWS

// ..........................................................................
//#ifdef XXX
//#ifdef UNICODE
//bool scmp_utf8_uni( const char* in1, const TCHAR* in2 )
//{
//	return false;
//}
//#define _tcscmp2 scmp_utf8_uni
//#else
//#define _tcscmp2 _tcscmp
//
//#endif
//#endif

#endif // GLOBALS_H

/*
std::string NarrowString(const std::wstring& str, const char* localeName = "C")
{
  std::string result;
  result.resize(str.size());

  std::locale loc(localeName);

  std::use_facet<std::ctype<wchar_t> >(loc).narrow(
    str.c_str(), str.c_str() + str.size(), '?',  &*result.begin());

  return result;
}
*/

