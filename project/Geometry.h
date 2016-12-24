
//Geometry.h

#pragma once

#ifndef GEOMETRY_H
#define GEOMETRY_H

// ..........................................................................
typedef TCHAR TPCB_CHAR;

// max signed __int64 9,223,372,036,854,775,807
//typedef __int64 GEO_NUM;
typedef double GEO_NUM;

//milimeters to nanometers
#define MILL_MULT 1000000

typedef unsigned int TPCB_UINT;

template < typename NumType >
struct bg_point_
{
	GEO_NUM x;
	GEO_NUM y;
	bg_point_( )
		:x( 0 )
		,y( 0 )
	{ }
	bg_point_( GEO_NUM ix, GEO_NUM iy )
		:x( ix )
		,y( iy )
	{ }
    template <std::size_t K>
   inline NumType const& get( ) const
    {
        BOOST_STATIC_ASSERT(K < 2);
		if( K == 0 )
	        return x;
		//else
		return y;
   }

    template <std::size_t K>
    inline void set(NumType const& value)
    {
        BOOST_STATIC_ASSERT(K < 2);
		if( K == 0 )
	        x= value;
		else
			y= value;
    }
	GEO_NUM get_x( ) const { return x; }
	GEO_NUM get_y( ) const { return y; }
	void set_x( GEO_NUM ix ) { x= ix; }
	void set_y( GEO_NUM iy ) { y= iy; }
	void set_xy( GEO_NUM ixy ) { x= ixy; y= ixy; }
	void set( bg_point_ in ) { x= in.get_x( ); y= in.get_y( ); }
};


namespace boost { namespace geometry
{

///*!
//\brief Meta-function defining return type of distance function
//\ingroup distance
//\note The strategy defines the return-type (so this situation is different
//    from length, where distance is sqr/sqrt, but length always squared)
// */
////template <typename Geometry1, typename Geometry2 = Geometry1>
//struct default_distance_result
//{
//    typedef typename strategy::distance::services::return_type
//        <
//            typename strategy::distance::services::default_strategy
//                <
//                    point_tag,
//                    typename point_type<__int64>::type,
//                    typename point_type<__int64>::type
//                >::type,
//            typename point_type<__int64>::type,
//            typename point_type<__int64>::type
//        >::type type;
//};


}} // namespace boost::geometry
// ............................................................................
BOOST_GEOMETRY_REGISTER_POINT_2D( bg_point_< GEO_NUM >, GEO_NUM, bg::cs::cartesian, x, y );

// ............................................................................
namespace boost { namespace geometry { namespace traits {
	//nothing here yet........
}}}

// ............................................................................
// typedefs....................................................................
// ............................................................................
typedef bg_point_< GEO_NUM > bg_point;
typedef bg::model::linestring< bg_point> bg_linestring_t;

typedef bg::model::segment< bg_point > bg_line;
typedef bg::model::box< bg_point > bg_box;
typedef bg::model::polygon< bg_point > bg_polygon;

// ............................................................................
inline bg_point bg_polar( GEO_NUM angle, GEO_NUM radius )
{
	return bg_point( radius * cos( angle ), radius * sin( angle ) );
}

// ............................................................................
// these rotates operate around the world origin
// ............................................................................
inline bg_point bg_rotate( GEO_NUM angle, const bg_point& p, const bg_point& org= bg_point( ) )
{
	GEO_NUM cos_v= cos( angle );
	GEO_NUM sin_v= sin( angle );
	return bg_point(
		( p.get_x( ) - org.get_x( ) ) * cos_v - ( p.get_y( ) - org.get_y( ) ) * sin_v + org.get_x( ),
		( p.get_x( ) - org.get_x( ) ) * sin_v + ( p.get_y( ) - org.get_y( ) ) * cos_v + org.get_y( )
		);
}

// ............................................................................
inline bg_line bg_rotate( GEO_NUM angle, const bg_line& p, const bg_point& o= bg_point( ) )
{
	GEO_NUM cos_v= cos( angle );
	GEO_NUM sin_v= sin( angle );
	return bg_line(
		bg_point(
			( p.first.get_x( ) - o.get_x( ) ) * cos_v - ( p.first.get_y( ) - o.get_y( ) ) * sin_v + o.get_x( ),
			( p.first.get_x( ) - o.get_x( ) ) * sin_v + ( p.first.get_y( ) - o.get_y( ) ) * cos_v + o.get_y( )
		),
		bg_point(
			( p.second.get_x( ) - o.get_x( ) ) * cos_v - ( p.second.get_y( ) - o.get_y( ) ) * sin_v + o.get_x( ),
			( p.second.get_x( ) - o.get_x( ) ) * sin_v + ( p.second.get_y( ) - o.get_y( ) ) * cos_v + o.get_y( )
		));
}

