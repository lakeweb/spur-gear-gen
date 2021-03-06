
/*
	objects.h
*/

#pragma once
#ifndef OBJECT_H
#define OBJECT_H

// ..........................................................................
// ..........................................................................
//base class for milling objects
class BaseItem
	:public boost::enable_shared_from_this< BaseItem > //item
{
	COLORREF color;
	size_t width;
	bool bMill;

public:
	virtual ~BaseItem( ) { }
	BaseItem( )
		:color( RGB( 0, 0, 0 ) )
		,width( 1 )
		,bMill( false )
	{ }
	BaseItem( const BaseItem& o )
		:color( o.color )
		,width( o.width )
		,bMill( o.bMill )
	{ }
	typedef boost::shared_ptr< BaseItem > SP_BaseItem;
	SP_BaseItem get_SP( ) {	return boost::make_shared< BaseItem >( *this ); }
	void SetColor( COLORREF in ) { color= in; }
	void SetWidth( size_t in ) { width= in; }
	const COLORREF GetColor( ) const { return color; }
	void SetMill( ) { bMill= true; }
	bool GetMill( ) const { return bMill; }
};

typedef boost::shared_ptr< BaseItem > SP_BaseItem;
typedef std::vector< SP_BaseItem > sp_obj_vect_t;
typedef std::vector< SP_BaseItem >::const_iterator sp_obj_vect_cit_t;

// ..........................................................................
class PointItem : public BaseItem
{
	bg_point point;

public:
	PointItem( const bg_point& a )
		:point( a )
	{ }
	operator const bg_point& ( ) { return point; }
	const bg_point& get_bg_point( ) const { return point; }
	void set( bg_point& in ) { point= in; }
	void operator = ( const PointItem& in ) { point= in.get_bg_point( ); }
};
typedef boost::shared_ptr< PointItem > SP_PointItem;

// ..........................................................................
class LineItem : public BaseItem
	,public boost::enable_shared_from_this< LineItem > //item
{
	bg_line line;

public:
	LineItem( const bg_point& a, const bg_point& b )
		:line( a, b )
	{ }
	LineItem( const bg_line& a )
		:line( a )
	{ }
	LineItem( ) { }
	boost::shared_ptr< LineItem > get_SP( ) {	return boost::make_shared< LineItem >( *this ); }
	operator const bg_line& ( ) const { return line; }
	const bg_line& get_bg_line( ) const { return line; }
	void set( bg_line& in ) { line= in; }
};
typedef boost::shared_ptr< LineItem > SP_LineItem;

// ..........................................................................
/* in:
	a as start of arc
	b as end of arc
	o center of arc
*/
class ArcItem : public BaseItem
	,public boost::enable_shared_from_this< ArcItem > //item
{
	bg_point a;
	bg_point b;
	bg_point o;
	//
	double rad;

	friend void operator += ( ArcItem& a, const bg_point& b );
public:
	ArcItem( ) { }

	ArcItem( const bg_point& a, const bg_point& b, const bg_point& o )
		:a( a )
		,b( b )
		,o( o )
	{
		rad= sqrt( ( ( o.get_x( ) - a.get_x( ) ) * ( o.get_x( ) - a.get_x( ) ) )
			+ ( ( o.get_y( ) - a.get_y( ) ) * ( o.get_y( ) - a.get_y( ) ) ) );
	}
	//box for DC
	bg_box get_box( ) const { return bg_box( bg_point( o.get_x( ) - rad, o.get_y( ) - rad ), bg_point( o.get_x( ) + rad, o.get_y( ) + rad ) ); }
	bg_point get_a( ) const { return a; }
	bg_point get_b( ) const { return b; }
	bg_point get_o( ) const { return o; }
	GEO_NUM get_rad( ) const { return rad; }
	//angles for DXF
	GEO_NUM get_start_ang( ) const;
	GEO_NUM get_end_ang( ) const;
	void rotate( const GEO_NUM angle, const bg_point& origin );
	boost::shared_ptr< ArcItem > get_SP( ) {	return boost::make_shared< ArcItem >( *this ); }
	//void set( const bg_point& ia, const bg_point& ib, const bg_point& io ) { a= ia; b= ib; o= io; }
};
typedef boost::shared_ptr< ArcItem > SP_ArcItem;

// ..........................................................................
class RectItem : public BaseItem
	,public boost::enable_shared_from_this< RectItem > //item
{
	bg_box box;

public:
	RectItem( const bg_point& a, const bg_point& b )
		:box( a, b )
	{ }
	RectItem( const bg_box& a )
		:box( a )
	{ }
	boost::shared_ptr< RectItem > get_SP( ) {	return boost::make_shared< RectItem >( *this ); }
	const bg_box& get_bg_box( ) const { return box; }
	GEO_NUM width( ) const { return box.max_corner( ).get_x( ) - box.min_corner( ).get_x( ); }
	GEO_NUM height( ) const { return box.max_corner( ).get_y( ) - box.min_corner( ).get_y( ); }

	bg_point& get_min_point( ) { return box.min_corner( ); }
	bg_point& get_max_point( ) { return box.max_corner( ); }
};
typedef boost::shared_ptr< RectItem > SP_RectItem;

