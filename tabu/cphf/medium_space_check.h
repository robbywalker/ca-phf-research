// performs CPHF testing using less space but more time
// seems almost necessary for anything harder than t = 4, v = 5

#pragma once

#include <stdlib.h>

#include <vector>
#include <set>
#include <list>
#include <algorithm>

#include "iterable_utuple.h"
#include "utuple.h"

#include "../math/galois_field.h"
#include "../util/util.h"

namespace cphf {

	typedef std::set<int> nonCoveringGroup;

	template <class Symbol>
	class cphf_medium_space_Check {
		typedef std::vector<Symbol> sliceType;	

		static nonCoveringGroup ** nonCoveringDoubles;

		static bool initialized;

	public:
		cphf_medium_space_Check(int t, int base) {
			if ( !initialized )
				init( t, base );
		}

		static void init(int t, int base) {
			std::list<nonCoveringGroup> nonCovering;

			// build the alphas
			// for each alpha, build the "bad" set

			int i, j, k;
			for ( i = 1; i < t; i++ ) {
				// iterate over all selections of alpha_0, alpha_i+1 --> alpha_t-1
				   // i - 1 zeroes
				   // i pieces predetermined
				iterable_utuple it(base, t-i);
				do {
					utuple alpha(t);
					alpha[0] = it[0];
					for ( j = 1; j < i; j++ ) {
						alpha[j] = 0;
					}
					alpha[i] = 1;
					for ( j = i+1; j < t; j++ ) {
						alpha[j] = it[ j - i ];
					}

					// u is now a unique set of valid alphas
					nonCoveringGroup ncg;
					iterable_utuple hIt(base, t-2);

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

					nonCovering.push_back( ncg );

				} while ( it++ );
			}		

			// now, we reverse and double this information
			int tupleCount = util::ipow( base, t-1 );

			nonCoveringDoubles = new nonCoveringGroup *[ tupleCount - 1 ];
			for ( i = 0; i < tupleCount; i++ ) {
				nonCoveringDoubles[i] = new nonCoveringGroup[ tupleCount - i ];
			}

			j = 0;
			std::list<nonCoveringGroup>::const_iterator it;
			for ( it = nonCovering.begin(); it != nonCovering.end(); it++, j++ ) {
				
				nonCoveringGroup::const_iterator gIt;
				for ( gIt = it->begin(); gIt != it->end(); gIt++ ) {
					nonCoveringDoubles[*gIt][0].insert( j );
				}
			}

			/*for ( i = 0; i < tupleCount; i++ ) {
				printf("Size of %d single: %d\n", i, nonCoveringDoubles[i][0].size());
			}*/

			for ( i = 0; i < tupleCount; i++ ) {
				for ( j = i+1; j < tupleCount; j++ ) {
					nonCoveringGroup::const_iterator itI, itJ;
					itI = nonCoveringDoubles[i][0].begin();
					itJ = nonCoveringDoubles[j][0].begin();

					while ( itI != nonCoveringDoubles[i][0].end() && itJ != nonCoveringDoubles[j][0].end() ) {
						if ( *itI == *itJ ) {
							nonCoveringDoubles[i][j-i].insert( *itI );
							itI++; itJ++;
						} else if ( *itI < *itJ ) {
							itI++;
						} else {
							itJ++;
						}
					}
				}
			}

			/*for ( i = 0; i < tupleCount; i++ ) {
				for ( j = i+1; j < tupleCount; j++ ) {
					printf("Size of %d, %d double: %d\n", i, j, nonCoveringDoubles[i][j-i].size());
				}
			}*/
		}

		bool good( const sliceType & slice ) {
			int i, j;
			int size = (int) slice.size();
			for ( i = 0; i < (size - 1); i++ ) {
				for ( j = i+1; j < size; j++ ) {
					if ( slice[i] == slice[j] ) {
						return false;
					}
				}
			}

			// we go two by two checking for intersections
			nonCoveringGroup curSet;
			if ( slice[0].asInt() < slice[1].asInt() ) {
				curSet = nonCoveringDoubles[ slice[0].asInt() ][ slice[1].asInt() - slice[0].asInt() ];
			} else {
				int x = slice[0].asInt();
				int y = slice[1].asInt();
				curSet = nonCoveringDoubles[ slice[1].asInt() ][ slice[0].asInt() - slice[1].asInt() ];
			}

			i = 2;
			while ( true ) {
				nonCoveringGroup::const_iterator nextSetIt, nextSetEnd;

				if ( i + 1 == size ) {
					nextSetIt = nonCoveringDoubles[ slice[i].asInt() ][0].begin();
					nextSetEnd = nonCoveringDoubles[ slice[i].asInt() ][0].end();
				} else {
					if ( slice[i].asInt() < slice[i+1].asInt() ) {
						nextSetIt = nonCoveringDoubles[ slice[i].asInt() ][ slice[i+1].asInt() - slice[i].asInt() ].begin();
						nextSetEnd = nonCoveringDoubles[ slice[i].asInt() ][ slice[i+1].asInt() - slice[i].asInt() ].end();
					} else {
						nextSetIt = nonCoveringDoubles[ slice[i+1].asInt() ][ slice[i].asInt() - slice[i+1].asInt() ].begin();
						nextSetEnd = nonCoveringDoubles[ slice[i+1].asInt() ][ slice[i].asInt() - slice[i+1].asInt() ].end();
					}
				}

				i += 2;
				bool last = i >= size;

				nonCoveringGroup tempSet;
				nonCoveringGroup::const_iterator curSetIt = curSet.begin();

				while ( curSetIt != curSet.end() && nextSetIt != nextSetEnd ) {
					if ( *curSetIt == *nextSetIt ) {
						if ( last ) 
							return false;

						tempSet.insert( *curSetIt );
						curSetIt++; nextSetIt++;
					} else if ( *curSetIt < *nextSetIt ) {
						curSetIt++;
					} else {
						nextSetIt++;
					}
				}

				if ( last ) {
                    return true;
				}

				curSet = tempSet;
			}
		}		
	};

	template <class Symbol>
	nonCoveringGroup ** cphf_medium_space_Check<Symbol> :: nonCoveringDoubles;

	template <class Symbol>
	bool cphf_medium_space_Check<Symbol> :: initialized = false;

};