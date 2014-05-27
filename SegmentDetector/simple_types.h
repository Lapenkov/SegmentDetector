#ifndef _SIMPLE_TYPES_H_DEFINED_
#define _SIMPLE_TYPES_H_DEFINED_

#include <vector>

namespace storage
{
	struct box;

	class point
	{
	private:
		static size_t id_count;
		const size_t id_;

	public:
		int x;
		int y;

		explicit point( const int x = 0, const int y = 0 );
		const point& operator = ( const point& to );

		size_t get_id() const;
		bool above( const point& to ) const;
		bool below( const point& to ) const;
		bool right_to( const point& to ) const;
		bool left_to( const point& to ) const;
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
	
	//

	struct box
	{
		point low_left;
		point top_right;

		explicit box( const point& low_left, const point& top_right);
		explicit box();

		static box build_polygon( const std::vector< point >& points );
		bool in_polygon( const box& other_box ) const;
		bool overlaps( const box& other_box ) const;
		unsigned overlap_square( const box& other_box ) const;
		unsigned square() const;
	};
}

#endif //_SIMPLE_TYPES_H_DEFINED_