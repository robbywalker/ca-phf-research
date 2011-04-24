// performs CPHF testing using less space but more time
// seems almost necessary for anything harder than t = 4, v = 5

#include <stdlib.h>

#include <set>

#include "../tabu/cphf/utuple.h"
#include "../tabu/cphf/iterable_utuple.h"

#include "../tabu/math/galois_field.h"
#include "../tabu/util/util.h"

typedef std::set<int> nonCoveringGroup;

void main(int argc, char * argv[]) {
	int base = atoi( argv[1] );
	int t = atoi( argv[2] );

	// build the alphas
	// for each alpha, build the "bad" set

	int i, j, k;
	for ( i = 1; i < t; i++ ) {
		// iterate over all selections of alpha_0, alpha_i+1 --> alpha_t-1
			// i - 1 zeroes
			// i pieces predetermined
		cphf::iterable_utuple it(base, t-i);
		do {
			cphf::utuple alpha(t);
			alpha[0] = it[0];
			for ( j = 1; j < i; j++ ) {
				alpha[j] = 0;
			}
			alpha[i] = 1;
			for ( j = i+1; j < t; j++ ) {
				alpha[j] = it[ j - i ];
			}

			alpha.print();

			// u is now a unique set of valid alphas
			nonCoveringGroup ncg;
			cphf::iterable_utuple hIt(base, t-2);

			do {
                // plug in the h's
				// solve the equation to get h_i
				math::GFNum tally(alpha[0], base);
				int iVal = 0;

				k = 1;
				for ( j = 1; j < t; j++ ) {
					if ( j == i )
						continue;

					iVal += util::ipow( base, j-1 ) * hIt[k-1];

					tally += math::GFNum( alpha[j], base ) * math::GFNum( hIt[k-1], base );
					k++;
				}

				int h_i = -tally;
				iVal += util::ipow( base, i-1 ) * h_i;

				ncg.insert( iVal );
				
			} while ( hIt++ );

			printf(" -- ");
			nonCoveringGroup::const_iterator it = ncg.begin();
			for ( ; it != ncg.end(); it++ ) {
				int h = *it;
				for ( int i = 0; i < t-1; i++ ) {
                    printf("%d", h % base);
					h /= base;
				}

				printf(" ", *it);
			}
			printf("\n");			

		} while ( it++ );
	}		
}