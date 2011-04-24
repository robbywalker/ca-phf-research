#pragma once

#include "../tabu/arrays/tuple.h"
#include "../tabu/cphf/iterable_utuple.h"
#include "../tabu/util/util.h"

#include "../coverage/fhCache.h"
#include "../coverage/byteCache.h"

#include <stdio.h>
#include <direct.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>

namespace coverage {
	int bitCounts[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3,
					    4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4,
						4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 
						3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 
						4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 
						4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 
						3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 
						6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 
						4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 
						6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 };

	int bitIndex[] =  { -1, 0, 1, -1, 2, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					    -1, -1, -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7, -1, -1, -1, -1, -1, -1, 
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
						-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

	class fileStore {
		int rows, columns, symbols, strength;

		int bytesPerUtuple;
		int blockSize;

		char * dir;

		bool empty;

		fhCache fhC;
		byteCache byteC;

		inline int fidx( const arrays::tuple t,  const cphf::iterable_utuple & u ) {
			int uIdx = 0;
			for ( int i = 0; i < strength; i++ ) {
				uIdx *= symbols;
				uIdx += u[i];
			}

			return (uIdx * bytesPerUtuple) + blockSize * (t[ t.strength() - 1 ] - t[ t.strength() - 2 ] - 1);
		}

	public:
		fileStore( int rows, int columns, int symbols, int strength, bool create ) {
			this->rows = rows;
			this->columns = columns;
			this->symbols = symbols;
			this->strength = strength;

			bytesPerUtuple = rows / 8;
			if ( rows % 8 != 0 ) bytesPerUtuple++;

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

			fhC.setDir( dir );
			fhC.setFileLength( bytesPerUtuple, util::ipow( symbols, strength ), columns );

			blockSize = bytesPerUtuple * util::ipow( symbols, strength );

			byteC.setFHCache( &fhC );
		}

		bool isEmpty() {
			return empty;
		}

		void setCovered( const arrays::tuple & t, const cphf::iterable_utuple & u, int row ) {
			int fIdx = this->fidx( t, u );

			int rIdx = row / 8;
			int rq = row % 8;

			byteId id(t,fIdx + rIdx);
			byteC.set( id, byteC.get( id ) | (1 << rq) );
		}

		void unsetCovered( const arrays::tuple & t, const cphf::iterable_utuple & u, int row ) {
			int fIdx = this->fidx( t, u );

			int rIdx = row / 8;
			int rq = row % 8;

			byteId id(t,fIdx + rIdx);
			byteC.set( id, byteC.get( id ) & ~(1 << rq) );
		}

		bool isCovered( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			int fIdx = this->fidx( t, u );

			bool found = false;
			byteId id(t,fIdx);
			for ( int i = 0; i < bytesPerUtuple && !found; i++ ) {
				id.second = fIdx + i;
				if ( byteC.get( id ) != 0 ) {
					found = true;
				}				
			}			

			return found;
		}

		bool isUnique( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			int fIdx = this->fidx( t, u );

			bool found = false;
			unsigned char buffer;			
			byteId id(t,fIdx);
			for ( int i = 0; i < bytesPerUtuple; i++ ) {
				id.second = fIdx + i;
				buffer = byteC.get( id );
				if ( buffer != 0 ) {
					if ( found ) {
						// multiple
						return false;
					} else {
						if ( bitCounts[ buffer ] == 1 ) {
							found = true;
						} else {
							// multiple
							return false;
						}
					}
				}				
			}			

			return found;
		}

		int getUnique( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			int fIdx = this->fidx( t, u );

			int ret = -1;
		
			unsigned char buffer;	
			byteId id(t,fIdx);
			for ( int i = 0; i < bytesPerUtuple; i++ ) {
				id.second = fIdx + i;
				buffer = byteC.get( id );
				if ( buffer != 0 ) {
					if ( ret != -1 ) {
						return -1; // a multiple
					} else {
						ret = bitIndex[ buffer ];

						if ( ret == -1 ) // a multiple
							return -1;
						else 
							ret += i*8;
					}
				}				
			}			

			return ret;
		}

		int getCount( const arrays::tuple & t, const cphf::iterable_utuple & u ) {
			int fIdx = this->fidx( t, u );

			int ret = 0;
		
			byteId id(t,fIdx);
			for ( int i = 0; i < bytesPerUtuple; i++ ) {
				id.second = fIdx + i;
				ret += bitCounts[ (unsigned char) byteC.get( id ) ];
			}			

			return ret;
		}

		int getDuo( const arrays::tuple & t, const cphf::iterable_utuple & u, int ignore ) {
			int fIdx = this->fidx( t, u );

			int ret = -1;

			int ignoreB = ignore / 8;
			int ignoreQ = ignore % 8;
		
			unsigned char buffer;	
			byteId id(t,fIdx);
			for ( int i = 0; i < bytesPerUtuple; i++ ) {
				id.second = fIdx + i;
				buffer = byteC.get( id );

				if ( i == ignoreB )
					buffer &= ~(1 << ignoreQ);

				if ( buffer != 0 ) {
					if ( ret != -1 ) {
						return -1; // a multiple
					} else {
						ret = bitIndex[ buffer ];

						if ( ret == -1 ) // a multiple
							return -1;
						else
							ret += i * 8;
					}
				}				
			}			

			return ret;
		}

		void persist() {
			byteC.flush();
		}
	};

}