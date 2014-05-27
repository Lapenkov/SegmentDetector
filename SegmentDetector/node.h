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
		box edges;
		std::vector< node* > children;
		std::vector< T > content;

		explicit node();
		bool is_leaf() const;
	};

	template< typename T >
	node< T >::node()
	{
	}
	template< typename T >
	bool node< T >::is_leaf() const
	{
		return children.empty();
	}
}

#endif //_NODE_H_DEFINED_