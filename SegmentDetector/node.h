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
		
		bool is_leaf;
		std::vector< node* > children;
		std::vector< segment_t > content;
		
		struct 
		{
			point left_bottom;
			point right_top;
		} edges;

		explicit node();
	};

	template< typename T >
	node< T >::node()
	{}
}

#endif //_NODE_H_DEFINED_