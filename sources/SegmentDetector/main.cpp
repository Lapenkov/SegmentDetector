#include <cstdlib>
#include <fstream>

#include "tree.h"

int main()
{
	using namespace storage;

	typedef segment< int > seg;
	std::ifstream in( SOURCE_DIR"/input.txt" );

	int n, a, b, c, d;
	in >> n;
	tree< seg > t;
	for( int i = 0; i < n; ++i )
	{
		in >> a >> b >> c >> d;
		t.add_segment( seg( point(a ,b), point(c, d), i ) );
	}

	in >> a >> b >> c >> d;
	std::vector< seg > res;
	t.find_segments( box(point(a, b), point(c, d)), std::back_inserter( res ) );
	for( int i = 0; i < res.size(); ++i )
		std::cout << res[i].get_info();
	return 0;
}