// ..........................................................................
class EllipesItem : public BaseItem
{
	bg_box box;

public:
	EllipesItem( const bg_point& a, const bg_point& b )
		:box( a, b )
	{ }

	EllipesItem( const double diameter, COLORREF color )
		:box( bg_point( - diameter / 2, - diameter / 2 ), bg_point( diameter / 2, diameter / 2 ) )
	{ SetColor( color ); }

	EllipesItem( )
		:box( bg_point( 0, 0 ), bg_point( 0, 0 ) )
	{ }
	const bg_box& get_bg_box( ) const { return box; }
	void set( bg_box& in ) { box= in; }
};
typedef boost::shared_ptr< EllipesItem > SP_EllipesItem;

// ..........................................................................
class BezierCubeItem : public BaseItem
{
	bg_point p0;
	bg_point p1;
	bg_point p2;
	bg_point p3;

public: //cubic bezier
	BezierCubeItem( const bg_point& p0, const bg_point& p1, const bg_point& p2, const bg_point& p3 )
		:p0( p0 )
		,p1( p1 )
		,p2( p2 )
		,p3( p3 )
	{ }

	BezierCubeItem( ) { }
	//BezierCubeItem( const double diameter, COLORREF color )
	//	:box( bg_point( - diameter / 2, - diameter / 2 ), bg_point( diameter / 2, diameter / 2 ) )
	//{ SetColor( color ); }

	//BezierCubeItem( )
	//	:box( bg_point( 0, 0 ), bg_point( 0, 0 ) )
	//{ }
	const bg_point& get_point0( ) const { return p0; }
	const bg_point& get_point1( ) const { return p1; }
	const bg_point& get_point2( ) const { return p2; }
	const bg_point& get_point3( ) const { return p3; }

	void set_point0( const bg_point& i ) { p0= i; }
	void set_point1( const bg_point& i ) { p1= i; }
	void set_point2( const bg_point& i ) { p2= i; }
	void set_point3( const bg_point& i ) { p3= i; }
};
typedef boost::shared_ptr< BezierCubeItem > SP_BezierCubeItem;

// ..........................................................................
class PolyItem : public BaseItem
	,public boost::enable_shared_from_this< PolyItem > //item
{
	bg_linestring_t ls;
	typedef bg_linestring_t::const_iterator iter;
	typedef bg_linestring_t::const_reverse_iterator riter;
public:
	void operator += ( const bg_point& i ) { ls.push_back( i ); }
	iter begin( ) const { return ls.begin( ); }
	iter end( ) const { return ls.end( ); }
	riter rbegin( ) const { return ls.rbegin( ); }
	riter rend( ) const { return ls.rend( ); }
	size_t size( ) const { return ls.size( ); }
	const bg_linestring_t& get_item( ) const { return ls; }
	bg_point& back( ) { return ls.back( ); }
	bg_point& at (size_t i ) { return ls.at( i ); }
	boost::shared_ptr< PolyItem > get_SP( ) {	return boost::make_shared< PolyItem >( *this ); }
};
typedef boost::shared_ptr< PolyItem > SP_PolyItem;

// ..........................................................................
//wrapper for sp_obj_vect_t

typedef class _ItemSet : public BaseItem
	,public boost::enable_shared_from_this< _ItemSet >
{
	sp_obj_vect_t set;
	size_t layer;
	size_t id;
	bg_point offset;

public:
	_ItemSet( )
		:layer( 0 )
		,id( 0 )
	{ }
	_ItemSet( size_t layer )
		:layer( layer )
	{ }
	void push_back( SP_BaseItem spi ) { set.push_back( spi ); }
	sp_obj_vect_cit_t begin( ) const { return set.begin( ); }
	sp_obj_vect_cit_t end( ) const { return set.end( ); }
	const sp_obj_vect_t& get_set( ) const { return set; }
	size_t GetLayer( ) const { return layer; }
	void set_layer( size_t ilayer ) { layer= ilayer; }
	void clear( ) { set.clear( ); }
	void insert( sp_obj_vect_t::const_iterator first, sp_obj_vect_t::const_iterator last ) { set.insert( set.begin( ), first, last ); }
	void set_id( size_t in ) { id= in; }
	size_t get_id( ) const { return id; }
	void set_offset( const bg_point& in ) { offset= in; }
	const bg_point& get_offset( ) const { return offset; }
	boost::shared_ptr< _ItemSet > make_SP( ) {	return boost::make_shared< _ItemSet >( *this ); }

}ItemSet;

//typedef boost::shared_ptr< ItemSet > SP_ItemSet;
//
//typedef std::vector< SP_ItemSet > object_set_t;
//using a list so the objects won't move around on addition/deletion
typedef std::list< ItemSet > object_set_t;

//globals .................................
//both of these rotate/tranform a copied object leaving the origanal in place
SP_BaseItem rotate_object( GEO_NUM angle, const SP_BaseItem& input, bg_point origin= bg_point( ) );
SP_BaseItem transform_object( const SP_BaseItem& input, bg_point offset );

bg_box get_rect_hull( const SP_BaseItem& obs );
bg_box get_rect_hull( const sp_obj_vect_t& obs );
bg_box get_rect_hull( const object_set_t& objects );

std::ostream& operator << ( std::ostream& os, const SP_BaseItem& in );


#endif //OBJECT_H
