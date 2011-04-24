#pragma once

#include "../arrays/array.h"
#include "../arrays/tuple.h"
#include "../arrays/symbol.h"
#include "../tabu/search.h"
#include "tupleMap.h"
#include <set>

namespace alo {
	typedef std::set<int> rowset;

	template <class RowChecker, class Symbol, class Tuple=arrays::tuple>
	class tabuStatus : public tabu::tabuStatus<Symbol> {
		int strength;		
		typename Tuple::initParams params;
		tupleMap<rowset,Tuple> goods;
		std::set<Tuple> bads;
		arrays::Array<Symbol> arr;
		RowChecker * checker;
		Symbol * slice;
		bool keepStrength;
		
	public:
		tabuStatus() {
			slice = NULL;
		}

		tabuStatus( const tabuStatus<RowChecker,Symbol,Tuple> & copy ) :
			strength( copy.strength ),
			params( copy.params ),
			goods( copy.goods ),
			bads( copy.bads ),
			arr( copy.arr ),
			checker( copy.checker ),
			keepStrength( copy.keepStrength )
		{
			slice = new Symbol[strength];
		}

		tabuStatus( arrays::Array<Symbol> & _arr, int _strength, bool initialize ) : 
		  arr(_arr, 1), 
		  params( _strength, _arr.getWidth() ),
		  goods( params ),
		  checker( new RowChecker )
		{
			keepStrength = false;
			strength = _strength;
			slice = new Symbol[strength];

			if ( initialize ) {
				printf("Constructing tabu status!\n");
				init();
			}
		}

		tabuStatus( arrays::Array<Symbol> & _arr, int _strength, bool initialize, RowChecker * _checker ) : 
		  arr(_arr, 1), 
		  params( _strength, _arr.getWidth() ),
		  goods( params ),
		  checker( _checker )
		{
			keepStrength = false;
			strength = _strength;
			slice = new Symbol[strength];

			if ( initialize ) {
				printf("Constructing tabu status!\n");
				init();
			}
		}

		tabuStatus( arrays::Array<Symbol> & _arr, int _strength, bool initialize, const typename Tuple::initParams  & _params ) : 
		  arr(_arr, 1), 
		  goods( _params ),
		  checker( new RowChecker ),
		  params( _params )
		{
			keepStrength = false;
			strength = _strength;
			slice = new Symbol[strength];

			if ( initialize ) {
				printf("Constructing tabu status!\n");
				init();
			}
		}

		tabuStatus( arrays::Array<Symbol> & _arr, int _strength, bool initialize, const typename Tuple::initParams & _params, RowChecker * _checker ) : 
		  arr(_arr, 1), 
		  goods( _params ),
		  checker( _checker ),
		  params( _params )
		{
			keepStrength = false;
			strength = _strength;
			slice = new Symbol[strength];

			if ( initialize ) {
				printf("Constructing tabu status!\n");
				init();
			}
		}

		~tabuStatus() {
			if ( slice != NULL ) {
				delete[] slice;
				slice = NULL;
			}
		}

		void reInit( arrays::Array<Symbol> & _arr, int _strength ) {
			printf("Re-constructing tabu status!\n");
			_arr.copyInto( arr );

			if ( ! keepStrength ) {
				strength = _strength;
				if ( slice != NULL ) delete[] slice;
				slice = new Symbol[_strength];
			}

			init();
		}

		void memorizeStrength() {
			keepStrength = true;
		}

		void init() {
			int v = arr.getSymbolCount();
			int k = arr.getWidth();
			int N = arr.getHeight();
			
			Tuple t(params);

			int i;
			bool found;

			long inc = 0;
			do {
				// FIRST CHECK THE CURRENT T-TUPLE
				found = false;

				for ( i = 0; i < N; i++ ) {
					arr.sliceInto(i, t, slice);
					if ( checker->good( slice, strength ) ) {
						found = true;
						goods[ t ]->insert( i );
					}
				}
                
				if ( ! found ) {
					bads.insert( t );					
				}

				inc++;
				if ( (inc % 10000) == 0 ) {
					printf("at inc %ld with %ld bads\n", inc, (long) bads.size() );
				}

				// NOW, INCREMENT THE CURRENT T-TUPLE
			} while ( ++t );

			printf("Finished construction of tabu status!\n");
		}

		tabu::intScore getScore() {
			return tabu::intScore( (int) bads.size() );
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
			printf("\nTUPLE MAP: \n");
			Tuple t(params);

			do { 
				const rowset * r = goods[ t ];

				t.print();
				printf(" ==> ");

				if ( r->empty() ) {
					printf("XX");
				} else {
					rowset::const_iterator rsIt = r->begin();
					while ( true ) {
						printf("[%d]", *rsIt);

						if ( ++rsIt != r->end() ) {
							printf(" -> ");
						} else {
							break;
						}
					}
				}
				printf("\n");

			} while (++t);
			printf("\n");

			printf("\nBAD TUPLES: \n");
			std::set<Tuple>::const_iterator it = bads.begin();
			for ( ; it != bads.end(); it++ ) {
				it->print();
				printf("\n");
			}
		}

		int countBadPerColumn( int * counts ) {
			int total = 0;
			for ( int i = 0; i < arr.getWidth(); i++ ) {
				counts[i] = 0;
			}

			std::set<Tuple>::const_iterator it = bads.begin();
			for ( ; it != bads.end(); it++ ) {
				int st = it->strength();

				for ( i = 0; i < st; i++ ) {
					counts[ it->get( i ) ]++;
					total++;
				}
			}

			return total;
		}

		int calculateEffectOfChange( int row, int col, Symbol newValue ) {			
			Tuple real( params );
			Tuple::lesserTuple iterator( real.createLesserTuple() );

			int delta = 0;

			do {
				for ( int i = 0; i < strength - 1; i++ ) {
					int xx = iterator[i];
					xx = xx;
				}

				real.initViaUpgrade( iterator, col );

				for ( int i = 0; i < strength; i++ ) {
					int xx = real[i];
					xx = xx;
				}

				int sz = (int) goods[ real ]->size();
				if ( sz == 0 ) {
					// things can get better if this tuple was previously bad
					arr.sliceInto(row,real,slice);
					slice[ real.indexOf( col ) ] = newValue;

					if ( checker->good( slice, strength ) )
						delta--;

				} else if ( sz == 1 ) {
					// things can get worse if this tuple was previously good, but not covered
					arr.sliceInto(row,real,slice);

					if ( checker->good( slice, strength ) ) {
						slice[ real.indexOf( col ) ] = newValue;

						if ( ! checker->good( slice, strength ) )
							delta++;
					}
				}

			} while ( ++iterator );

			return delta;
		}		

		int change( int row, int col, Symbol newValue ) {
			arr.set( row, col, newValue );

			Tuple real( params );
			Tuple::lesserTuple iterator( real.createLesserTuple() );

			int delta = 0;

			do {
				real.initViaUpgrade( iterator, col );
				arr.sliceInto(row,real,slice);

				bool wasBad = ( goods[ real ]->size() == 0 );
				
				if ( checker->good( slice, strength ) ) {
					goods[ real ]->insert( row );
					if ( wasBad ) {
						bads.erase( real );
					}
				} else {
					goods[ real ]->erase( row );
					if ( !wasBad && goods[ real ]->size() == 0 ) {
						bads.insert( real );
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