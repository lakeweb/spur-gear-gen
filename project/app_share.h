#pragma once


typedef struct _profile
{
	_profile( )
		:export_type( 2 )
	{ }
	size_t export_type;

}profile_t;

//TODO this stuff will point properly in release

inline bfs::path get_program_folder( ) { return bfs::path( L".\\ProgramFiles\\" ); } // use 'program' in debug;
inline bfs::path get_user_folder( ) { return bfs::path( L".\\AppData\\" ); }

profile_t& get_user_profile( );

