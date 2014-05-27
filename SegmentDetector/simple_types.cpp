#include "simple_types.h"

#include <algorithm>
#include <limits>

namespace storage
{
	size_t point::id_count = 0ul;

	point::point( const int x, const int y )
		: x( x )
		, y( y )
		, id_( ++id_count )
	{
	}
	const point& point::operator = ( const point& to )
	{
		x = to.x;
		y = to.y;
		return *this;
	}
	size_t point::get_id() const 
	{
		return id_;
	}
	bool point::above( const point& to ) const
	{
		return ( y > to.y );
	}
	bool point::below( const point& to ) const
	{
		return ( y < to.y );
	}
	bool point::right_to( const point& to ) const
	{
		return ( x > to.x );
	}
	bool point::left_to( const point& to ) const
	{
		return ( x < to.x );
	}
	bool point::in_polygon( const point& low_left, const point& top_right ) const
	{
		return ( *this >= low_left && *this <= top_right );
	}
	bool point::in_polygon( const box& polygon ) const
	{
		return in_polygon( polygon.low_left, polygon.top_right ) ;
	}
	bool point::operator ==( const point& to ) const
	{
		return ( x == to.x && y == to.y );
	}
	bool point::operator !=( const point& to ) const
	{
		return !( *this == to );
	}
	bool point::operator >( const point& to ) const
	{
		return ( x > to.x && y > to.y );
	}
	bool point::operator >=( const point& to ) const
	{
		return ( *this > to || *this == to );
	}
	bool point::operator <( const point& to ) const
	{
		return ( x < to.x && y < to.y );
	}
	bool point::operator <=( const point& to ) const
	{
		return ( *this < to || *this == to );
	}
	size_t hash_value( const point& value )
    {
        return value.get_id();
    }

	//

	box::box( const point& low_left, const point& top_right )
		: low_left( low_left )
		, top_right( top_right )
	{
	}
	box::box()
		: low_left( point( 0, 0 ) )
		, top_right( point( 0, 0 ) )
	{
	}
	bool box::in_polygon( const box& other_box ) const
	{
		return ( low_left.in_polygon( other_box ) && top_right.in_polygon( other_box ) );
	}
	bool box::overlaps( const box& other_box ) const
	{
		return overlap_square( other_box ) > 0;
	}
	unsigned box::overlap_square( const box& other_box ) const
	{
		if( low_left.in_polygon( other_box ) && !top_right.in_polygon( other_box ) )
			return ( other_box.top_right.x - low_left.x ) * ( other_box.top_right.y - low_left.y );
		else if( top_right.in_polygon( other_box ) && !low_left.in_polygon( other_box ) )
			return ( top_right.x - other_box.low_left.x ) * ( top_right.y - other_box.low_left.y );
		else if( in_polygon( other_box ) )
			return square();
		else if( other_box.in_polygon( *this ) )
			return other_box.square();
		else
			return 0;
	}
	unsigned box::square() const
	{
		return ( top_right.x - low_left.x ) * ( top_right.y - low_left.y );
	}
	box box::build_polygon( const std::vector< point >& points )
	{
		int min_x = std::numeric_limits< int >::max();
		int min_y = std::numeric_limits< int >::max();
		int max_x = std::numeric_limits< int >::min();
		int max_y = std::numeric_limits< int >::min();

		for( std::vector< point >::const_iterator cit = points.begin(); cit != points.end(); ++cit )
		{
			min_x = std::min( min_x, cit->x );
			max_x = std::max( max_x, cit->x );
			min_y = std::min( min_y, cit->y );
			max_y = std::max( max_y, cit->y );
		}

		return box( point( min_x, min_y ), point( max_x, max_y ) );
	}
}