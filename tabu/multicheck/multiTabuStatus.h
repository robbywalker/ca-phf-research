#pragma once

#include "../arrays/array.h"
#include "../arrays/tuple.h"
#include "../arrays/symbol.h"
#include "../arrays/intsymbol.h"
#include "../tabu/search.h"
#include "../tabu/tabustatus.h"
#include "../alo/tabustatus.h"
#include <set>

namespace multicheck {
	template <class Symbol>
	class multiTabuStatus : public tabu::tabuStatus<Symbol> {		
	protected:
		int count, width, strength;
		tabu::tabuStatus<Symbol> ** statuses;

		multiTabuStatus( int _width, int _strength ) : width(_width), strength(_strength) {
		}

		void deleteStatuses() {
			for ( int i = 0; i < count; i++ ) {
				delete statuses[i];
			}
		}

	public:

		multiTabuStatus( tabu::tabuStatus<Symbol> ** _statuses, int _count, int _width, int _strength ) 
			: statuses( _statuses ), count( _count ), width( _width), strength( _strength )
		{
		}

		void reInit( arrays::Array<Symbol> & _arr, int _strength ) {
			for ( int i = 0; i < count; i++ ) {
				statuses[i]->reInit( _arr, _strength );
			}
		}

		tabu::intScore getScore() {
			int score = 0;

			for ( int i = 0; i < count; i++ ) {				
				score += statuses[i]->getScore().asInt();
			}

			return tabu::intScore( score );
		}

		void getResultInto(arrays::Array<Symbol> & s) {
			statuses[0]->getResultInto( s );
		}

		Symbol getArrayValue( int i, int j ) const {
			return statuses[0]->getArrayValue( i, j );
		}

		arrays::SymbolGenerator<Symbol> * getSymbolGenerator() {
			return statuses[0]->getSymbolGenerator();
		}

		const arrays::Array<Symbol> & getArr() {
			return statuses[0]->getArr();
		}

		const arrays::Array<Symbol> * getArrayPtr() {
			return statuses[0]->getArrayPtr();
		}

		void print() const {
			printf("Not yet implemented.\n");
		}

		int countBadPerColumn( int * counts ) {
			int * tempCounts = new int[width];

			int total = 0;
			for ( int i = 0; i < width; i++ ) {
				counts[i] = 0;
			}

			for ( i = 0; i < count; i++ ) {
				total += statuses[i]->countBadPerColumn( tempCounts );
				for ( int j = 0; j < width; j++ ) {
					counts[j] += tempCounts[j];
				}
			}			

			delete[] tempCounts;

			return total;
		}

		int calculateEffectOfChange( int row, int col, Symbol newValue ) {
			int delta = 0;

			for ( int i = 0; i < count; i++ ) {
				delta += statuses[i]->calculateEffectOfChange( row, col, newValue );
			}

			return delta;
		}		

		int change( int row, int col, Symbol newValue ) {
			int delta = 0;

			for ( int i = 0; i < count; i++ ) {
				delta += statuses[i]->change( row, col, newValue );
			}

			return delta;
		}	

		int getStrength() {
			return strength;
		}

		int getSymbolCount() {
			return statuses[0]->getSymbolCount();
		}
	};
};