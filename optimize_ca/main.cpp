#include "../tabu/arrays/array.h"
#include "../tabu/arrays/intsymbol.h"
#include "../tabu/arrays/tuple.h"

#include "../tabu/cphf/iterable_utuple.h"

#include "../tabu/util/reportManager.h"

#include "coverage/tabusearcher.h"
#include "../tabu/globals.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>

typedef arrays::int_Symbol SYM;

#define SCORE_EXP 1
#define ALPHA 0.965
#define FACTOR 0.12
#define MAX_TEMP 0.0001

inline double maxd( double a, double b ) {
	if ( a > b ) return a;
	else return b;
}

void main(int argc, char* argv[]) {
	srand( (unsigned)time( NULL ) );

	int syms, strength;
	arrays::Array<SYM> input( argv[1], syms, strength );

	tabuCount = (int) (input.getWidth() * input.getHeight() * syms * 0.02);
	printf("Tabu size is %d.\n", tabuCount);

	coverage::coverage_tabuSearcher searcher(input, syms, strength);

	if ( argc <= 2 ) {
		printf("Beginning tabu search.\n");
		searcher.search();
		printf("Program exiting.\n");
	}
}