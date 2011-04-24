#pragma once

#include <vector>
#include "cphfsymbol.h"

namespace cphf {

	class utuple {
		std::vector<int> values;
	public:
		utuple() {
		}

		utuple(int size) : values(size) {
		}

		utuple ( const std::vector<cphf_Symbol> & cVals ) {
			values = std::vector<int>( cVals.size() );
			
			for ( int i = 0; i < (int) cVals.size(); i++ ) {
				values[i] = cVals[i].asInt();
			}
		}

		utuple ( std::vector<int> & values ) {
			this->values = values;
		}

		inline int get(int i) const {
			return values[i];
		}

		int& operator[] (int i) {
			return values[i];
		}

		const int& operator[] (int i) const {
			return values[i];
		}

		static int compare( const utuple & a, const utuple & b ) {
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

		friend bool operator<( const utuple & a, const utuple & b ) {
			return (compare(a,b) < 0);
		}

		friend bool operator>( const utuple & a, const utuple & b ) {
			return (compare(a,b) > 0);
		}

		friend bool operator==( const utuple & a, const utuple & b ) {
			return (compare(a,b) == 0);
		}

		friend bool operator!=( const utuple & a, const utuple & b ) {
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