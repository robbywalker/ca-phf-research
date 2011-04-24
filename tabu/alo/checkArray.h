#pragma once

#include "../arrays/array.h"
#include "../arrays/tuple.h"
#include <vector>

namespace alo {
	template <class RowChecker, class Symbol>
	class alo_Check {
	public:
		static bool good( const arrays::Array<Symbol> & arr, int strength ) {
			int v = Symbol::count;
			int k = arr.getWidth();
			int N = arr.getHeight();

			if ( strength > k ) {
				return true;
			}
			
			std::vector<Symbol> slice(strength);
			arrays::tuple t(strength, k);

			int i;
			bool found;
			do {
				// FIRST CHECK THE CURRENT T-TUPLE
				found = false;

				for ( i = 0; i < N; i++ ) {
					arr.sliceInto(i, t, slice);
					if ( RowChecker::good( slice ) ) {
						found = true;
						break;
					}
				}
                
				if ( ! found ) {
					for ( i = 0; i < strength; i++ ) {
						printf("%d", t[i] );
						if ( i != ( strength - 1 ) ) {
							printf(" ");
						} else {
							printf("\n");
						}
					}
					return false;
				}


				// NOW, INCREMENT THE CURRENT T-TUPLE
			} while ( ++t );

			return true;
		}
	};
}