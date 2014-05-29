#include <cstdlib>
#include "tree.h"

int main()
{
	using namespace storage;

	typedef segment< int > seg;
	tree< seg > t;
	std::vector< seg > res;
	t.add_segment( seg( point(0 ,0), point(1, 1), 132 ) );
	t.find_segments( box(point(-1, -1), point(1, 1)), std::back_inserter( res ) );

	for( int i = 0; i < res.size(); ++i )
		std::cout << res[i].get_info();
	return 0;
}