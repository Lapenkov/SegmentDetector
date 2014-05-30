#include "simple_types.h"

#include <algorithm>
#include <limits>

#include <boost/assign/list_of.hpp>

namespace storage
{
	namespace detail
	{
		void update_edges( box& old, const point& to_include )
		{
			old = box::build_polygon( boost::assign::list_of( old.low_left )
															( old.top_right) 
															( to_include ) );
		}
		void update_edges( box& old, const box& to_include )
		{
			old = box::build_polygon( boost::assign::list_of( old.low_left )
															( old.top_right) 
															( to_include.low_left )
															( to_include.top_right ) );
		}
		box get_edges( point& obj1, const point& obj2 )
		{
			return box::build_polygon( boost::assign::list_of( obj1 )
															 ( obj2 ) );
		}
	}
	point::point( const int x, const int y )
		: x( x )
		, y( y )
	{
	}
	const point& point::operator = ( const point& to )
	{
		x = to.x;
		y = to.y;
		return *this;
	}
	bool point::is_defined() const
	{
		return x != undefined_coord && y != undefined_coord;
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
		return ( x >= to.x && y >= to.y );
	}
	bool point::operator <( const point& to ) const
	{
		return ( x < to.x && y < to.y );
	}
	bool point::operator <=( const point& to ) const
	{
		return ( x <= to.x && y <= to.y );
	}

	//

	box::box( const point& low_left, const point& top_right )
		: low_left( low_left )
		, top_right( top_right )
	{
	}
	box::box()
		: low_left( point() )
		, top_right( point() )
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
		int xside = std::max( low_left.x, other_box.low_left.x ) - std::min( top_right.x, other_box.top_right.x );
		int yside = std::max( low_left.y, other_box.low_left.y ) - std::min( top_right.y, other_box.top_right.y );

		return ( xside * yside ) < 0 ? 0 : ( xside * yside );
	}
	unsigned box::square() const
	{
		return ( top_right.x - low_left.x ) * ( top_right.y - low_left.y );
	}
	box box::build_polygon( const std::list< point >& points )
	{
		int min_x = std::numeric_limits< int >::max();
		int min_y = std::numeric_limits< int >::max();
		int max_x = std::numeric_limits< int >::min();
		int max_y = std::numeric_limits< int >::min();

		for( std::list< point >::const_iterator cit = points.begin(); cit != points.end(); ++cit )
		{
			min_x = std::min( min_x, cit->x );
			max_x = std::max( max_x, cit->x );
			min_y = std::min( min_y, cit->y );
			max_y = std::max( max_y, cit->y );
		}

		return box( point( min_x, min_y ), point( max_x, max_y ) );
	}
}