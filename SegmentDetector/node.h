#ifndef _NODE_H_DEFINED_
#define _NODE_H_DEFINED_

#include "simple_types.h"

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <vector>

namespace storage
{
	template< typename T >
	struct node : private virtual boost::noncopyable
	{
		typedef segment< T > segment_t; 
		
		box edges;
		std::vector< node* > children;
		std::vector< segment_t > content;

		explicit node();
	};

	template< typename T >
	node< T >::node()
	{}
}

#endif //_NODE_H_DEFINED_