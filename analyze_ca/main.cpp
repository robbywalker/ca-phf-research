#include "../tabu/arrays/array.h"
#include "../tabu/arrays/intsymbol.h"

#include "../optimize_ca/coverage/state.h"

#include <stdlib.h>

#include <map>
#include <list>

typedef arrays::int_Symbol SYM;
typedef std::list<int> intList;

void main(int argc, char* argv[]) {
	int syms, strength;
	arrays::Array<SYM> input( argv[1], syms, strength );

	char dir[255];
	sprintf(dir, "%d-%d-%d-%d.tmp", input.getHeight(), input.getWidth(), syms, strength );
			
	coverage::state cState(input, syms, strength);

	printf("Completed analysis.\nWriting cached data to disk.\n");
	cState.flush();
	printf("Finished cache data write.\n\n");

	for ( int i = 0; i < input.getHeight(); i++ ) {
		printf("Row: %d; Count: %d\n", i+1, cState.getRowCount(i));
	}

	printf("\n%d elements of %d are flexible\n", cState.getFlexibleCount(), input.getHeight() * input.getWidth() );

}