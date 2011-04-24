#pragma once

#include <vector>
#include "cphfsymbol.h"

namespace cphf {

	class iterable_utuple {
		int base;
		std::vector<int> values;
	public:
		iterable_utuple(int _base, int size) : values(size, 0), base(_base) {
		}

		inline int get(int i) const {
			return values[i];
		}

		inline int& operator[] (int i) {
			return values[i];
		}

		inline const int& operator[] (int i) const {
			return values[i];
		}

		static int compare( const iterable_utuple & a, const iterable_utuple & b ) {
			if ( a.values.size() != b.values.size() ) {
				return (int) (a.values.size() - b.values.size());
			} else {
				for ( int i = 0; i < (int) a.values.size(); i++ ) {
					if ( a.values[i] != b.values[i] ) {
						return a.values[i] - b.values[i];
					}
				}
				return 0;
			}
		}

		bool operator++(int) {
			int i = (int) values.size() - 1;
			while ( i >= 0 ) {
				values[i]++;

				if ( values[i] >= base ) {
					values[i] = 0;
					i--;
				} else {
					break;
				}
			}	

			return ( i >= 0 );
		}

		friend bool operator<( const iterable_utuple & a, const iterable_utuple & b ) {
			return (compare(a,b) < 0);
		}

		friend bool operator>( const iterable_utuple & a, const iterable_utuple & b ) {
			return (compare(a,b) > 0);
		}

		friend bool operator==( const iterable_utuple & a, const iterable_utuple & b ) {
			return (compare(a,b) == 0);
		}

		friend bool operator!=( const iterable_utuple & a, const iterable_utuple & b ) {
			return (compare(a,b) != 0);
		}

		void print() const {
			printf("[");
			for ( int i = 0; i < (int) values.size(); i++ ) {
				printf("%d", values[i]);
				if ( i != values.size()-1 ) {
					printf(", ");
				}
			}
			printf("]");
		}
	};

};