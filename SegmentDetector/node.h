#ifndef _NODE_H_DEFINED_
#define _NODE_H_DEFINED_

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace storage
{
	template< class T >
	struct node : private virtual boost::noncopyable
	{
		node* upright;
		node* downright;
		node* upleft;
		node* downleft;

		const T data;

		explicit node( const T& data );
	};

	template< class T >
	node< T >::node( const T& data )
		: data( data )
		, upright( NULL )
		, downright( NULL )
		, upleft( NULL )
		, downleft( NULL )
	{}
}

#endif //_NODE_H_DEFINED_