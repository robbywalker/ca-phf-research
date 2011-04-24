#pragma once

#include <map>

namespace math {
				 
	struct gftable {
		int ** add;
		int ** mul;
		int * add_inv;
	};

	class GFTables {
		std::map<int,gftable*> tables;
	public:
		GFTables() {
		}		

		gftable * get(int);
	};

	class GFNum {
		static GFTables tables;

		int value;
		gftable * table;

		GFNum( int _value, gftable * table );

	public:
		GFNum( int _size );

		GFNum( int _value, int _size );
		GFNum( const GFNum & _copy );
		
		GFNum operator+= ( const GFNum & a );

		GFNum operator+ ( const GFNum & a );
		GFNum operator* ( const GFNum & a );

		GFNum operator- () const;

		GFNum operator++(int);
		friend int operator< ( const GFNum & a, int b );

		operator int () {
			return value;
		}

		GFNum & operator= (int x) {
			value = x;
			return *this;
		}

		void print();
	};
};