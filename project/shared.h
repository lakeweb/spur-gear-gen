

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

	call the_call;
	std::string basic_info;

	_info_notify( call which )
		:the_call( which )
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



