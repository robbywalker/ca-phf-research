#include "galois_field.h"

namespace math {

	int table4_add[4][4] = 
							{ { 0, 1, 2, 3 },
						  	  { 1, 0, 3, 2 },
							  { 2, 3, 0, 1 },
							  { 3, 2, 1, 0 } };
							 
	int table4_mul[4][4] = 
							{ { 0, 0, 0, 0 },
						  	  { 0, 1, 2, 3 },
							  { 0, 2, 3, 1 },
							  { 0, 3, 1, 2 } };

	int table8_add[8][8] =
						{{	0,     1,     2,     3,     4,     5,     6,     7 },
						 {	1,     0,     3,     2,     5,     4,     7,     6 },
						 {	2,     3,     0,     1,     6,     7,     4,     5 },
						 {	3,     2,     1,     0,     7,     6,     5,     4 },
						 {	4,     5,     6,     7,     0,     1,     2,     3 },
						 {	5,     4,     7,     6,     1,     0,     3,     2 },
						 {	6,     7,     4,     5,     2,     3,     0,     1 },
						 {	7,     6,     5,     4,     3,     2,     1,     0 }};

	int table8_mul[8][8] =
						{{0,     0,     0,     0,     0,     0,     0,     0},
 						 {0,     1,     2,     3,     4,     5,     6,     7},
						 {0,     2,     4,     6,     3,     1,     7,     5},
						 {0,     3,     6,     5,     7,     4,     1,     2},
						 {0,     4,     3,     7,     6,     2,     5,     1},
						 {0,     5,     1,     4,     2,     7,     3,     6},
						 {0,     6,     7,     1,     5,     3,     2,     4},
						 {0,     7,     5,     2,     1,     6,     4,     3}};

	int table9_add[9][9] =
   {{0, 1, 2, 3, 4, 5, 6, 7, 8},
	{1, 2, 0, 4, 5, 3, 7, 8, 6},
	{2, 0, 1, 5, 3, 4, 8, 6, 7},
	{3, 4, 5, 6, 7, 8, 0, 1, 2},
	{4, 5, 3, 7, 8, 6, 1, 2, 0},
	{5, 3, 4, 8, 6, 7, 2, 0, 1},
	{6, 7, 8, 0, 1, 2, 3, 4, 5},
	{7, 8, 6, 1, 2, 0, 4, 5, 3},
	{8, 6, 7, 2, 0, 1, 5, 3, 4}};

	int table9_mul[9][9] =
   {{0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 2, 3, 4, 5, 6, 7, 8},
	{0, 2, 1, 6, 8, 7, 3, 5, 4},
	{0, 3, 6, 2, 5, 8, 1, 4, 7},
	{0, 4, 8, 5, 6, 1, 7, 2, 3},
	{0, 5, 7, 8, 1, 3, 4, 6, 2},
	{0, 6, 3, 1, 7, 4, 2, 8, 5},
	{0, 7, 5, 4, 2, 6, 8, 3, 1},
	{0, 8, 4, 7, 3, 2, 5, 1, 6}};



	GFTables GFNum::tables;

	gftable * GFTables::get(int x) {
		if ( tables.find(x) == tables.end() ) {
			gftable * t = new gftable;
			if ( x == 4 ) {				
				int ** add = new int*[x];
				int ** mul = new int*[x];
				for ( int i = 0; i < x; i++ ) {
					add[i] = new int[x];
					mul[i] = new int[x];
					for ( int j = 0; j < x; j++ ) {
						add[i][j] = table4_add[i][j];
						mul[i][j] = table4_mul[i][j];
					}
				}		

				t->add = add;
				t->mul = mul;

			} else if ( x == 8 ) {
				int ** add = new int*[x];
				int ** mul = new int*[x];
				for ( int i = 0; i < x; i++ ) {
					add[i] = new int[x];
					mul[i] = new int[x];
					for ( int j = 0; j < x; j++ ) {
						add[i][j] = table8_add[i][j];
						mul[i][j] = table8_mul[i][j];
					}
				}		

				t->add = add;
				t->mul = mul;

			} else if ( x == 9 ) {
				int ** add = new int*[x];
				int ** mul = new int*[x];
				for ( int i = 0; i < x; i++ ) {
					add[i] = new int[x];
					mul[i] = new int[x];
					for ( int j = 0; j < x; j++ ) {
						add[i][j] = table9_add[i][j];
						mul[i][j] = table9_mul[i][j];
					}
				}		

				t->add = add;
				t->mul = mul;

			} else {
				// WE ASSUME X IS PRIME
				int ** add = new int*[x];
				int ** mul = new int*[x];
				for ( int i = 0; i < x; i++ ) {
					add[i] = new int[x];
					mul[i] = new int[x];
					for ( int j = 0; j < x; j++ ) {
						add[i][j] = (i + j) % x;
						mul[i][j] = (i * j) % x;
					}
				}		

				t->add = add;
				t->mul = mul;
			}

			t->add_inv = new int[x];
			for ( int i = 0; i < x; i++ ) {
				for ( int j = 0; j < x; j++ ) {
					if ( t->add[i][j] == 0 ) {
						t->add_inv[i] = j;
						break;
					}
				}
			}

			tables[x] = t;
		}

		return tables[x];
	}

	GFNum::GFNum( int _size ) {
		value = 0;
		table = tables.get(_size);
	}

	GFNum::GFNum( int _value, int _size ) {
		value = _value;
		table = tables.get(_size);
	}

	GFNum::GFNum( const GFNum & _copy ) {
		value = _copy.value;
		table = _copy.table;
	}

	GFNum::GFNum( int _value, gftable * _table ) {
		value = _value;
		table = _table;
	}

	GFNum GFNum::operator+ ( const GFNum & a ) {
		return GFNum( table->add[value][a.value], table );
	}

	GFNum GFNum::operator+= ( const GFNum & a ) {
		value = table->add[value][a.value];
		return *this;
	}

	GFNum GFNum::operator* ( const GFNum & a ) {
		return GFNum( table->mul[value][a.value], table );
	}

	GFNum GFNum::operator- () const {
		return GFNum( table->add_inv[value], table );
	}

	GFNum GFNum::operator++(int) {
		value++;
		return *this;
	}

	int operator< ( const GFNum & a, int b ) {
		return a.value < b;
	}

	void GFNum::print() {
		printf("%d", value);
	}
};