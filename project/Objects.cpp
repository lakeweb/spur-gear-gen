
#include "stdafx.h"

#include "Geometry.h"
#include "objects.h"

///................................................
// get angles, ArcItem 'a is start of arc to ArcItem 'b
// to convert we have to deal with quadrants
// (maybe it would be easier to keep arcs in this format?)
// we will continue to use radians and convert at DXF time

// ............................................................................
GEO_NUM ArcItem::get_start_ang( ) const
{
	double ta= a.get_y( ) /  a.get_x( );
	double test=  atan( ta );
	if( a.get_x( ) < o.get_x( ) )
		return atan( ta ) + PI;
	
	else if( a.get_y( ) < o.get_y( ) )//even though negative arcs work fine here, we will correct it
		return atan( ta ) + ( PI * 2 );

	return atan( ta );
}

// ............................................................................
GEO_NUM ArcItem::get_end_ang( ) const
{
	double tb= b.get_y( ) /  b.get_x( );
	if( b.get_x( ) < o.get_x( ) )
		return atan( tb ) + PI;

	else if( b.get_y( ) < o.get_y( ) )
		return atan( tb ) + ( PI * 2 );

	return atan( tb );
}

// TODO !!!!! this is off and I just can't see it !!?????
//void ArcItem::rotate( const GEO_NUM angle, const bg_point& org )
//{
//	GEO_NUM cos_v= cos( angle );
//	GEO_NUM sin_v= sin( angle );
//		
//	a.set_x( ( a.get_x( ) - org.get_x( ) ) * cos_v - ( a.get_y( ) - org.get_y( ) ) * sin_v + org.get_x( ) );
//	a.set_y( ( a.get_x( ) - org.get_x( ) ) * sin_v + ( a.get_y( ) - org.get_y( ) ) * cos_v + org.get_y( ) );
//	b.set_x( ( b.get_x( ) - org.get_x( ) ) * cos_v - ( b.get_y( ) - org.get_y( ) ) * sin_v + org.get_x( ) );
//	b.set_y( ( b.get_x( ) - org.get_x( ) ) * sin_v + ( b.get_y( ) - org.get_y( ) ) * cos_v + org.get_y( ) );
//	o.set_x( ( o.get_x( ) - org.get_x( ) ) * cos_v - ( o.get_y( ) - org.get_y( ) ) * sin_v + org.get_x( ) );
//	o.set_y( ( o.get_x( ) - org.get_x( ) ) * sin_v + ( o.get_y( ) - org.get_y( ) ) * cos_v + org.get_y( ) );
//
//}

//and this works fine, not as efficient
void ArcItem::rotate( const GEO_NUM angle, const bg_point& origin )
{
	a= bg_rotate( angle, a, origin );
	b= bg_rotate( angle, b, origin );
	o= bg_rotate( angle, o, origin );
}

//Arc operators
// ............................................................................
inline void operator += ( ArcItem& a, const bg_point& b )
{
	a.a+= b;
	a.b+= b;
	a.o+= b;
}


// ............................................................................
SP_BaseItem rotate_object( GEO_NUM angle, const SP_BaseItem& input, bg_point origin )
{
	SP_BaseItem pt;
	typecase( *input,

		[ & ]( PointItem& pa ) {
		SP_PointItem item( boost::make_shared< PointItem >( pa ) );
			item->set( bg_rotate( angle, pa.get_bg_point( ), origin ) );
		pt= item;
	},
		[ & ]( LineItem& pa ) {
		SP_LineItem item( boost::make_shared< LineItem >( pa ) );
			item->set( bg_rotate( angle, pa.get_bg_line( ), origin ) );
		pt= item;
	},
		[ & ]( const ArcItem& pa ) {
		SP_ArcItem item( boost::make_shared< ArcItem >( pa ) );
		//item->set( 
		//	bg_rotate( angle, pa.get_a( ), origin ),
		//	bg_rotate( angle, pa.get_b( ), origin ),
		//	bg_rotate( angle, pa.get_o( ), origin ) );
		item->rotate( angle, origin );
		pt= item;
	},
		[ & ]( const RectItem& pa ) {
	},
		[ & ]( const EllipesItem& pa ) {
	},
		[ & ]( const BezierCubeItem& pa ) {
	},
		[ & ]( const PolyItem& pa ) {
		auto poly= SP_PolyItem( boost::make_shared< PolyItem >( ) );
		for( auto it : pa )
			*poly+= bg_rotate( angle, it, origin );
		pt= poly;
	});
	return pt;
}

// ............................................................................
SP_BaseItem transform_object( const SP_BaseItem& input, bg_point offset )
{
	//origin not used yet. the world 0, 0 is used for now....
	SP_BaseItem pt;
	typecase( *input,

		[ & ]( PointItem& pa ) {
		bg_point tpt( pa.get_bg_point( ) );
		tpt+= offset;
		SP_PointItem item(
			boost::make_shared< PointItem >( tpt ) );
		pt= item;
	},
		[ & ]( LineItem& pa ) {
		LineItem tpt( pa.get_bg_line( ).first + offset,
			pa.get_bg_line( ).second + offset );
		SP_LineItem item( boost::make_shared< LineItem >( tpt ) );
		pt= item;
	},
		[ & ]( const ArcItem& pa ) {
//		ArcItem tpt( pa );
		SP_ArcItem item( boost::make_shared< ArcItem >( pa ) );
		*item+= offset;
		pt= item;
	//},
	//	[ & ]( const RectItem& pa ) {
	//},
	//	[ & ]( const EllipesItem& pa ) {
	//},
	//	[ & ]( const BezierCubeItem& pa ) {
	},
		[ & ]( const PolyItem& pa ) {
		PolyItem tpt( pa );
		for( auto& item : tpt )
			const_cast< bg_point& >( item )+= offset;
		SP_PolyItem pitem( boost::make_shared< PolyItem >( tpt ) );
		pt= pitem;
	});
	return pt;
}

// ..................................................................
bg_box get_rect_hull( const SP_BaseItem& obs )
{
	bg_box box;
	using boost::geometry::expand;
	using boost::geometry::envelope;

	typecase( *obs,
		[ & ]( PointItem& pa ) {
		box.max_corner( ).set( pa );
		box.min_corner( ).set( pa );
	},
		[ & ]( LineItem& pa ) {
		envelope< bg_line >( pa, box );
		//box.max_corner( ).set( pa.get_bg_line( ).first );
		//box.min_corner( ).set( pa.get_bg_line( ).first );
		expand( box, pa.get_bg_line( ) );
	},
		[ & ]( const ArcItem& pa ) {
	},
		[ & ]( const RectItem& pa ) {
	},
		[ & ]( const EllipesItem& pa ) {
	},
		[ & ]( const BezierCubeItem& pa ) {
	},
		[ & ]( const PolyItem& pa ) {
		auto poly= SP_PolyItem( boost::make_shared< PolyItem >( ) );
		for( auto it : pa )
			expand( box, it );
	});
	return box;
}

// ..................................................................
bg_box get_rect_hull( sp_obj_vect_t& obs )
{
	bg_box box= get_rect_hull( obs.front( ) );

	for( auto item : obs )
	{
		boost::geometry::expand( box, get_rect_hull( item ) );
	}
	return box;
}

// ..................................................................
bg_box get_rect_hull( object_set_t& objects )
{
	bg_box box;
	for( auto& set : objects )
	{
		bg_box bt= get_rect_hull( set.get_set( ) );
		boost::geometry::expand( box, bt );
	}
	return box;
}

