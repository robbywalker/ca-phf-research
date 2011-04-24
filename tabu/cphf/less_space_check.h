// performs CPHF testing using less space but more time
// seems almost necessary for anything harder than t = 4, v = 5

#pragma once

#include <stdlib.h>

#include <vector>
#include <set>
#include <list>

#include "iterable_utuple.h"
#include "utuple.h"

#include "../math/galois_field.h"
#include "../util/util.h"

namespace cphf {

	typedef std::set<int> nonCoveringGroup;

	template <class Symbol>
	class cphf_less_space_Check {
		typedef std::vector<Symbol> sliceType;
		typedef Symbol* arrayType;

		static std::list<nonCoveringGroup> nonCovering;
		static bool initialized;

	public:
		cphf_less_space_Check(int t, int base) {
			if ( !initialized )
				init(t, base);
		}

		static void init(int t, int base) {
			initialized = true;

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
		}

		inline bool good( const sliceType & slice ) {
			int i, j;
			int size = (int) slice.size();
			for ( i = 0; i < (size - 1); i++ ) {
				for ( j = i+1; j < size; j++ ) {
					if ( slice[i] == slice[j] ) {
						return false;
					}
				}
			}

			// if all the elements of our slice are in the same nonCovering set,
			// we return false - else return true
			std::list<nonCoveringGroup>::const_iterator it = nonCovering.begin();
			for ( ; it != nonCovering.end(); it++ ) {
				for ( i = 0; i < (int) slice.size(); i++ ) {
					if ( it->count( slice[i].asInt() ) == 0 ) 
						break;
				}
				if ( i == (int) slice.size() )
					return false;
			}
			return true;
		}

		inline bool good( const arrayType slice, int size ) {
			int i, j;
			for ( i = 0; i < (size - 1); i++ ) {
				for ( j = i+1; j < size; j++ ) {
					if ( slice[i] == slice[j] ) {
						return false;
					}
				}
			}

			// if all the elements of our slice are in the same nonCovering set,
			// we return false - else return true
			std::list<nonCoveringGroup>::const_iterator it = nonCovering.begin();
			for ( ; it != nonCovering.end(); it++ ) {
				for ( i = 0; i < size; i++ ) {
					if ( it->count( slice[i].asInt() ) == 0 ) 
						break;
				}
				if ( i == (int) size )
					return false;
			}
			return true;
		}
		
	};

	template <class Symbol>
	std::list<nonCoveringGroup> cphf_less_space_Check<Symbol> :: nonCovering;

	template <class Symbol>
	bool cphf_less_space_Check<Symbol> :: initialized = false;

};