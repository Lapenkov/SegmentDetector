#ifndef _TREE_H_DEFINED_
#define _TREE_H_DEFINED_

#include "node.h"
#include "simple_types.h"

#include <vector>
#include <iterator>

#include <boost/range.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/unordered_set.hpp>
#include <boost/foreach.hpp>

namespace storage
{
	template< class T  >
	class tree
	{
	private:
		static const size_t max_content_in_node = 4ul;

		typedef std::vector< T > segment_arr;
		typedef node< T > vertex;
		vertex* root_;

	public:
		explicit tree( const segment_arr& segments );
		explicit tree();

		void add_segment( const T& to_add );
		void add_segment( const T& to_add, vertex* current_v );
		
		template< class U > 
		void find_segments( const box& query, std::back_insert_iterator< U > res ) const; 
		template< class U > 
		void find_segments( const box& query, std::back_insert_iterator< U > res, const vertex* current_v ) const; 
	};

	template< class T >
	tree< T >::tree()
	{
	}
	template< class T >
	tree< T >::tree( const segment_arr& segments )
	{
		boost::range::for_each( segments.begin(), segments.end(), add_segment( boost::lambda::_1 ) );
	}
	template< class T >
	void tree< T >::add_segment( const T& to_add )
	{
		add_segment( to_add, root_ );
	}
	template< class T >
	void tree< T >::add_segment( const T& to_add, vertex* current_v )
	{
		
	}
	template< class T >
	template< class U >
	void tree< T >::find_segments( const box& query, std::back_insert_iterator< U > res ) const
	{
		if( root_ == NULL )
			return;
		find_segments( query, res, root_ );
	}
	template< class T >
	template< class U >
	void tree< T >::find_segments( const box& query, std::back_insert_iterator< U > res, const vertex* current_v ) const
	{
		if( current_v->children.empty() )
		{
			//leaf node
			BOOST_FOREACH( const T& segment, current_v->content )
				if( segment.in_polygon( query ) )
					( *res++ ) = segment;
		}
		else
		{
			//non-leaf node
			BOOST_FOREACH( const vertex* child, current_v->children )
				if( child->edges.overlaps( query ) )
					find_segments( query, res, child );
		}
	}
}

#endif //_TREE_H_DEFINED_