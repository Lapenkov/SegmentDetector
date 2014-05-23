#include <cstdlib>
#include "tree.h"

int main()
{
	using namespace storage;
	typedef segment< std::string > segment_type; 
	typedef segment_type::point_type point_type;

	tree< segment_type > t;
	t.add_segment( segment_type( point_type(), point_type() ) );
	std::vector< segment_type > f = t.find_segments( point_type(), point_type() );

	return 0;
}