// ............................................................................
inline bg_box bg_rotate( const GEO_NUM angle, const bg_box& p )
{
	GEO_NUM cos_v= cos( angle );
	GEO_NUM sin_v= sin( angle );
	return bg_box(
		bg_point( p.min_corner( ).get_x( ) * cos_v - p.min_corner( ).get_y( ) * sin_v,
		p.min_corner( ).get_x( ) * sin_v + p.min_corner( ).get_y( ) * cos_v ),
		bg_point( p.max_corner( ).get_x( ) * cos_v - p.max_corner( ).get_y( ) * sin_v,
		p.max_corner( ).get_x( ) * sin_v + p.max_corner( ).get_y( ) * cos_v )
		);
}

// ............................................................................
// point oerators
// ............................................................................
inline bg_point mirror_x( const bg_point& a )
{
	return bg_point( - a.get_x( ), a.get_y( ) );
}

// ............................................................................
inline bg_point mirror_y( const bg_point& a )
{
	return bg_point( a.get_x( ), -a.get_y( ) );
}

// ............................................................................
//inline bool operator == ( const bg_point& a, const bg_point& b );
//inline bool operator != ( const bg_point& a, const bg_point& b );

// ............................................................................
inline bool operator == ( const bg_point& a, const bg_point& b )
{
	return a.get< 0 >( ) == b.get< 0 >( ) && a.get< 1 >( ) == b.get< 1 >( );
}

// ............................................................................
inline bool operator != ( const bg_point& a, const bg_point& b )
{
	return ! ( a == b );
}

// ..........................................................................
inline bg_point	operator + ( bg_point const& a, bg_point const& b )
{
	return bg_point( a.get_x( ) + b.get_x( ), a.get_y( ) + b.get_y( ) );
}

// ..........................................................................
inline bg_point	operator + ( bg_point const& a, GEO_NUM const& b )
{
	return bg_point( a.get_x( ) + b, a.get_y( ) + b );
}

// ............................................................................
inline bg_point operator - ( bg_point& a, const bg_point& b )
{
	return bg_point( a.get< 0 >( ) - b.get< 0 >( ), a.get< 1 >( ) - b.get< 1 >( ) );
}

// ............................................................................
inline bg_point operator - ( bg_point& a, const GEO_NUM& b )
{
	return bg_point( a.get< 0 >( ) - b, a.get< 1 >( ) - b );
}

// ..........................................................................
inline bool operator < ( const bg_point& a, const bg_point& b )
{
	if( a.get< 0 >( ) < b.get< 0 >( ) )
		return true;
	else if( a.get< 0 >( ) > b.get< 0 >( ) )
		return false;
	return a.get< 1 >( ) < b.get< 1 >( );
}

// ............................................................................
inline void operator += ( bg_point& a, const bg_point& b )
{
	a.set< 0 >( a.get< 0 >( ) + b.get< 0 >( ) );
	a.set< 1 >( a.get< 1 >( ) + b.get< 1 >( ) );
}

// ............................................................................
inline void operator += ( bg_point& a, GEO_NUM const& b )
{
	a.set< 0 >( a.get< 0 >( ) + b );
	a.set< 1 >( a.get< 1 >( ) + b );
}

// ............................................................................
inline void operator -= ( bg_point& a, const bg_point& b )
{
	a.set< 0 >( a.get< 0 >( ) - b.get< 0 >( ) );
	a.set< 1 >( a.get< 1 >( ) - b.get< 1 >( ) );
}

// ............................................................................
inline void operator -= ( bg_point& a, GEO_NUM const& b )
{
	a.set< 0 >( a.get< 0 >( ) - b );
	a.set< 1 >( a.get< 1 >( ) - b );
}

// ............................................................................
inline bg_point operator / ( bg_point& a, const bg_point& b )
{
	return bg_point( a.get_x( ) / b.get_x( ), a.get_y( ) / b.get_y( ) );
}

// ............................................................................
inline bg_point operator / ( bg_point& a, const GEO_NUM& b )
{
	return bg_point( a.get_x( ) / b, a.get_y( ) / b );
}

// ............................................................................
inline std::wostream& operator << ( std::wostream& out, const bg_point& p )
{
	out << "pt: " << p.get_x( ) << ", " << p.get_y( );
	return out;
}

// ............................................................................
inline std::wostream& operator << ( std::wostream& out, const bg_box& p )
{
	out << " fr: " << p.min_corner( ).get_x( ) << ',' << p.min_corner( ).get_y( )
		<< " to " << p.max_corner( ).get_x( ) << ',' << p.max_corner( ).get_y( );
	return out;
}

//Line operators
// ............................................................................
inline void operator += ( bg_line& a, const bg_point& b )
{
	a.first+= b;
	a.second+= b;
}


#endif // GEOMETRY_H
