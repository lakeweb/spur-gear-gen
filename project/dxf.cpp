

#include "stdafx.h"
#include "shared.h"
#include "Geometry.h"
#include "objects.h"
#include "Drawing.h"
#include "dxf.h"
#include <dxflib/dl_dxf.h>
#include <dxflib/dl_writer_ascii.h>


// ..........................................................................
bool StartDXF_write( DL_Dxf* dxf, DL_WriterA* dw )
{
	dxf->writeHeader(*dw);
	// int variable:

	// 1 inches 4 millimeters
	dw->dxfString(9, "$INSUNITS");
	dw->dxfInt(70, 1);

	// real (double, float) variable:
	dw->dxfString(9, "$DIMEXE");
	dw->dxfReal(40, 1.25);
	// string variable:
	dw->dxfString(9, "$TEXTSTYLE");
	dw->dxfString(7, "Standard");
	// vector variable:
	dw->dxfString(9, "$LIMMIN");
	dw->dxfReal(10, 0.0);
	dw->dxfReal(20, 0.0);
	dw->sectionEnd();

	//....................
	dw->sectionTables();
	dxf->writeVPort(*dw);
	dw->tableLinetypes(25);
	dxf->writeLinetype(*dw,
		DL_LinetypeData("CONTINUOUS", "disc", 0, 0, 0 ));
	//dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", 0));
	//dxf->writeLinetype(*dw,
	//	DL_LinetypeData("ACAD_ISO02W100", 0));
	//dxf->writeLinetype(*dw,
	//	DL_LinetypeData("ACAD_ISO03W100", 0));
	//dxf->writeLinetype(*dw,
	//	DL_LinetypeData("ACAD_ISO04W100", 0));
	//dxf->writeLinetype(*dw,
	//	DL_LinetypeData("ACAD_ISO05W100", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("BORDER", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("BORDER2", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("BORDERX2", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("CENTER", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("CENTER2", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("CENTERX2", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DASHDOT", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DASHDOT2", 0));
	//dxf->writeLinetype(*dw,
	//	DL_LinetypeData("DASHDOTX2", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DASHED", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DASHED2", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DASHEDX2", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DIVIDE", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DIVIDE2", 0));
	//dxf->writeLinetype(*dw,
	//	DL_LinetypeData("DIVIDEX2", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DOT", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DOT2", 0));
	//dxf->writeLinetype(*dw, DL_LinetypeData("DOTX2", 0));
	dw->tableEnd();


	//....................
	int numberOfLayers = 3;
	dw->tableLayers(numberOfLayers);

	dxf->writeLayer(*dw,
		DL_LayerData("0", 0),
		DL_Attributes(
			std::string(""),      // leave empty
			DL_Codes::black,        // default color
			100,                  // default width
			"CONTINUOUS", 1));       // default line style

	//dxf->writeLayer(*dw,
	//	DL_LayerData("mainlayer", 0),
	//	DL_Attributes(
	//		std::string(""),
	//		DL_Codes::red,
	//		100,
	//		"CONTINUOUS",1));

	//dxf->writeLayer(*dw,
	//	DL_LayerData("anotherlayer", 0),
	//	DL_Attributes(
	//		std::string(""),
	//		DL_Codes::black,
	//		100,
	//		"CONTINUOUS",1));

	dw->tableEnd();

	//....................
	dw->tableStyle( 1 );
	dxf->writeStyle(*dw, DL_StyleData("standard", 0, 2.5, 1.0, 0.0, 0, 2.5, "standard", ""));
	dw->tableEnd( );

	dxf->writeView(*dw);
	dxf->writeUcs(*dw);

	dw->tableAppid(1);
	dw->tableAppidEntry(0x12);
	dw->dxfString(2, "ACAD");
	dw->dxfInt(70, 0);
	dw->tableEnd();

	//....................
	dxf->writeDimStyle(*dw,
		1,
		1,
		1,
		1,
		1);
	dxf->writeBlockRecord(*dw);
	dxf->writeBlockRecord(*dw, "myblock1");
	dxf->writeBlockRecord(*dw, "myblock2");
	dw->tableEnd();
	dw->sectionEnd();


	//....................
	dw->sectionBlocks();

	dxf->writeBlock(*dw,
		DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Model_Space");

	dxf->writeBlock(*dw,
		DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Paper_Space");

	//dxf->writeBlock(*dw,
	//	DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
	//dxf->writeEndBlock(*dw, "*Paper_Space0");

	dxf->writeBlock(*dw,
		DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
	// ...
	// write block entities e.g. with dxf->writeLine(), ..
	// ...
	dxf->writeEndBlock(*dw, "myblock1");

	dxf->writeBlock(*dw,
		DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
	// ...
	// write block entities e.g. with dxf->writeLine(), ..
	// ...
	dxf->writeEndBlock(*dw, "myblock2");

	dw->sectionEnd( );

	return true;
}

bool EndDXF_write( DL_Dxf* dxf, DL_WriterA* dw )
{
	//....................
	dxf->writeObjects( *dw );
	dxf->writeObjectsEnd( *dw );

	dw->dxfEOF( );
	dw->close( );

	delete dw;
	delete dxf;
	dw= NULL;
	dxf= NULL;
	return true;
}

void DXF_Stream( std::wofstream& os, DrawingObects& drawobj, DL_Dxf* dxf, DL_WriterA* dw );
void DXF_WriteFile( bfs::path& pathname,  DrawingObects& drawobj )
{
	std::wofstream output( bfs::path( pathname / _T(".log") ).wstring( ) );
	DL_Dxf* dxf= new DL_Dxf( );
	DL_Codes::version exportVersion= DL_Codes::AC1015;
	DL_WriterA* dw = dxf->out( pathname.string( ).c_str( ), exportVersion );
	StartDXF_write( dxf, dw );
	dw->sectionEntities( );
	DXF_Stream( output, drawobj, dxf, dw );
	//this will deete dxf and dw....
	EndDXF_write( dxf, dw );
}

void DXF_Stream( std::wofstream& os, DrawingObects& drawobj, DL_Dxf* dxf, DL_WriterA* dw )
{
	//DL_Codes::version exportVersion= DL_Codes::AC1015;

	//	DL_WriterA* dw = dxf->out( uni_t0_utf8( lpszPathName ), exportVersion );

//	DL_WriterA* dw = dxf->out( "test.dxf", exportVersion );

	//........................
	//dw->sectionEntities( );

	for( auto groups : drawobj.get_set( ) )
	{
		//groups of items are on a group layer
		//test
		//std::string layer( test_layer_names[ groups.layer
		for( auto item : groups )
			//if( item->GetMill( ) )
		{
			//TODO use typecase(...) here...
			const std::type_info& ti1= typeid( item );
			//if( ! item->GetMill( ) )
			//	continue;
			{
				const ArcItem* p=	dynamic_cast< const ArcItem* >( item.get( ) );
				if( p )
				{
					DL_ArcData ad( p->get_o( ).get_x( ),
						p->get_o( ).get_y( ),
						0,
						p->get_rad( ),
						p->get_start_ang( ) * ( 180 / PI ),
						p->get_end_ang( ) * ( 180 / PI )
						//atan(  p->get_a( ).get_y( ) /  p->get_a( ).get_x( ) ) * ( 180 / PI ),
						//atan(  p->get_b( ).get_y( ) /  p->get_b( ).get_x( ) ) * ( 180 / PI )
						);
					dxf->writeArc( *dw, ad,
						DL_Attributes("mainlayer", 256, 1, "BYLAYER",1) );

					os << SET_OSFLOAT << "box: " << p->get_box( )
						<< " a: " << p->get_a( )
						<< " b: " << p->get_b( )
						<< " rad: " << p->get_rad( )
						<< "\n   st ang: " << p->get_start_ang( ) * ( 180 / PI )
						<< " end ang: " << p->get_end_ang( ) * ( 180 / PI )
						<< " a_o_dif: " << p->get_a( ).get_y( ) - p->get_o( ).get_y( )
						<< " b_o_dif: " << p->get_b( ).get_y( ) - p->get_o( ).get_y( )
						<< std::endl;
					os << SET_OSFLOAT << "test tan a.y/a.x: " << p->get_a( ).get_y( ) /  p->get_a( ).get_x( )
						<< "  test tan b.y/b.x: " << p->get_b( ).get_y( ) /  p->get_b( ).get_x( )
						<< "  atan a: " << p->get_start_ang( ) * ( 180 / PI )
						<< "  atan b: " << p->get_end_ang( ) * ( 180 / PI )
						<< std::endl << std::endl;

					continue;
				}
			}

			{
				const LineItem* p=	dynamic_cast< const LineItem* >( item.get( ) );

				if( p )
				{
					dxf->writeLine(
						*dw,
						DL_LineData( p->get_bg_line( ).first.get_x( ),   // start point
							p->get_bg_line( ).first.get_y( ),
							0.0,
							p->get_bg_line( ).second.get_x( ),   // end point
							p->get_bg_line( ).second.get_y( ),   // end point
							0.0),
						DL_Attributes("mainlayer", 256, 1, "BYLAYER",2));
					continue;
				}
			}
			{
				const BezierCubeItem* p= dynamic_cast< const BezierCubeItem* >( item.get( ) );

				if( p )
				{
					size_t degree= 3;
					size_t ctl_pts= 4;
					size_t knots= ctl_pts + degree + 1;
					dxf->writeSpline(
						*dw,
						DL_SplineData( 3, 8, 4, 0, 8 ),
						DL_Attributes("mainlayer", 256, 1, "BYLAYER",2)
						);

					//bg_point kn;
					//for( size_t i= 1; i <= knots; ++i )
					//{
					//	if( i < degree )
					//		kn.set_xy( 0 );
					//	else if( i < knots - degree )
					//		kn.set_xy( 1.0 / ( knots - 2 * degree + 2 ) * ( i - degree + 1 ) );
					//	else
					//		kn.set_xy( 1 );
					//	dxf->writeKnot( *dw, DL_KnotData( kn.get_x( ) ) );
					//}
					bg_point kn;
					for( size_t i= 1; i <= knots; ++i )
					{
						dxf->writeKnot( *dw, DL_KnotData( ( i + 3 < knots ? 0 : 1 ) ) );
					}
					// write spline control points:
					dxf->writeControlPoint( *dw,
						DL_ControlPointData( p->get_point0( ).get_x( ),
							p->get_point0( ).get_y( ),
							0.0, 1.0
							)
						);
					dxf->writeControlPoint( *dw,
						DL_ControlPointData( p->get_point1( ).get_x( ),
							p->get_point1( ).get_y( ),
							0.0, 1.0
							)
						);
					dxf->writeControlPoint( *dw,
						DL_ControlPointData( p->get_point3( ).get_x( ),
							p->get_point3( ).get_y( ),
							0.0, 1.0
							)
						);
					dxf->writeControlPoint( *dw,
						DL_ControlPointData( p->get_point2( ).get_x( ),
							p->get_point2( ).get_y( ),
							0.0, 1.0
							)
						);
					continue;
				}
			}

			{
				const PolyItem* p=	dynamic_cast< const PolyItem* >( item.get( ) );

				if( p )
				{// <true for closed polyline, false for open polylines>
					dxf->writePolyline(
						*dw,
						DL_PolylineData( p->size( ), 0, 0, 0 ), 
						DL_Attributes("mainlayer", 256, 1, "BYLAYER",2) );

					for( auto item : *p )
						dxf->writeVertex( *dw, DL_VertexData( item.get_x( ), item.get_y( ), 0 ) );

					dxf->writePolylineEnd( *dw );
					continue;
				}
			}
		}
	}
	//........................
	dw->sectionEnd( );

}

/*
DL_Dxf* dxf= new DL_Dxf( );
DL_Codes::version exportVersion= DL_Codes::AC1015;
//	DL_WriterA* dw = dxf->out( uni_t0_utf8( lpszPathName ), exportVersion );
DL_WriterA* dw = dxf->out( "test.dxf", exportVersion );
StartDXF_write( dxf, dw );

std::wofstream os( "test.log", std::ios::out );

//........................
dw->sectionEntities( );

for( auto groups : drawobj.get_set( ) )
{
	//groups of items are on a group layer
	//test
	//std::string layer( test_layer_names[ groups.layer
	for( auto item : groups )
		//if( item->GetMill( ) )
	{
		//auto item= *ittem;
		const std::type_info& ti1= typeid( item );
		//if( ! item->GetMill( ) )
		//	continue;
		{
			const ArcItem* p=	dynamic_cast< const ArcItem* >( item.get( ) );
			if( p )
			{
				DL_ArcData ad( p->get_o( ).get_x( ),
					p->get_o( ).get_y( ),
					0,
					p->get_rad( ),
					p->get_start_ang( ) * ( 180 / PI ),
					p->get_end_ang( ) * ( 180 / PI )
					//atan(  p->get_a( ).get_y( ) /  p->get_a( ).get_x( ) ) * ( 180 / PI ),
					//atan(  p->get_b( ).get_y( ) /  p->get_b( ).get_x( ) ) * ( 180 / PI )
					);
				dxf->writeArc( *dw, ad,
					DL_Attributes("mainlayer", 256, 1, "BYLAYER",1) );

				os << SET_OSFLOAT << "box: " << p->get_box( )
					<< " a: " << p->get_a( )
					<< " b: " << p->get_b( )
					<< " rad: " << p->get_rad( )
					<< "\n   st ang: " << p->get_start_ang( ) * ( 180 / PI )
					<< " end ang: " << p->get_end_ang( ) * ( 180 / PI )
					<< " a_o_dif: " << p->get_a( ).get_y( ) - p->get_o( ).get_y( )
					<< " b_o_dif: " << p->get_b( ).get_y( ) - p->get_o( ).get_y( )
					<< std::endl;
				os << SET_OSFLOAT << "test tan a.y/a.x: " << p->get_a( ).get_y( ) /  p->get_a( ).get_x( )
					<< "  test tan b.y/b.x: " << p->get_b( ).get_y( ) /  p->get_b( ).get_x( )
					<< "  atan a: " << p->get_start_ang( ) * ( 180 / PI )
					<< "  atan b: " << p->get_end_ang( ) * ( 180 / PI )
					<< std::endl << std::endl;

				continue;
			}
			if( p )
				;
			//TRACE( "an arc\n" );
		}

		{
			const LineItem* p=	dynamic_cast< const LineItem* >( item.get( ) );

			if( p )
			{
				dxf->writeLine(
					*dw,
					DL_LineData( p->get_bg_line( ).first.get_x( ),   // start point
						p->get_bg_line( ).first.get_y( ),
						0.0,
						p->get_bg_line( ).second.get_x( ),   // end point
						p->get_bg_line( ).second.get_y( ),   // end point
						0.0),
					DL_Attributes("mainlayer", 256, 1, "BYLAYER",2));
				continue;
			}
		}
		{
			const BezierCubeItem* p= dynamic_cast< const BezierCubeItem* >( item.get( ) );

			if( p )
			{
				size_t degree= 3;
				size_t ctl_pts= 4;
				size_t knots= ctl_pts + degree + 1;
				dxf->writeSpline(
					*dw,
					DL_SplineData( 3, 8, 4, 0, 8 ),
					DL_Attributes("mainlayer", 256, 1, "BYLAYER",2)
					);

				//bg_point kn;
				//for( size_t i= 1; i <= knots; ++i )
				//{
				//	if( i < degree )
				//		kn.set_xy( 0 );
				//	else if( i < knots - degree )
				//		kn.set_xy( 1.0 / ( knots - 2 * degree + 2 ) * ( i - degree + 1 ) );
				//	else
				//		kn.set_xy( 1 );
				//	dxf->writeKnot( *dw, DL_KnotData( kn.get_x( ) ) );
				//}
				bg_point kn;
				for( size_t i= 1; i <= knots; ++i )
				{
					dxf->writeKnot( *dw, DL_KnotData( ( i + 3 < knots ? 0 : 1 ) ) );
				}
				// write spline control points:
				dxf->writeControlPoint( *dw,
					DL_ControlPointData( p->get_point0( ).get_x( ),
						p->get_point0( ).get_y( ),
						0.0, 1.0
						)
					);
				dxf->writeControlPoint( *dw,
					DL_ControlPointData( p->get_point1( ).get_x( ),
						p->get_point1( ).get_y( ),
						0.0, 1.0
						)
					);
				dxf->writeControlPoint( *dw,
					DL_ControlPointData( p->get_point3( ).get_x( ),
						p->get_point3( ).get_y( ),
						0.0, 1.0
						)
					);
				dxf->writeControlPoint( *dw,
					DL_ControlPointData( p->get_point2( ).get_x( ),
						p->get_point2( ).get_y( ),
						0.0, 1.0
						)
					);
				continue;
			}
		}

		{
			const PolyItem* p=	dynamic_cast< const PolyItem* >( item.get( ) );

			if( p )
			{// <true for closed polyline, false for open polylines>
				dxf->writePolyline(
					*dw,
					DL_PolylineData( p->size( ), 0, 0, 0 ), 
					DL_Attributes("mainlayer", 256, 1, "BYLAYER",2) );

				for( auto item : *p )
					dxf->writeVertex( *dw, DL_VertexData( item.get_x( ), item.get_y( ), 0 ) );

				dxf->writePolylineEnd( *dw );
				continue;
			}
		}
	}
}
//........................
dw->sectionEnd( );

//this will deete dxf and dw....
EndDXF_write( dxf, dw );
*/

