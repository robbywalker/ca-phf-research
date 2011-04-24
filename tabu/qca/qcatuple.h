#pragma once

#include "../arrays/tuple.h"

namespace qca {

	struct qcaTupleParams {
		int k;
		int l;
	};

	class qcaLesserTuple {
		int copies, columns;
		int copy, column;

	public:
		qcaLesserTuple(int _copies, int _columns) : 
		  copy(0), column(0), copies(_copies), columns(_columns) 
		{
		}

		bool operator++ (void) {
			if ( ++column < columns )
				return true;

			column = 0;
			return (++copy < copies);
		}

		inline int getCopy() const {
			return copy;
		}

		inline int getColumn() const {
			return column;
		}
	};

	class qcatuple {
		int k, l;
		arrays::tuple t_copies;
		arrays::tuple t_cols;

		inline int idx( int copy, int col ) const {
			return k * copy + col;
		}


	public:
		typedef qcaLesserTuple lesserTuple;
		typedef qcaTupleParams initParams;

 		qcatuple( int cols, int copies ) : 
		  k(cols), l(copies),
		  t_copies( 2, copies ),
		  t_cols( 2, cols )
        {
		}

		qcatuple( const initParams & params ) : 
		  k(params.k), l(params.l),
		  t_copies( 2, params.l ),
		  t_cols( 2, params.k )
		{
		}

		qcatuple( const qcatuple & copy ) : k( copy.k ), l( copy.l), t_copies( copy.t_copies ), t_cols( copy.t_cols ) {
		}

		lesserTuple createLesserTuple() {
			return lesserTuple( l - 1, k - 1 );
		}

		inline int strength() const {
			return 4;
		}

		inline int get(int i) const {
			switch (i) {
				case 0: return idx( t_copies.get(0), t_cols.get(0) );
				case 1: return idx( t_copies.get(0), t_cols.get(1) );
				case 2: return idx( t_copies.get(1), t_cols.get(1) );
				case 3: return idx( t_copies.get(1), t_cols.get(0) );
			}

			return -1;
		}

		int indexOf(int idx) const {
			if ( get(0) == idx ) return 0;
			if ( get(1) == idx ) return 1;
			if ( get(2) == idx ) return 2;
			if ( get(3) == idx ) return 3;

			return -1;
		}

		inline int operator[] (int i) const {
			return get(i);
		}

		bool operator++ (void) {
			if ( ++t_cols ) 
				return true;
			
			t_cols.reset();
			return ++t_copies;
		}

		static int compare( const qcatuple & a, const qcatuple & b ) {
			int x = arrays::tuple::compare( a.t_cols, b.t_cols );
			if ( x == 0 ) {
				return arrays::tuple::compare( a.t_copies, b.t_copies );
			}
			return x;
		}

		friend bool operator<( const qcatuple & a, const qcatuple & b ) {
			return (compare(a,b) < 0);
		}

		friend bool operator>( const qcatuple & a, const qcatuple & b ) {
			return (compare(a,b) > 0);
		}

		friend bool operator==( const qcatuple & a, const qcatuple & b ) {
			return (compare(a,b) == 0);
		}

		friend bool operator!=( const qcatuple & a, const qcatuple & b ) {
			return (compare(a,b) != 0);
		}

		void print() const {
			printf("<%d, %d, %d, %d>", get(0), get(1), get(2), get(3));
		}

		const arrays::tuple & getCopiesTuple() const {
			return t_copies;
		}

		const arrays::tuple & getColumnsTuple() const {
			return t_cols;
		}

		// "Upgrades" this tuple
		void initViaUpgrade( const lesserTuple & input, int col ) {
			int column = col % k;
			int copy = col / k;

			if ( input.getCopy() < copy ) {
				t_copies[0] = input.getCopy();
				t_copies[1] = copy;
			} else {
				t_copies[0] = copy;
				t_copies[1] = input.getCopy() + 1;
			}

			if ( input.getColumn() < column ) {
				t_cols[0] = input.getColumn();
				t_cols[1] = column;
			} else {
				t_cols[0] = column;
				t_cols[1] = input.getColumn() + 1;
			}
		}
	};



};