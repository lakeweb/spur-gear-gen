
#include "stdafx.h"
#include "Geometry.h"
#include "objects.h"
#include "shared.h"
#include "Drawing.h"
#include "sgv.h"
#include <sstream>

// ......................................................................
void svg_stream( std::ostream& os, const SP_BaseItem& in )
{
	typecase( *in,
		[ & ]( PointItem& pa ) {
		os << " " << pa.get_bg_point( ).get_x( ) << " " << pa.get_bg_point( ).get_y( );
	},
		[ & ]( LineItem& pa ) {
		os << " " << pa.get_bg_line( ).first.get_x( ) << " "
			<< pa.get_bg_line( ).first.get_y( ) << "  "
			<< pa.get_bg_line( ).second.get_x( ) << " "
			<< pa.get_bg_line( ).second.get_y( )
			;
	},
		[ & ]( const ArcItem& pa ) {
		double rad= bg::distance( pa.get_o( ), pa.get_a( ) );
		os << "M"
			<< " " << pa.get_a( ).get_x( )
			<< " " << pa.get_a( ).get_y( )
			<< " A "
			<< " " << rad
			<< " " << rad
			<< " 0 0 1"//					<-- TODO!!!
			<< " " << pa.get_b( ).get_x( )
			<< " " << pa.get_b( ).get_y( )
			<< " M "
			;
	},
		[ & ]( const RectItem& pa ) {
	},
		[ & ]( const EllipesItem& pa ) {
	},
		[ & ]( const BezierCubeItem& pa ) {
	},
		[ & ]( const PolyItem& pa ) {
		for( auto l : pa )
			os << " " << l.get_x( ) << " " << l.get_y( ) << " ";
	});
}

// ......................................................................
void SVG_WriteFile( bfs::path& path, DrawingObects& draw )
{
	CXML xml;
	xml.Open( path.wstring( ).c_str( ), "svg", CXML::modeProgFile );
	xml.Clear( );
	xml.CreateEmptyDoc( "svg" );
	xml.SetDOCTYPE( L"!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"" );

	XMLNODE svg= xml.GetCurrentNode( );
	svg.SetElementValue( "metadata", L"spur_gear_gen Copyright: Highlands Electronics 1996-2017" );
	svg.SetAttribute( "width", L"40cm" );
	svg.SetAttribute( "height", L"40cm" );
	svg.SetAttribute( "viewBox",	L"-1 -1 2 2" );
	svg.SetAttribute( "xmlns",		L"http://www.w3.org/2000/svg" );
	svg.SetAttribute( "version",	L"1.1" );
	svg.SetElementValue( "title",	L"spur gear gen" );

	std::ostringstream os;

	os << "M ";
	for( auto it : draw )
		for( auto im : it )
			os << svg_obj( im ) << " ";

	std::wstring buf( utf8_to_uni( ( os.str( ).c_str( ) ) ) );

	XMLNODE node= svg.AddChild( "path" );
	node.SetAttribute( "stroke-width", L".01" );
	node.SetAttribute( "stroke", L"black" );
	node.SetAttribute( "fill", L"none" );
	node.SetAttribute( "d", buf.c_str( ) );

	//os.str("");
	//os.clear( );
	//node= svg.AddChild( "circle" );
	////<circle cx="10" cy="10" r="2" fill="red"/>
	//node.SetAttribute( "cx", L".0" ); 
	//node.SetAttribute( "cy", L".0" ); 
	//node.SetAttribute( "r", L".1" ); 
	//node.SetAttribute( "fill", L"blue" ); 

	//	node.SetAttribute( "fill", L"none" );

	xml.Close( );
}

