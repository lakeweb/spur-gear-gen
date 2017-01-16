
#pragma once

// ..........................................................................
struct DrawExtent
{
	CRect& rectClient;
	long& of_x;
	long& of_y;
	double& zoom;
	bool bVert;
	bool bHorz;

	DrawExtent( CRect& rect, long& ix, long& iy, double& izoom )
		:rectClient( rect )
		,of_x( ix )
		,of_y( iy )
		,zoom( izoom )
		,bVert( false )
		,bHorz( false )
	{ }
	void set_all( ) { bVert= true; bHorz= true; }
	void clear_all( ) { bVert= false; bHorz= false; }
};

// ..............................................................
typedef struct _layer
{
	size_t id;
	std::wstring label;
	bool enabled;

	_layer( size_t id, const wchar_t* str, bool enabled= true )
		:id( id )
		,label( str ? str : L"" )
		,enabled( enabled )
	{ }
	bool operator < ( const _layer& a ) const { return id < a.id; }

}layer_t;

typedef std::set< layer_t > layer_set_t;

// ..............................................................
#define APP_INFO_NOTIFY ( WM_USER + 100 )
#define VIEW_INFO_NOTIFY ( WM_USER + 101 )

//for messaging between views for update notifications...........
typedef struct _info_notify
{
	enum call
	{
		output,
		layer,
	};
	bool bClear;
	call the_call;
	std::string basic_info;

	_info_notify( call which )
		:the_call( which )
		,bClear( false )
	{ }

}info_notify_t;

// ..............................................................
typedef struct _info_layer
{
	std::vector< size_t > list;

}info_layer_t;

typedef struct _dock_notify
{
	enum docker {
		layer,
	};

	docker the_docker;
	size_t info;

	_dock_notify( docker d, size_t i= 0 )
		:the_docker( d )
		,info( i )
	{ }
} dock_notify_t;

// ..............................................................
// ..............................................................
//app messaging
//#define MSG_APP_BEGIN                   32801
//#define MSG_APP_END                     32850

#define MSG_LAYER_ENABLE				( MSG_APP_BEGIN + 1 )
#define CMD_TEST						( MSG_APP_BEGIN + 2 )


//for sending notifications to CDocument Objects
//https://www.codeproject.com/articles/14706/notifying-the-document
typedef struct tagNMHDROBJECT
{
	NMHDR nmHdr;
	void* pObject; 
} NMHDROBJECT, *PNMHDROBJECT;

#define TO_RAD( x ) ( x * PI / 180 )

