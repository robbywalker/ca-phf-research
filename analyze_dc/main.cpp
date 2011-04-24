#include "../tabu/arrays/array.h"
#include "../tabu/arrays/intsymbol.h"

#include <stdlib.h>
#include "../tabu/util/reportManager.h"

typedef arrays::int_Symbol SYM;

void main(int argc, char* argv[]) {
	int syms, strength;
	arrays::Array<SYM> input( argv[1], syms, strength );

	int i, j;
	int height = input.getHeight();

	int * rowCounts;
	int ** posCounts;

	rowCounts = new int[ input.getHeight() ];
	posCounts = new int*[ input.getHeight() ];
	for ( i = 0; i < input.getHeight(); i++ ) {
		rowCounts[i] = 0;
		posCounts[i] = new int[ input.getWidth() ];
		for ( j = 0; j < input.getWidth(); j++ ) {
			posCounts[i][j] = 0;
		}
	}

	int tupleCount = util::ipow( syms, strength );
	int * state = new int[tupleCount];

	int ** arr;
	arr = new int*[height];
	for ( i = 0; i < height; i++ ) {
		arr[i] = new int[input.getWidth()];
		for ( j = 0; j < input.getWidth(); j++ ) {
			arr[i][j] = input.get(i,j).asInt();
		}	
	}

	util::reportManager report;
	printf("Analyzing the array.\n");

	arrays::tuple t(strength, input.getWidth());

	int accum;

	int repC = 0;

	do {
		// reporting
		if ( repC == 100 ) {
			if ( report.reportRequired() ) {
				printf("Current tuple: ");
				t.print();
				printf("\n");

				report.reported();
			}
			repC = 0;
		} else {
			repC++;
		}

		// reset state
		for ( i = 0; i < tupleCount; i++ ) {
			state[i] = -1;
		}
		
		// iterate through rows
		for ( i = 0; i < height; i++ ) {
			accum = 0;
			for ( j = 0; j < strength; j++ ) {
				accum *= syms;
				accum += arr[i][t[j]];
			}

			if ( state[accum] == -1 ) {
				state[accum] = i;

			} /* else if ( state[accum] != -2 ) {
				state[accum] = -2;
			} */
		}

		// analyze results
		for ( i = 0; i < tupleCount; i++ ) {
			if ( state[i] >= 0 ) {
				for ( j = 0; j < strength; j++ ) {
					posCounts[ state[i] ][ t[ j ] ]++;
				}

				rowCounts[ state[i] ]++;
			} else if ( state[i] == -1 ) {
				printf("The array is not a covering array - no coverage found for tuple %d in columns ", i);
				t.print();
				printf("\n");
				exit(1);
			}
		}
	} while ( ++t );

	printf("%d %d %d %d\n", input.getHeight(), input.getWidth(), syms, strength );
	for ( i = 0; i < input.getHeight(); i++ ) {
		for ( j = 0; j < input.getWidth(); j++ ) {
			if ( posCounts[i][j] == 0 ) {
				printf("X");
			} else {
				printf("%d", input.get(i,j));
			}

			if ( j != input.getWidth() - 1 ) {
				printf(" ");
			}
		}
		printf("\n");
	}
}