#pragma once

#include "../tabu/util/cache.h"
#include "../tabu/arrays/tuple.h"

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <io.h>

namespace coverage {
	typedef util::cache< arrays::tuple, FILE * > fhCacheBase;

	class fhCache : public fhCacheBase {
		const char * dir;
		int bytesPerUtuple;
		int utupleCount;
		int cols;
		char * blankUtuple;
	public: 
		fhCache() : fhCacheBase(50) {
			this->dir = NULL;
			autoDirty = true;
		}

		void setDir( const char * dir ) {
			this->dir = dir;
		}

		void setFileLength( int bytesPerUtuple, int utupleCount, int columns ) {
			this->bytesPerUtuple = bytesPerUtuple;
			this->utupleCount = utupleCount;
			this->cols = columns;

			blankUtuple = (char *) calloc( bytesPerUtuple, 1 );
		}

		virtual FILE * populate( const arrays::tuple & id ) {
			char buffer[255];
			strcpy( buffer, dir );

			char tmp[20];
			for ( int i = 0; i < id.strength() - 1; i++ ) {
				itoa( id[i], tmp, 10);
				strcat( buffer, tmp );

				if ( i != id.strength() - 2 ) {					
					strcat( buffer, "\\" );
				}				
			}

			if ( _access( buffer, 00 ) ) {
				// file does not exist
				strcpy( buffer, dir );
				for ( int i = 0; i < id.strength() - 1; i++ ) {
					itoa( id[i], tmp, 10);
					strcat( buffer, tmp );

					if ( i != id.strength() - 2 ) {	
						_mkdir( buffer );
						strcat( buffer, "\\" );
					}				
				}

				FILE * f = fopen( buffer, "w+bS" );
				int count = utupleCount * (cols - id[ id.strength() - 2 ] - 1);
				for ( int i = 0; i < count; i++ ) {
					fwrite( blankUtuple, 1, bytesPerUtuple, f );
				}

				return f;
			}

			return fopen( buffer, "r+bR" );
		}

		virtual void persist( const arrays::tuple & id, FILE * value ) {
			fclose( value );
		}
	private:
		void flush() { // flush makes no sense in this context
		}
	};
};