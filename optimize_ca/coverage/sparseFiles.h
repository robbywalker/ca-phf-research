#pragma once

#include "../tabu/arrays/tuple.h"
#include "../tabu/cphf/iterable_utuple.h"
#include "../tabu/util/util.h"

#include "../coverage/setCache.h"

#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>

namespace coverage {

	class sparseFileStore {
		int rows, columns, symbols, strength;

		int bytesPerUtuple;
		char * dir;

		bool empty;

		setCache setC;

	public:
		sparseFileStore( int rows, int columns, int symbols, int strength, bool create ) {
			this->rows = rows;
			this->columns = columns;
			this->symbols = symbols;
			this->strength = strength;

			dir = new char[255];
			sprintf(dir, "%d-%d-%d-%d.tmp", rows, columns, symbols, strength );
			
			if ( ! _mkdir( dir ) ) {
				empty = true;
				if ( !create ) {
					_rmdir( dir );
					printf("Directory '%s' does not exist.\n", dir);
					printf("Please run the analyze_ca tool first.\n", dir);
					exit( 1 );
				}
			} else {
				empty = false;
			}

			strcat( dir, "\\" );

			setC.setDir( dir );
		}

		bool isEmpty() {
			return empty;
		}

		void setCovered( const arrays::tuple & t, const cphf::iterable_utuple & u, int row ) {
			setId id(t,u);

			intSet * set = setC.getForModification( id );
			set->insert( row );
		}

		void unsetCovered( const arrays::tuple & t, const cphf::iterable_utuple & u, int row ) {
			setId id(t,u);

			intSet * set = setC.getForModification( id );
			set->erase( row );
		}

		bool isCovered( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			return getCount( t, u ) > 0;
		}

		bool isUnique( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			return getCount( t, u ) == 1;
		}

		int getUnique( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			setId id(t,u);

			intSet * set = setC.get( id );

			return (set->size() == 1) ? (*(set->begin())) : -1;
		}

		int getCount( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			setId id(t,u);

			intSet * set = setC.get( id );			
			return (int) set->size();
		}

		int getDuo( const arrays::tuple & t, const cphf::iterable_utuple & u, int ignore ) {
			setId id(t,u);

			intSet * set = setC.get( id );
			if ( set->size() != 2 )
				return -1;

			intSet::iterator it = set->begin();

			return (*it == ignore) ? *(++it) : *it;
		}

		void persist() {
			setC.flush();
		}
	};

}