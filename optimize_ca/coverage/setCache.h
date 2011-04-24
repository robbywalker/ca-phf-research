#pragma once

#include "../tabu/util/cache.h"
#include "../tabu/arrays/tuple.h"

#include "../tabu/cphf/iterable_utuple.h"

#include "../coverage/fhCache.h"

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <utility>
#include <set>

namespace coverage {
	typedef std::set<int> intSet;

	typedef std::pair< arrays::tuple, cphf::iterable_utuple > setId;
	typedef util::cache< setId, intSet * > setCacheBase;

	class setCache : public setCacheBase {
		char filename[255];
		const char * dir;
	public: 
		setCache( ) : setCacheBase(100000) {			
		}

		void setDir( const char * dir ) {
			this->dir = dir;
		}

		virtual intSet * populate( const setId & id ) {
			// create the filename
			getFilename( id );

			FILE * f = fopen( filename, "rb" );
			int count, i, temp;
			fread( &count, sizeof(int), 1, f );

			intSet::iterator it;			
			intSet * set = new intSet;

			for ( i = 0; i < count; i++ ) {
				fread( &temp, sizeof(int), 1, f );
				if ( i == 0 ) {					
					it = set->insert( temp ).first;
				} else {
					it = set->insert( it, temp );
				}
			}			

			fclose( f );
			return set;
		}

		virtual void persist( const setId & id, intSet * value ) {
			// create the filename
			getFilename( id );

			FILE * f = fopen( filename, "wb" );
			int count, temp;

			count = (int) value->size();
			fwrite( &count, sizeof(int), 1, f );

			intSet::iterator it;			

			for ( it = value->begin(); it != value->end(); ++it ) {
				temp = *it;
				fwrite( &temp, sizeof(int), 1, f );
			}			

			fclose( f );
		}

		virtual void remove( const setId & id, intSet * value ) {
			delete value;
		}
	protected:
		inline void getFilename( const setId & id ) {
			int i;
			char tmp[20];

			strcpy( filename, dir );

			int strength = id.first.strength();

			for ( i = 0; i < strength; i++ ) {
				itoa( id.first[i], tmp, 10);
				strcat( filename, tmp );
				strcat( filename, "\\" );
			}

			for ( i = 0; i < strength; i++ ) {
				itoa( id.second[i], tmp, 10 );
				strcat( filename, tmp );
				if ( i != strength - 1 ) {
					strcat( filename, "-" );
				}
			}

			// create the file if it does not exist
			if ( _access( filename, 00 ) ) {				
				char buffer[255];
				strcpy( buffer, dir );
				for ( i = 0; i < strength; i++ ) {
					itoa( id.first[i], tmp, 10);
					strcat( buffer, tmp );

					_mkdir( buffer );
					strcat( buffer, "\\" );
				}

				FILE * f = fopen( filename, "wb" );
				int count = 0;
				fwrite( &count, sizeof(int), 1, f );
				fclose( f );
			}
		}
	};
};