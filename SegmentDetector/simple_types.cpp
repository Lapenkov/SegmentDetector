#include "simple_types.h"

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
	bool point::in_polygon( const point& left_bottom, const point& right_top ) const
	{
		return ( left_to( right_top ) && 
				below( right_top ) && 
				above( left_bottom ) && 
				right_to( left_bottom ) );
	}
	bool point::operator ==( const point& to ) const
	{
		return ( x == to.x && y == to.y );
	}
	bool point::operator !=( const point& to ) const
	{
		return !( *this == to );
	}
	size_t hash_value( const point& value )
    {
        return value.get_id();
    }
}