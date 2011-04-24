#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <set>
#include <string>

namespace words {
	template <class Symbol>
	class word_Check {
		int strength;
		std::set<std::string> okWords;
	public:
		word_Check(const char * filename, int _strength) : strength(_strength) {
			char * buf = new char[strength+1];

			FILE * f = fopen( filename, "rt" );

			while ( !feof(f) ) {
				if ( fscanf(f, " %s ", buf) ) {
					std::string s(buf);
					okWords.insert( std::string( buf ) );

				} else 
					break; // reached eof
			}

			delete[] buf;

		}

		bool good( const Symbol * slice, int size ) {
			char * buf = new char[strength+1];
			for ( int i = 0; i < strength; i++ ) {
				buf[i] = slice[i].asChar();
			}
			buf[strength] = 0;
			std::string s( buf );

			delete[] buf;

			return ( okWords.count( s ) != 0 );
		}
		
	};

};