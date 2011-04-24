#include "../tabu/arrays/array.h"
#include "../tabu/arrays/intsymbol.h"
#include "../tabu/arrays/tuple.h"

#include "../tabu/cphf/utuple.h"

#include "../tabu/util/util.h"

#include <stdlib.h>
#include <time.h>

#include <set>

typedef arrays::int_Symbol SYM;

void main(int argc, char* argv[]) {
	int syms, strength;
	arrays::Array<SYM> input( argv[1], syms, strength );

	input.print();

	printf("\n\n");

	int max = util::ipow( syms, strength );
	int cols = input.getWidth();

	std::vector<SYM> slice(strength);
	arrays::tuple t(strength, cols);
	cphf::utuple u(strength);
	int i, j;
	do {
		std::set<cphf::utuple> covered;
		for ( i = 0; (i < input.getHeight()) && (max > (int) covered.size()); i++ ) {
			input.sliceInto(i, t, slice);
			for ( j = 0; j < strength; j++ ) {
				u[j] = slice[j].asInt();
			}

			covered.insert( u );
		}

		if ( (int) covered.size() != max ) {
			printf("Failed at tuple: ");
			t.print();
			printf("\n");
			printf("Covered only:\n");

			std::set<cphf::utuple>::const_iterator it = covered.begin();
			for ( ; it != covered.end(); it++ ) {
				it->print();
				printf("\n");
			}
			return;
		}
	} while ( ++t );

	printf("Success!");
}