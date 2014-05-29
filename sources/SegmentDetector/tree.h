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
#include <boost/shared_ptr.hpp>

namespace storage
{
	template< class T >
	class tree
	{
	private:
		static const size_t min_content_in_node = 2ul;
		static const size_t max_content_in_node = 6ul;

		typedef std::vector< T > segment_arr;
		typedef std::list< T > segment_list;
		typedef node< T > vertex;
		typedef boost::shared_ptr< T > segment_ptr;
		std::vector< segment_ptr > all_segments;
		vertex* root_;

	public:
		explicit tree( const segment_arr& segments );
		explicit tree();

		void add_segment( const T& to_add );
		template< class U > 
		void find_segments( const box& query, std::back_insert_iterator< U > res ) const; 
		template< class U > 
		void find_segments( const box& query, std::back_insert_iterator< U > res, const vertex* current_v ) const; 

	private:
		void quadratic_split( vertex* to_split, vertex* group1, vertex* group2 ) const;
		template< typename U >
		void quadratic_split( std::list< U >& to_split, std::list< U >& group1, std::list< U >& group2 ) const;
		vertex* choose_leaf( const T* to_add );
		void adjust_tree( vertex* current_v, vertex* child1, vertex* child2 );
	};

	template< class T >
	tree< T >::tree()
		: root_( new vertex() )
	{
	}
	template< class T >
	tree< T >::tree( const segment_arr& segments )
		: root_( new vertex() )
	{
		boost::range::for_each( segments.begin(), segments.end(), add_segment( boost::lambda::_1 ) );
	}
	template< class T >
	void tree< T >::add_segment( const T& to_add )
	{
		segment_ptr to_add_ptr( new T( to_add ) );
		all_segments.push_back( to_add_ptr );

		vertex* current_v = choose_leaf( &( *to_add_ptr ) );
		current_v->content.push_back( &*to_add_ptr );
		current_v->resolve_edges();
		if( current_v->parent != NULL )
			current_v->parent->children.remove( current_v );
		if( current_v->content.size() > max_content_in_node )
		{
			vertex* new_left = new vertex();
			vertex* new_right = new vertex();

			quadratic_split( current_v, new_left, new_right );
			adjust_tree( current_v->parent, new_left, new_right );
			delete current_v;
		}
		else
		{
			adjust_tree( current_v->parent, current_v, NULL );
		}
	}
	template< class T >
	template< class U >
	void tree< T >::find_segments( const box& query, std::back_insert_iterator< U > res ) const
	{
		find_segments( query, res, root_ );
	}
	template< class T >
	template< class U >
	void tree< T >::find_segments( const box& query, std::back_insert_iterator< U > res, const vertex* current_v ) const
	{
		if( current_v->is_leaf() )
		{
			BOOST_FOREACH( const T* segment, current_v->content )
				if( segment->in_polygon( query ) )
					( *res++ ) = *segment;
		}
		else
		{
			BOOST_FOREACH( const vertex* child, current_v->children )
				if( child->edges.overlaps( query ) )
					find_segments( query, res, child );
		}
	}
	template< typename  T >
	void tree< T >::quadratic_split( vertex* to_split, vertex* group1, vertex* group2 ) const
	{
		if( to_split->is_leaf() )
			quadratic_split< T* >( to_split->content, group1->content, group2->content );
		else
			quadratic_split< node< T >* >( to_split->children, group1->children, group2->children );

		group1->resolve_edges();
		group2->resolve_edges();
	}
	template< class T >
	template< typename U >
	void tree< T >::quadratic_split( std::list< U >& to_split, std::list< U >& group1, std::list< U >& group2 ) const
	{
		typedef std::list< U > ContentType;
		ContentType::iterator first_found, second_found;
		int max_enlargement = -1;
		for( ContentType::iterator cit = to_split.begin(); cit != to_split.end(); ++cit  )
		{
			ContentType::iterator cjt = cit;
			++cjt;
			for( ; cjt != to_split.end(); ++cjt )
			{
				const U& first = *cit;
				const U& second = *cjt;
				box new_bounds = box::build_polygon( boost::assign::list_of ( first->get_edges().low_left )
																			( first->get_edges().top_right )
																			( second->get_edges().low_left )
																			( second->get_edges().top_right ) );

				int enlargement = new_bounds.square() - first->get_edges().square() - second->get_edges().square();
				if( enlargement > max_enlargement )
				{
					first_found = cit;
					second_found = cjt;
					max_enlargement = enlargement;
				}
			}
		}
		
		box group1_edges = ( *first_found )->get_edges();
		box group2_edges = ( *second_found )->get_edges();
		group1.push_back( *first_found );
		group2.push_back( *second_found );
		to_split.erase( first_found );
		to_split.erase( second_found );

		while( !to_split.empty() )
		{
			if( group1.size() > group2.size() )
				std::swap( group1, group2 );
			if( to_split.size() == 1 )
			{
				group1.push_back( to_split.front() );
				detail::update_edges( group1_edges, to_split.front()->get_edges() );
				break;
			}

			ContentType::iterator next_entry;
			int max_difference = std::numeric_limits< int >::min();
			int enlarge1;
			int enlarge2;

			for( ContentType::iterator cit = to_split.begin(); cit != to_split.end(); ++cit )
			{
				const U& element = *cit;
				int d1 = box::build_polygon( boost::assign::list_of ( group1_edges.low_left )
																	( group1_edges.top_right ) 
																	( element->get_edges().low_left )
																	( element->get_edges().top_right ) ).square() - group1_edges.square();
				int d2 = box::build_polygon( boost::assign::list_of ( group2_edges.low_left )
																	( group2_edges.top_right ) 
																	( element->get_edges().low_left )
																	( element->get_edges().top_right ) ).square() - group2_edges.square();

				if( d1 - d2 > max_difference )
				{
					max_difference = d1 - d2;
					enlarge1 = d1;
					enlarge2 = d2;
					next_entry = cit;
				}
			}

			if( enlarge1 < enlarge2 )
			{
				detail::update_edges( group1_edges, to_split.front()->get_edges() );
				group1.push_back( *next_entry );
			}
			else
			{
				detail::update_edges( group1_edges, to_split.front()->get_edges() );
				group2.push_back( *next_entry );
			}

			to_split.erase( next_entry );
		}
	}
	template< class T >
	typename tree< T >::vertex* tree< T >::choose_leaf( const T* to_add )
	{
		vertex* result = root_;
		while( !result->is_leaf() )
		{
			unsigned min_enlargement = std::numeric_limits< unsigned >::max();
			vertex* chosen_path = NULL;
			BOOST_FOREACH( vertex* child, result->children )
			{
				box new_bounds = box::build_polygon( boost::assign::list_of ( to_add->get_start() )
																			( to_add->get_end() )
																			( child->edges.low_left )
																			( child->edges.top_right ) );
				unsigned enlargement = new_bounds.square() - child->edges.square();
				if( enlargement < min_enlargement )
				{
					min_enlargement = enlargement;
					chosen_path = child;
				}
			}
			result = chosen_path;
		}

		return result;
	}
	template< class T >
	void tree< T >::adjust_tree( vertex* current_v, vertex* child1, vertex* child2 )
	{
		if( current_v == NULL )
			return;
		vertex* parent = current_v->parent;

		current_v->children.push_back( child1 );
		if( child2 != NULL )
			current_v->children.push_back( child2 );
		current_v->resolve_edges();

		if( parent != NULL )
			parent->children.remove( current_v );
		if( current_v->children.size() > max_content_in_node )
		{
			vertex* new_left = new vertex();
			vertex* new_right = new vertex();

			quadratic_split( current_v, new_left, new_right );
			if( parent == NULL )
			{
				root_ = new vertex();
				root_->children.push_back( new_left );
				root_->children.push_back( new_right );
			}
			else
			{
				adjust_tree( current_v->parent, new_left, new_right );
			}
			delete current_v;
		}
		else 
		{
			adjust_tree( current_v->parent, current_v, NULL );
		}
	}
}

#endif //_TREE_H_DEFINED_