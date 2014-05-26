#ifndef _SIMPLE_TYPES_H_DEFINED_
#define _SIMPLE_TYPES_H_DEFINED_

namespace storage
{
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
		bool in_polygon( const point& left_bottom, const point& right_top ) const;
		bool operator == ( const point& to ) const;
		bool operator != ( const point& to ) const;
	};
	//
	template< class T >
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
		bool in_polygon( const point& left_bottom, const point& right_top ) const;
	};
	
	template< class T >
	segment< T >::segment( const point& start, const point& end, const T& info )
		: a_( start )
		, b_( end )
		, info_( info )
	{
	}
	template< class T >
	const segment< T >& segment< T >::operator = ( const segment< T >& to )
	{
		a_ = to.a_;
		b_ = to.b_;
		info_ = to.info_
		return *this;
	}
	template< class T > 
	const point& segment< T >::get_start() const
	{
		return a_;
	}
	template< class T > 
	const point& segment< T >::get_end() const
	{
		return b_;
	}
	template< class T >
	bool segment< T >::in_polygon( const point& left_bottom, const point& right_top ) const
	{
		return ( a_.in_polygon( left_bottom, right_top ) && b_.in_polygon( left_bottom, right_top ) ); 
	}
}

#endif //_SIMPLE_TYPES_H_DEFINED_