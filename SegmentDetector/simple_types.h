#ifndef _SIMPLE_TYPES_H_DEFINED_
#define _SIMPLE_TYPES_H_DEFINED_

namespace storage
{
	template< typename T >
	class point
	{
	private:
		static size_t id_count;

		int x_;
		int y_;
		const size_t id_;
		point< T >* linked_;
		T info_;

	public:
		explicit point();
		point( const int x, const int y, const T& info );
		const point& operator = ( const point& to );

		size_t get_id() const;
		int get_x() const;
		int get_y() const;
		const T& get_info() const;
		const point< T >* get_linked() const;
		void set_linked( point< T >* to );
		bool above( const point< T >& to ) const;
		bool below( const point< T >& to ) const;
		bool right_to( const point< T >& to ) const;
		bool left_to( const point< T >& to ) const;
		bool in_polygon( const point< T >& left_bottom, const point< T > right_top ) const;
		bool operator == ( const point< T >& to ) const;
		bool operator != ( const point< T >& to ) const;
	};

	template< typename T >
	size_t point< T >::id_count = 0ul;

	template< typename T >
	point< T >::point()
		: x_( 0 )
		, y_( 0 )
		, id_( ++id_count )
	{
	}
	template< typename T >
	point< T >::point( const int x, const int y, const T& info )
		: x_( x )
		, y_( y )
		, info_( info )
		, id_( ++id_count )
	{
	}
	template< typename T >
	const point< T >& point< T >::operator = ( const point& to )
	{
		x_ = to.x_;
		y_ = to.y_;
		linked_ = to.linked_;
		info_ = to.info_;
		return *this;
	}

	template< typename T >
	size_t point< T >::get_id() const 
	{
		return id_;
	}
	template< typename T >
	int point< T >::get_x() const 
	{
		return x_;
	}
	template< typename T >
	int point< T >::get_y() const 
	{
		return y_;
	}
	template< typename T >
	const T& point< T >::get_info() const 
	{
		return info_;
	}
	template< typename T >
	const point< T >* point< T >::get_linked() const 
	{
		return linked_;
	}
	template< typename T >
	void point< T >::set_linked( point< T >* to ) 
	{
		linked_ = to;
	}
	template< typename T >
	bool point< T >::above( const point< T >& to ) const
	{
		return ( y_ > to.y_ );
	}
	template< typename T >
	bool point< T >::below( const point< T >& to ) const
	{
		return ( y_ < to.y_ );
	}
	template< typename T >
	bool point< T >::right_to( const point< T >& to ) const
	{
		return ( x_ > to.x_ );
	}
	template< typename T >
	bool point< T >::left_to( const point< T >& to ) const
	{
		return ( x_ < to.x_ );
	}
	template< typename T >
	bool point< T >::in_polygon( const point< T >& left_bottom, const point< T > right_top ) const
	{
		return ( left_to( right_top ) && 
				below( right_top ) && 
				above( left_bottom ) && 
				right_to( left_bottom ) );
	}
	template< typename T >
	bool point< T >::operator ==( const point< T >& to ) const
	{
		return ( x_ == to.x_ && y_ == to.y_ );
	}
	template< typename T >
	bool point< T >::operator !=( const point< T >& to ) const
	{
		return !( *this == to );
	}
	template< typename T >
	size_t hash_value( const point< T >& value )
    {
        return value.get_id();
    }

	//

	template< class T >
	class segment
	{
	public:
		typedef point< T > point_type;
	
	private:
		point_type a_;
		point_type b_;
	
	public:
		explicit segment( point_type& a, point_type& b );
		const segment& operator = ( const segment< T >& to );

		const point_type& get_start() const;
		const point_type& get_end() const;
	};
	
	template< class T >
	segment< T >::segment( point_type& start, point_type& end )
		: a_( start )
		, b_( end )
	{
		a_.set_linked( &b_ );
		b_.set_linked( &a_ );
	}
	template< class T >
	const segment< T >& segment< T >::operator = ( const segment< T >& to )
	{
		a_ = to.a_;
		b_ = to.b_;

		return *this;
	}
	template< class T > 
	const typename segment< T >::point_type& segment< T >::get_start() const
	{
		return a_;
	}
	template< class T > 
	const typename segment< T >::point_type& segment< T >::get_end() const
	{
		return b_;
	}
}

#endif //_SIMPLE_TYPES_H_DEFINED_