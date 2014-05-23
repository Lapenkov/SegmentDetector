#ifndef _TREE_H_DEFINED_
#define _TREE_H_DEFINED_

#include "node.h"
#include "simple_types.h"

#include <vector>

#include <boost/range.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/unordered_set.hpp>

namespace storage
{
	template< class T >
	class tree
	{
	private:
		typedef std::vector< T > segment_arr;
		typedef typename T::point_type point_type;
		typedef node< point_type > vertex;
		typedef boost::unordered_set< point_type > point_set;
		vertex* top_;

	public:
		explicit tree( const segment_arr& segments );
		explicit tree();

		void add_segment( const T& to_add );
		void add_point( const point_type& to_add );
		void add_point( const point_type& to_add, vertex* current_v );
		segment_arr find_segments( const point_type& left_bottom, const point_type& right_top ) const; 

	private:
		void find_points( const point_type& left_bottom, const point_type& right_top, vertex* current_v, point_set& found ) const;
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
		add_point( to_add.get_start() );
		add_point( to_add.get_end() );
	}
	template< class T >
	void tree< T >::add_point( const point_type& to_add )
	{
		add_point( to_add, top_ );
	}	
	template< class T >
	void tree< T >::add_point( const point_type& to_add, vertex* current_v )
	{
		if( current_v == NULL )
		{
			current_v = new vertex( to_add );
			return ;
		}

		const point_type& cur_point = current_v->data;
		if( ( to_add.above( cur_point ) || to_add.get_y() == cur_point.get_y() ) && to_add.right_to( cur_point ) )
			add_point( to_add, current_v->upright );
		else if( to_add.above( cur_point ) && ( to_add.left_to( cur_point ) || to_add.get_x() == cur_point.get_x() ) )
			add_point( to_add, current_v->upleft );
		else if( ( to_add.below( cur_point ) || to_add.get_y() == cur_point.get_y() ) && to_add.left_to( cur_point ) )
			add_point( to_add, current_v->downleft );
		else if( to_add.below( cur_point ) && ( to_add.left_to( cur_point ) || to_add.get_x() == cur_point.get_x() ) )
			add_point( to_add, current_v->downright );

	}	
	template< class T >
	typename tree< T >::segment_arr tree< T >::find_segments( const point_type& left_bottom, const point_type& right_top ) const
	{
		point_set found_points;
		segment_arr result;

		find_points( left_bottom, right_top, top_, found_points );

		for( point_set::const_iterator cit = found_points.begin(); cit != found_points.end(); )
		{
			point_set::const_iterator linked_place = found_points.find( *cit->get_linked() );
			if( linked_place != found_points.end() )
			{
				result.push_back( T( point_type(), point_type() ) );
				found_points.erase( linked_place );
				found_points.erase( cit++ );
			}
		}
		
		return result;
	}
	template< class T >
	void tree< T >::find_points( const point_type& left_bottom, const point_type& right_top, vertex* current_v, point_set& found ) const
	{
		if( current_v == NULL )
			return ;

		if( current_v->data.in_polygon( left_bottom, right_top ) )
		{
			found.insert( current_v->data );
			find_points( left_bottom, right_top, current_v->upright, found );
			find_points( left_bottom, right_top, current_v->upleft, found );
			find_points( left_bottom, right_top, current_v->downright, found );
			find_points( left_bottom, right_top, current_v->downleft, found );
		}

	}
}

#endif //_TREE_H_DEFINED_