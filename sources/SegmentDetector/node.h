#ifndef _NODE_H_DEFINED_
#define _NODE_H_DEFINED_

#include "simple_types.h"

#include <boost/noncopyable.hpp>
#include <boost/assign/list_of.hpp>

#include <vector>
#include <list>

namespace storage
{
	template< typename T >
	struct node : private virtual boost::noncopyable
	{
		node* parent;
		box edges;
		std::list< node* > children;
		std::list< T* > content;

		explicit node();
		bool is_leaf() const;
		void resolve_edges();
		const box& get_edges() const;
	};

	template< typename T >
	node< T >::node()
		: parent( NULL )
	{
	}
	template< typename T >
	bool node< T >::is_leaf() const
	{
		return children.empty();
	}
	template< typename T >
	void node< T >::resolve_edges()
	{
		std::list< point > all_points;

		if( edges.low_left.is_defined() && edges.top_right.is_defined() )
		{
			all_points.push_back( edges.low_left );
			all_points.push_back( edges.top_right );
		}

		if( is_leaf() )
		{
			for( std::list< T* >::const_iterator cit = content.begin(); cit != content.end(); ++cit )
			{
				all_points.push_back( (*cit)->get_edges().low_left );
				all_points.push_back( (*cit)->get_edges().top_right );
			}
		}
		else
		{
			for( std::list< node* >::const_iterator cit = children.begin(); cit != children.end(); ++cit )
			{
				all_points.push_back( (*cit)->get_edges().low_left );
				all_points.push_back( (*cit)->get_edges().top_right );
			}
		}

		edges = box::build_polygon( all_points );
	}
	template< typename T >
	const box& node< T >::get_edges() const
	{
		return edges;
	}
}

#endif //_NODE_H_DEFINED_