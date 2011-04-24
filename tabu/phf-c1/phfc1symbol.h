#pragma once

#include "../arrays/symbol.h"
#include "../util/util.h"

#include <set>
#include <algorithm>

namespace phf_c1 {
	class phf_c1_SymbolGenerator;

	template <class SYM>
	class phf_c1_CheckT1;

	template <class SYM>
	class phf_c1_CheckT2;

	template <class SYM>
	class phf_c1_CheckT3;

	class phf_c1_Symbol {		
		int syms;
		int size;

		bool type;
		std::set<int> value;

		void parse( int val ) {
			type = ( val & 1 ) != 0;
			val = val >> 1;

			for ( int i = 0; i < size; i++ ) {
				int temp = val % syms;
				value.insert( temp );
				val /= syms;
			}
		}

	public:
		phf_c1_Symbol() {
		}

		phf_c1_Symbol(int _syms, int _size) {
			syms = _syms;
			size = _size;

			type = true;
			value.insert( 0 );
		}

		phf_c1_Symbol(int _syms, int _size, int val) {			
			syms = _syms;
			size = _size;

			parse(val);
		}

		phf_c1_Symbol(FILE * f, int _syms, int strength) {
			int temp;
			fscanf(f, "%d-%d", &size, &temp);			
			syms = _syms;
			
			parse( temp );
		}

		int asInt() const {
			int temp = 0;

			std::set<int>::const_iterator it = value.begin();
			std::set<int>::const_iterator temp_it;

			for ( int i = 0; i < size; i++ )
			{
				temp *= syms;
				temp += *it;

				temp_it = it;
				it++;
				if ( it == value.end() ) {
					it = temp_it;
				}
			}

			temp <<= 1;
			temp += type ? 1 : 0;

			return temp;
		}

		bool getType() const {
			return type;
		}

		bool isBad() const {
			return !type && ( (int) value.size() < size);
		}

		bool intersects( const phf_c1_Symbol & b ) const {
			if ( isBad() || b.isBad() ) {
				return true;
			}

			std::set<int>::const_iterator it;

			if ( type == false ) {
				if ( b.type == false ) {
					printf("Should not reach here (1)!\n");
					exit(1);
					return true;
				} else {
					// this without, b with
					for ( it = b.value.begin(); it != b.value.end(); it++ ) {
						if ( value.count( *it ) == 0 ) {
							return false;
						}
					}
					return true;
				}
			} else {
				if ( b.type ) {
					// both with
					printf("Should not reach here (1)!\n");
					exit(1);
					return true;
				} else {
					// this with, b without
					for ( it = value.begin(); it != value.end(); it++ ) {
						if ( b.value.count( *it ) == 0 ) {
							return false;
						}
					}
					return true;
				}
			}
		}

		bool ok3( const phf_c1_Symbol & b, const phf_c1_Symbol & c ) const {
			if ( ! (type && b.type && c.type) )
				return false;

			//print(); printf(" "); b.print(); printf(" "); c.print(); printf("\n");

			std::set<int>::const_iterator it = value.begin();
			for ( ; it != value.end(); it++ ) {
				if ( b.value.count( *it ) || c.value.count( *it ) ) {
					return false;
				}
			}

			it = b.value.begin();
			for ( ; it != b.value.end(); it++ ) {
				if ( c.value.count( *it ) ) {
					return false;
				}
			}

			return true;
		}

		void print() const {
			if ( type ) {
				printf("I(");
			} else {
				printf("O(");
			}

			std::set<int>::const_iterator it = value.begin();
			for ( ; it != value.end(); it++ ) {
				if ( it != value.begin() ) {
					printf(",");
				}
				printf("%02d",*it);
			}

			printf(")");

			for ( int i = (int) value.size(); i < size; i++ ) {
				printf("   ");
			}
		}

		void fprint(FILE * out) const {
			fprintf(out, "%d-%d", size, asInt());
		}

		friend bool operator==(const phf_c1_Symbol & x, const phf_c1_Symbol & y) {
			return x.type == y.type && x.value == y.value;
		}

		friend phf_c1_SymbolGenerator;
		friend phf_c1_CheckT1<phf_c1_Symbol>;
		friend phf_c1_CheckT2<phf_c1_Symbol>;
		friend phf_c1_CheckT3<phf_c1_Symbol>;
	};

	class phf_c1_SymbolGenerator : public arrays::SymbolGenerator<phf_c1_Symbol> {
		int syms;
		int size;
		int count;
	public:
		phf_c1_SymbolGenerator(int _syms, int _size) {
			syms = _syms;
			size = _size;
			count = 2 * util::ipow(_syms,_size);
		}

		virtual int getCount() const {
			return count;
		}

		virtual phf_c1_Symbol generate(int val) const {
			return phf_c1_Symbol(syms, size, val);
		}

		virtual phf_c1_Symbol defaultValue() const {
			return phf_c1_Symbol(syms, size, 0);
		}

		virtual phf_c1_Symbol randomValue() const {
			return phf_c1_Symbol( syms, size, util::random( count ) );
		}
	};
};

