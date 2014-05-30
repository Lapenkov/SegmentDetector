#ifndef _SIMPLE_TYPES_H_DEFINED_
#define _SIMPLE_TYPES_H_DEFINED_

#include <list>

namespace storage
{
	struct box;
	struct point;

	namespace detail
	{
		void update_edges( box& old, const point& to_include );
		void update_edges( box& old, const box& to_include );
		box get_edges( point& obj1, const point& obj2 );
	}

	struct point
	{
		static const int undefined_coord = -1000000009;

		int x;
		int y;

		explicit point( const int x = undefined_coord, const int y = undefined_coord );
		const point& operator = ( const point& to );

		bool is_defined() const;
		bool in_polygon( const point& low_left, const point& top_right ) const;
		bool in_polygon( const box& polygon ) const;
		bool operator == ( const point& to ) const;
		bool operator != ( const point& to ) const;
		bool operator > ( const point& to ) const;
		bool operator < ( const point& to ) const;
		bool operator >= ( const point& to ) const;
		bool operator <= ( const point& to ) const;
	};

	//
	template< typename T >
	class segment;

	struct box
	{
		point low_left;
		point top_right;

		template< typename T >
		explicit box( segment< T > axis );
		explicit box( const point& low_left, const point& top_right);
		explicit box();

		static box build_polygon( const std::list< point >& points );
		bool in_polygon( const box& other_box ) const;
		bool overlaps( const box& other_box ) const;
		unsigned overlap_square( const box& other_box ) const;
		unsigned square() const;
	};

	template< typename T >
	box::box( segment< T > axis )
	{
		int x1, y1, x2, y2;
		x1 = axis.get_start().x;
		y1 = axis.get_start().y;
		x2 = axis.get_end().x;
		y2 = axis.get_end().y;
		if( x1 > x2 )
			std::swap( x1, x2 );
		if( y1 > y2 )
			std::swap( y1, y2 );
		low_left = point( x1, y1 );
		top_right = point( x2, y2 );
	}

	//

	template< typename T >
	class segment
	{	
	private:
		point a_;
		point b_;
		T info_;
	
	public:
		explicit segment( const point& start, const point& end, const T& info = T() );
		const segment& operator = ( const segment< T >& to );

		const point& get_start() const;
		const point& get_end() const;
		bool in_polygon( const point& low_left, const point& top_right ) const;
		bool in_polygon( const box& polygon ) const;
		const box get_edges() const;
		const T& get_info() const;
	};
	
	template< typename T >
	segment< T >::segment( const point& start, const point& end, const T& info )
		: a_( start )
		, b_( end )
		, info_( info )
	{
	}
	template< typename T >
	const segment< T >& segment< T >::operator = ( const segment< T >& to )
	{
		a_ = to.a_;
		b_ = to.b_;
		info_ = to.info_;
		return *this;
	}
	template< typename T > 
	const point& segment< T >::get_start() const
	{
		return a_;
	}
	template< typename T > 
	const point& segment< T >::get_end() const
	{
		return b_;
	}
	template< typename T >
	bool segment< T >::in_polygon( const point& low_left, const point& top_right ) const
	{
		return ( a_.in_polygon( low_left, top_right ) && b_.in_polygon( low_left, top_right ) ); 
	}
	template< typename T >
	bool segment< T >::in_polygon( const box& polygon ) const
	{
		return ( in_polygon( polygon.low_left, polygon.top_right ) ); 
	}
	template< typename T >
	const box segment< T >::get_edges() const
	{
		return box( *this );
	}
	template< typename T >
	const T& segment< T >::get_info() const
	{
		return info_;
	}
}

#endif //_SIMPLE_TYPES_H_DEFINED_