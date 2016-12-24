

// Drawing.h

#pragma once
#define DRAWING_H
#define SET_OSFLOAT std::fixed << std::setw( 7 ) << std::setprecision( 3 ) << std::setfill( L' ' )

// ..........................................................................
// ..........................................................................
class DrawArea;
class DrawingObects
{
	friend DrawArea;
	layer_set_t& layers;
	bg_point offset;
	object_set_t primitives;
	//size_t id;

public:
	DrawingObects( layer_set_t& layers )
		:layers( layers )
		//,id( 0 )
	{ }
	void set_offset( const bg_point& pt ) { offset= pt; }
	void push_back( const ObjectSet& obj ) { primitives.push_back( obj ); }
	object_set_t& get_set( ) { return primitives; }
	const layer_set_t& get_layers( ) const { return layers; }
};

// ..........................................................................
//drawing.....
typedef boost::shared_ptr< CPen > SP_CPen;
typedef std::vector< SP_CPen > VSP_CPen;
typedef boost::shared_ptr< CBrush > SP_CBrush;
typedef std::vector< SP_CBrush > VSP_CBrush;

AFX_STATIC_DATA	int penStylesRnd=  PS_SOLID | PS_ENDCAP_ROUND | PS_GEOMETRIC;
AFX_STATIC_DATA	int penStylesFlat=  PS_SOLID | PS_ENDCAP_FLAT |PS_GEOMETRIC;

// ..........................................................................
struct PCPEN
{
	COLORREF color;
	int styles;
	size_t width;
	//enum of style...
	SP_CPen sp_Pen;
	PCPEN( COLORREF icolor, size_t iwidth, int inStyles ) //for comp
		:color( icolor )
		,width( iwidth )
		,styles( inStyles )
	{ }

	bool operator < ( const PCPEN& a ) const
	{
		if( styles != a.styles )
			return a.styles < styles;
		return ( (__int64)a.color | ( (__int64)a.width << 32 ) )
			< ( (__int64)color | ( (__int64)width << 32 ) );
	}
	operator CPen* ( ) { return sp_Pen.get( ); }
};

typedef std::set< PCPEN > PenSet;
typedef std::set< PCPEN >::iterator PenSetIt;

// ..........................................................................
//brush is not used yet, or never?
struct PCBRUSH
{
	LOGBRUSH lb;
	int styles;
	size_t width;
	//enum of style...
	SP_CBrush sp_Brush;
	PCBRUSH( COLORREF icolor, int inStyles ) //for comp
		//:width( iwidth )
	{ lb.lbColor= icolor; }

	bool operator < ( const PCBRUSH& a ) const
	{
		if( styles != a.styles )
			return a.styles < styles;
		return ( (__int64)a.lb.lbColor | ( (__int64)a.width << 32 ) )
			< ( (__int64)lb.lbColor | ( (__int64)width << 32 ) );
	}
	operator CBrush* ( ) { return sp_Brush.get( ); }
};

typedef std::set< PCBRUSH > BrushSet;
typedef std::set< PCBRUSH >::iterator BrushSetIt;

// ..........................................................................
struct BrushInfo
{
	COLORREF color;
	UINT	type;
};

// ..........................................................................
struct DrawExtent
{
	CRect& rectClient;
	long& of_x;
	long& of_y;
	double& zoom;

	DrawExtent( CRect& rect, long& ix, long& iy, double& izoom )
		:rectClient( rect )
		,of_x( ix )
		,of_y( iy )
		,zoom( izoom )
	{ }
};

// ..........................................................................
class DrawArea
{
	CDC* pDC;
	DrawExtent draw;
	PenSet pen_set;
//	BrushSet brush_set;
	SP_CPen spLastPen;
	BrushInfo lastBrush;
	LOGBRUSH pen_logbrush;

public:
	DrawArea( CDC* pInDC, /*PCBoard& inBoard, */DrawExtent& iDraw )
		:pDC( pInDC )
		,draw( iDraw )
	{
		pDC->SelectStockObject( NULL_BRUSH );
		pen_logbrush.lbColor= RGB( 255, 255, 255 );
		pen_logbrush.lbHatch= 0;
		pen_logbrush.lbStyle= BS_SOLID;
	}
	CRect GetDrawRect( const bg_box& box ) const;
	CPoint GetDrawPoint( const bg_point& point ) const;

	void SetPen( COLORREF const& lb, size_t width, int inStyle );
	void SetDCBrush( COLORREF const& lb );//  { pDC->SelectStockObject( DC_BRUSH ); pDC->SetDCBrushColor( lb ); }
	void SetNullBrush( ) { pDC->SelectStockObject( NULL_BRUSH ); }
	void DrawLine( bg_line const& line );
	void DrawCross( CPoint pt );
	void DrawBoardCross( bg_point const& pt );
	void DrawRect( const bg_box& rect ) const;
	void DrawArc( const ArcItem& arc );
	void DrawEllipes( const bg_box& box ) const;
	void PopPenBrush( );
	void DrawObject( const ItemObj& obj );
	void DrawObjects( const DrawingObects& obj );
	void DrawPoint( const bg_point );
	void DrawRawPoint( const bg_point );
	void DrawBezier( const bg_point& p0, const bg_point& p1, const bg_point& p2, const bg_point& p3 );
	void DrawPoly( const bg_linestring_t& ls );
	void DrawTest( );
	void DrawGrid( );
};

// ............................................................................
inline void DrawArea::DrawRawPoint( const bg_point pt ) //temp
{
	pDC->Rectangle(
		(long)( pt.get_x( ) - 1 )
		,(long)( pt.get_y( )- 1 )
		,(long)( pt.get_x( )+ 1 )
		,(long)( pt.get_y( )+ 1 )
		);
}

// ............................................................................
inline CRect DrawArea::GetDrawRect( const bg_box& box ) const
{
	return CRect(
		(long)( box.min_corner( ).get_x( ) * draw.zoom + draw.of_x ),
		(long)( box.min_corner( ).get_y( ) * draw.zoom + draw.of_y ),
		(long)( box.max_corner( ).get_x( ) * draw.zoom + draw.of_x ),
		(long)( box.max_corner( ).get_y( ) * draw.zoom + draw.of_y )
	);
}

// ............................................................................
inline CPoint DrawArea::GetDrawPoint( const bg_point& point ) const
{
	return CPoint(
		(long)( point.get_x( ) * draw.zoom + draw.of_x ),
		(long)( point.get_y( ) * draw.zoom + draw.of_y )
	);
}

// ............................................................................
inline void DrawArea::DrawRect( const bg_box& rect ) const
{
	pDC->Rectangle( GetDrawRect( rect ) );
}


