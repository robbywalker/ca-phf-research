#pragma once

#include "../arrays/array.h"
#include "../arrays/tuple.h"
#include "../arrays/symbol.h"
#include "../arrays/intsymbol.h"
#include "../tabu/search.h"
#include "../alo/tabustatus.h"
#include "../multicheck/multiTabuStatus.h"
#include <set>

namespace alo_ofeach {
	typedef std::set<int> rowset;

	template <class Symbol, class It = Symbol *>
	class equal_Check {		
		It first;
	public:
		equal_Check( const It & _first ) : first( _first ) {
		}		

		bool good( const Symbol * slice, int size ) {
			It cur = first;

			for ( int i = 0; i < size; i++, cur++ ) {
				if ( slice[i] != (*cur) ) {
					return false;
				}
			}
			return true;
		}
		
	};

	template <class Symbol, class Tuple=arrays::tuple>
	class tabuStatus : public multicheck::multiTabuStatus<Symbol> {
	public:
		tabuStatus() {
		}

		tabuStatus( arrays::Array<Symbol> & arr, int strength, bool initialize, Symbol ** searches, int count ) : 
			multicheck::multiTabuStatus<Symbol>( arr.getWidth(), strength )
		{
			this->count = count;
			statuses = new tabu::tabuStatus<Symbol> * [ count ];

			for ( int i = 0; i < count; i++ ) {
				equal_Check<Symbol> * checker = new equal_Check<Symbol>( searches[i] );
				statuses[i] = new alo::tabuStatus<equal_Check<Symbol>, Symbol, Tuple>( arr, strength, initialize, checker );
			}
		}

		tabuStatus( arrays::Array<Symbol> & arr, int strength, bool initialize, Symbol ** searches, int count, const typename Tuple::initParams & params ) : 
			multicheck::multiTabuStatus<Symbol>( arr.getWidth(), strength )
		{
			this->count = count;
			statuses = new tabu::tabuStatus<Symbol> * [ count ];

			for ( int i = 0; i < count; i++ ) {
				equal_Check<Symbol> * checker = new equal_Check<Symbol>( searches[i] );
				statuses[i] = new alo::tabuStatus<equal_Check<Symbol>, Symbol, Tuple>( arr, strength, initialize, params, checker );
			}
		}

		~tabuStatus() {
		}
	};
};