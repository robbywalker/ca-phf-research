#pragma once

#include "../tabu/arrays/tuple.h"
#include "../tabu/cphf/iterable_utuple.h"

#include <map>
#include <list>
#include <set>
#include <utility>

namespace coverage {
	typedef std::set<int> intSet;
	typedef std::map<cphf::iterable_utuple, intSet> tupleCovering;	

	class memoryStore {
		int rows, columns, symbols, strength;
		std::map<arrays::tuple, tupleCovering> covered;

	public:
		memoryStore( int rows, int columns, int symbols, int strength ) {
			this->rows = rows;
			this->columns = columns;
			this->symbols = symbols;
			this->strength = strength;
		}

		void setCovered( const arrays::tuple & t, const cphf::iterable_utuple & u, int row ) {
			covered[t][u].insert( row );
		}

		void unsetCovered( const arrays::tuple & t, const cphf::iterable_utuple & u, int row ) {
			covered[t][u].erase( row );
		}

		bool isCovered( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			return ( covered[t][u].size() != 0 );
		}

		bool isUnique( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			return ( covered[t][u].size() == 1 );
		}

		int getUnique( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			return ( * covered[t][u].begin() );
		}
	};

}