#ifndef _TREE_H_DEFINED_
#define _TREE_H_DEFINED_

#include "node.h"
#include "simple_types.h"

#include <vector>
#include <iterator>

#include <boost/range.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/unordered_set.hpp>

namespace storage
{
	template< class T  >
	class tree
	{
	private:
		static const size_t max_content_in_node = 4ul;

		typedef std::vector< T > segment_arr;
		typedef node< T > vertex;
		vertex* top_;

	public:
		explicit tree( const segment_arr& segments );
		explicit tree();

		void add_segment( const T& to_add );
		template< class U > 
		void find_segments( const point& left_bottom, const point& right_top, std::back_insert_iterator< U >, vertex* current_v = NULL ) const; 
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
	}
	template< class T >
	template< class U >
	void tree< T >::find_segments( const point& left_bottom, const point& right_top, std::back_insert_iterator< U >, vertex* current_v ) const
	{
		
	}
}

#endif //_TREE_H_DEFINED_