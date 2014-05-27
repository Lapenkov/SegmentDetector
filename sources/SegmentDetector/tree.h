#ifndef _TREE_H_DEFINED_
#define _TREE_H_DEFINED_

#include "node.h"
#include "simple_types.h"

#include <vector>
#include <iterator>
#include <limits>
#include <list>

#include <boost/range.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/unordered_set.hpp>
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>

namespace storage
{
	template< class T  >
	class tree
	{
	private:
		static const size_t min_content_in_node = 2ul;
		static const size_t max_content_in_node = 6ul;

		typedef std::vector< T > segment_arr;
		typedef std::list< T > segment_list;
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

	private:
		void quadratic_split( vertex* to_split, vertex* group1, vertex* group2 ) const;
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
		if( current_v->is_leaf() )
		{
			current_v->content.push_back( to_add );
			if( current_v->content.size() > max_content_in_node )
			{
				vertex* new_left;
				vertex* new_right;

				quadratic_split( current_v, new_left, new_right );
			}
		}
		else
		{
			unsigned min_enlargement = std::numeric_limits< unsigned >::max();
			vertex* chosen_path = NULL;
			BOOST_FOREACH( vertex* child, current_v->children )
			{
				box new_bounds = box::build_polygon( boost::assign::list_of ( to_add.get_start() )
																			( to_add.get_end() )
																			( child->edges.low_left )
																			( child->edges.top_right ) );
				unsigned enlargement = new_bounds.square() - child->edges.square();
				if( enlargement < min_enlargement )
				{
					min_enlargement = enlargement;
					chosen_path = child;
				}
			}

			add_segment( to_add, chosen_path );
		}
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
		if( current_v->is_leaf() )
		{
			BOOST_FOREACH( const T& segment, current_v->content )
				if( segment.in_polygon( query ) )
					( *res++ ) = segment;
		}
		else
		{
			BOOST_FOREACH( const vertex* child, current_v->children )
				if( child->edges.overlaps( query ) )
					find_segments( query, res, child );
		}
	}
	template< class T >
	void tree< T >::quadratic_split( vertex* to_split, vertex* group1, vertex* group2 ) const
	{
		group1 = new vertex();
		group2 = new vertex();
		segment_list::const_iterator first_found, second_found;
		int max_enlargement = -1;
		for( segment_list::const_iterator cit = to_split->content.begin(); cit != to_split->content.end(); ++cit  )
		{
			segment_list::const_iterator cjt = cit;
			++cjt;
			for( ; cjt != to_split->content.end(); ++cjt )
			{
				const T& first = *cit;
				const T& second = *cjt;
				box new_bounds = box::build_polygon( boost::assign::list_of ( first.get_start() )
																			( first.get_end() )
																			( second.get_start() )
																			( second.get_end() ) );
				box first_bounds = box( first.get_start(), first.get_end() );
				box second_bounds = box( second.get_start(), second.get_end() );
						
				int enlargement = new_bounds.square() - first_bounds.square() - second_bounds.square();
				if( enlargement > max_enlargement )
				{
					first_found = cit;
					second_found = cjt;
					max_enlargement = enlargement;
				}
			}
		}

		group1->add_content( *first_found );
		group2->add_content( *second_found );
		to_split->content.erase( first_found );
		to_split->content.erase( second_found );

		while( !to_split->content.empty() )
		{
			if( group1->content.size() > group2->content.size() )
				std::swap( group1, group2 );
			if( to_split->content.size() == 1 )
			{
				group1->content.push_back( to_split->content.front() );
				to_split->content.clear();
			}

			segment_list::const_iterator next_entry;
			int max_difference = std::numeric_limits< int >::min();
			int enlarge1;
			int enlarge2;

			for( segment_list::const_iterator cit = to_split->content.begin(); cit != to_split->content.end(); ++cit )
			{
				int d1 = box::build_polygon( boost::assign::list_of ( group1->edges.low_left )
																	( group1->edges.top_right ) 
																	( cit->get_start() )
																	( cit->get_end() ) ).square() - group1->edges.square();
				int d2 = box::build_polygon( boost::assign::list_of ( group2->edges.low_left )
																	( group2->edges.top_right ) 
																	( cit->get_start() )
																	( cit->get_end() ) ).square() - group2->edges.square();

				if( d1 - d2 > max_difference )
				{
					max_difference = d1 - d2;
					enlarge1 = d1;
					enlarge2 = d2;
					next_entry = cit;
				}
			}

			if( enlarge1 < enlarge2 || ( enlarge1 == enlarge2 && group1->content.size() < group2->content.size() ) )
				group1->add_content( *next_entry );
			else
				group2->add_content( *next_entry );

			to_split->content.erase( next_entry );
		}
	}
}

#endif //_TREE_H_DEFINED_