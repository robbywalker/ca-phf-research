#pragma once

#include <vector>

namespace arrays {

	struct tupleParams {
		int t;
		int k;

		tupleParams( int strength, int cols ) : t(strength), k(cols) {
		}
	};


	class tuple {
		int t, k;
		std::vector<int> indices;
	public:
		typedef tuple lesserTuple;
		typedef tupleParams initParams;

		tuple( const initParams & p ) : t( p.t ), k( p.k ), indices( p.t ) {
			reset();
		}

		tuple( int strength, int cols ) : t(strength), k(cols), indices(strength) {
			reset();
		}

		tuple ( int cols, std::vector<int> & _indices ) : indices(_indices), t((int) _indices.size()), k(cols) {
		}

		lesserTuple createLesserTuple() {
			return lesserTuple( t - 1, k - 1 );
		}

		inline int strength() const {
			return t;
		}

		inline int get(int i) const {
			return indices[i];
		}

		inline void reset() {
			for ( int i = 0; i < t; i++ ) {
				indices[i] = i;
			}
		}

		int indexOf(int idx) const {
			for ( int i = 0; i < t; i++ ) {
				if ( indices[i] == idx )
					return i;
			}
			return -1;
		}

		inline int& operator[] (int i) {
			return indices[i];
		}

		inline const int& operator[] (int i) const {
			return indices[i];
		}

		bool operator++ (void) {
			if ( t == 0 ) return false;

			int i, j, a;
			for ( i = 0; i < t; i++ ) {
				j = t - i - 1;

				indices[j]++;
				if ( indices[j] >= (k - i) ) {
					if ( j == 0 ) {
						return false;
					}
				} else {
					for ( a = j+1; a < t; a++ ) {
						indices[a] = indices[j] + (a - j);
					}
					break;
				}
			}   
			return true;
		}

		static int compare( const tuple & a, const tuple & b ) {
			if ( a.t != b.t ) {
				return a.t - b.t;
			} else {
				for ( int i = 0; i < a.t; i++ ) {
					if ( a.indices[i] != b.indices[i] ) {
						return a.indices[i] - b.indices[i];
					}
				}
				return 0;
			}
		}

		friend bool operator<( const tuple & a, const tuple & b ) {
			return (compare(a,b) < 0);
		}

		friend bool operator>( const tuple & a, const tuple & b ) {
			return (compare(a,b) > 0);
		}

		friend bool operator==( const tuple & a, const tuple & b ) {
			return (compare(a,b) == 0);
		}

		friend bool operator!=( const tuple & a, const tuple & b ) {
			return (compare(a,b) != 0);
		}

		// "Upgrades" this tuple
		//   0 3 4 around col=2 becomes
		//   0 2 4 5
		void initViaUpgrade( const lesserTuple & input, int col ) {
			int i, j;
			for ( i = 0, j = 0; i < input.t; i++, j++ ) {
				if ( i == j ) {
					if ( col <= input.get(i) ) {
						indices[j] = col;
						j++;
						indices[j] = input.get(i) + 1;
					} else {
						indices[j] = input.get(i);
					}
				} else {
					indices[j] = input.get(i) + 1;
				}
			}
			if ( i == j ) {
				indices[j] = col;
			}
		}

		void print() const {
			printf("<");
			for ( int i = 0; i < t; i++ ) {
				printf("%d", indices[i]);
				if ( i != t-1 ) {
					printf(", ");
				}
			}
			printf(">");
		}
	};

};