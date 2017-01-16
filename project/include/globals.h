

#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef _WINDOWS
#ifdef USING_XML

#ifdef _DEBUG
#pragma comment(lib, "hexml_10d.lib")
#pragma comment(lib, "dxfd.lib")
#else
#pragma comment(lib, "hexml_10.lib")
#pragma comment(lib, "dxf.lib")
#endif
#endif

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

#endif // USE_WINDOWS_EX

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
#endif // GLOBALS_H
