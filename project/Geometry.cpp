
#include "stdafx.h"
#include "debug.h"
#ifdef TEST_PARSER
#include <loc_globals.h>
#endif
#include "Geometry.h"
//#include "board.h"


/*
bg_box GetBounds( SP_SSP_Node nodes )
{
	auto it= nodes.get( )->begin( );
	if( it == nodes.get( )->end( ) )
		return bg_box( );

	bg_box box( it->get( )->pt_min( ), it->get( )->pt_max( ) );
	GBTRACE( "%x pt_min x: %d y: %d\n", it->get( ), it->get( )->pt_min( ).get< 0 >( ), it->get( )->pt_min( ).get< 1 >( ) );
	GBTRACE( "%x pt_max x: %d y: %d\n", it->get( ), it->get( )->pt_max( ).get< 0 >( ), it->get( )->pt_max( ).get< 1 >( ) );
	for( ++it; it != nodes.get( )->end( ); ++it )
	{
		bg::expand( box, it->get( )->pt_min( ) );
		GBTRACE( "%x pt_min x: %d y: %d\n", it->get( ), it->get( )->pt_min( ).get< 0 >( ), it->get( )->pt_min( ).get< 1 >( ) );
		bg::expand( box, it->get( )->pt_max( ) );
		GBTRACE( "%x pt_max x: %d y: %d\n", it->get( ), it->get( )->pt_max( ).get< 0 >( ), it->get( )->pt_max( ).get< 1 >( ) );
	}
	GBTRACE( " box x_min: %d y_min: %d x_max: %d y_max: %d\n", box.min_corner( ).get< 0 >( ),  box.min_corner( ).get< 1 >( ),  box.max_corner( ).get< 0 >( ),  box.max_corner( ).get< 1 >( ) );
	return box;
}

*/
