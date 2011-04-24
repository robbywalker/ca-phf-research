#pragma once

#include "../arrays/array.h"
#include "../arrays/tuple.h"
#include "../arrays/symbol.h"
#include "../tabu/search.h"
#include "../alo/tupleMap.h"
#include <set>

namespace every {
	typedef std::set<int> rowset;

	template <class RowChecker, class Symbol>
	class tabuStatus : public tabu::tabuStatus<Symbol> {
		int strength;
		long total;
		int score;
		tupleMap<rowset> goods;
		arrays::Array<Symbol> arr;
		RowChecker * checker;
	public:
		tabuStatus() {
		}

		tabuStatus( arrays::Array<Symbol> & _arr, int _strength, bool initialize ) : 
		  arr(_arr, 1), 
		  goods(_strength, _arr.getWidth()),
		  checker( new RowChecker ),
		  total( choose( _arr.getWidth(), _strength ) * _arr.getHeight() )
		{
			strength = _strength;

			if ( initialize ) {
				printf("Constructing tabu status!\n");
				init();
			}

			
		}

		tabuStatus( arrays::Array<Symbol> & _arr, int _strength, bool initialize, RowChecker * _checker ) : 
		  arr(_arr, 1), 
		  goods(_strength, _arr.getWidth()),
		  checker( _checker ),
		  total( choose( _arr.getWidth(), _strength ) * _arr.getHeight() )
		{
			strength = _strength;

			if ( initialize ) {
				printf("Constructing tabu status!\n");
				init();
			}
		}

		void reInit( arrays::Array<Symbol> & _arr, int _strength ) {
			printf("Re-constructing tabu status!\n");
			_arr.copyInto( arr );

			strength = _strength;
			total = choose( _arr.getWidth(), _strength ) * _arr.getHeight();

			init();
		}

		void init() {
			int v = arr.getSymbolCount();
			int k = arr.getWidth();
			int N = arr.getHeight();

			std::vector<Symbol> slice(strength);
			arrays::tuple t(strength, k);

			int i;
			long good = 0;

			long inc = 0;
			do {
				// FIRST CHECK THE CURRENT T-TUPLE
				for ( i = 0; i < N; i++ ) {
					arr.sliceInto(i, t, slice);
					if ( checker->good( slice ) ) {
						good++;
						goods[ t ]->insert( i );
					}
				}
                
				inc++;
				if ( (inc % 10000) == 0 ) {
					printf("at inc %ld with %ld bads\n", inc, (long) bads.size() );
				}

				// NOW, INCREMENT THE CURRENT T-TUPLE
			} while ( ++t );

			score = (int) (total - good);

			printf("Finished construction of tabu status!\n");
		}

		tabu::intScore getScore() {
			return score;
		}

		void getResultInto(arrays::Array<Symbol> & s) {
			arr.copyInto( s );
		}

		Symbol getArrayValue( int i, int j ) const {
			return arr.get(i, j);
		}

		arrays::SymbolGenerator<Symbol> * getSymbolGenerator() {
			return arr.gen;
		}

		const arrays::Array<Symbol> & getArr() {
			return arr;
		}

		const arrays::Array<Symbol> * getArrayPtr() {
			return &arr;
		}

		void print() const {
			printf("Not yet implemented.\n");
		}

		int countBadPerColumn( int * counts ) {
			int i;
			int total = 0;
			for ( i = 0; i < arr.getWidth(); i++ ) {
				counts[i] = 0;
			}

			arrays::tuple t(strength, k);

			do {
				int bad = arr.getHeight() - goods[t]->size();
				total += bad * strength;

				for ( i = 0; i < strength; i++ ) {
					counts[ t[i] ] += bad;
				}

			} while ( ++t );

			return total;
		}

		int calculateEffectOfChange( int row, int col, Symbol newValue ) {
			arrays::tuple iterator( strength - 1, arr.getWidth() - 1 );
			arrays::tuple real( strength, arr.getWidth() );

			std::vector<Symbol> slice(strength);

			int delta = 0;
			int row;

			do {
				real.initViaUpgrade( iterator, col );

				arr.sliceInto(row,real,slice);
				slice[ real.indexOf( col ) ] = newValue;

				if ( goods[ real ]->count( row ) == 0 ) {
					// things can get better if this tuple and row was previously bad
					if ( checker->good( slice ) )
						delta--;

				} else {
					// things can get worse if this tuple and row was previously good
					if ( ! checker->good( slice ) )
						delta++;
				}
			} while ( ++iterator );

			return delta;
		}		

		int change( int row, int col, Symbol newValue ) {
			arr.set( row, col, newValue );

			arrays::tuple iterator( strength - 1, arr.getWidth() - 1 );
			arrays::tuple real( strength, arr.getWidth() );

			std::vector<Symbol> slice(strength);

			int delta = 0;

			do {
				real.initViaUpgrade( iterator, col );
				arr.sliceInto(row,real,slice);

				if ( checker->good( slice ) ) {
					if ( goods[ real ]->count( row ) == 0 ) {
						goods[ real ]->insert( row );
						score--;
					}
				} else {
					if ( goods[ real ]->count( row ) != 0 ) {
						goods[ real ]->erase( row );
						score++;
					}
				}

			} while ( ++iterator );

			return delta;
		}	

		int getStrength() {
			return strength;
		}

		int getSymbolCount() {
			return arr.getSymbolCount();
		}
	};
};