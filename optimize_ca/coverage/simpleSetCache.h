#pragma once

#include "../tabu/util/cache.h"
#include "../tabu/arrays/tuple.h"
#include "../tabu/arrays/array.h"

#include "../tabu/cphf/iterable_utuple.h"

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <utility>
#include <set>

namespace coverage {
	typedef std::set<int> intSet;

	typedef std::pair< arrays::tuple, cphf::iterable_utuple > setId;
	typedef util::cache< setId, intSet * > setCacheBase;

	template <class SYM>
	class simpleSetCache : public setCacheBase {
		const arrays::Array<SYM> * arr;
		const intSet * removable;
		int h, w;
	public: 
		simpleSetCache( ) : setCacheBase(100000) {			
		}

		void setArray( const arrays::Array<SYM> * arr ) {
			this->arr = arr;
			h = arr->getHeight();
			w = arr->getWidth();
		}

		void setRemovable( const intSet * removable ) {
			this->removable = removable;
		}

		virtual intSet * populate( const setId & id ) {
			intSet * set = new intSet;

			int i, j;
			int s = id.first.strength();
			for ( i = 0; i < h; i++ ) {
				if ( removable->count(i) != 0 )
					continue;

				for ( j = 0; j < s; j++ ) {
					if ( id.second[j] != arr->get(i,id.first[j]).asInt() )
						break;
				}

				if ( j == s )
					set->insert( i );
			}

			return set;
		}

		virtual void persist( const setId & id, intSet * value ) {
		}

		virtual void remove( const setId & id, intSet * value ) {
			delete value;
		}
	};
};