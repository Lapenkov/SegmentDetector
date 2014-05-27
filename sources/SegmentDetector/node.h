#ifndef _NODE_H_DEFINED_
#define _NODE_H_DEFINED_

#include "simple_types.h"

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/assign/list_of.hpp>

#include <vector>
#include <list>

namespace storage
{
	template< typename T >
	struct node : private virtual boost::noncopyable
	{		
		box edges;
		std::vector< node* > children;
		std::list< T > content;

		explicit node();
		bool is_leaf() const;
		void add_content( const T& to_add );
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
	template< typename T >
	void node< T >::add_content( const T& to_add )
	{
		if( content.empty() )
		{
			content.push_back( to_add );
			edges = box( to_add );
		}
		else
		{
			edges = box::build_polygon( boost::assign::list_of( edges.low_left )
															  (	edges.top_right )
															  ( to_add.get_start() ) 
															  (	to_add.get_end() ) );
		}
	}
}

#endif //_NODE_H_DEFINED_