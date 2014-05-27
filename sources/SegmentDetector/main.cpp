#include <cstdlib>
#include "tree.h"

int main()
{
	using namespace storage;

	tree< segment< int > > t;
	std::vector< segment< int > > res;
	t.add_segment( segment< int >( point(), point() ) );
	t.find_segments( box(), std::back_inserter( res ) );
	return 0;
}