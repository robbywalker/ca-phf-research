#pragma once

#include "../arrays/symbol.h"
#include "../util/util.h"

namespace cphf {
	class cphf_SymbolGenerator;
	template <class SYM>
	class cphf_Check;

	class cphf_Symbol {
		int value;
		int size;
		int base;

	public:
		cphf_Symbol() {
		}

		cphf_Symbol(int _size, int _base) {
			value = 0;
			size = _size;
			base = _base;
		}

		cphf_Symbol(int _size, int _base, int val) {
			value = val;
			size = _size;
			base = _base;
		}

		cphf_Symbol(int _size, int _base, int * arr) {
			size = _size;
			base = _base;

			value = 0;
			for ( int i = size-1; i >= 0; i-- ) {
                value *= _base;
				value += arr[i];
			}
		}

		cphf_Symbol(FILE * f, int syms, int strength) {
			base = syms;
			size = strength - 1;

			int temp;
            fscanf(f, " %X ", &temp);

			int * buf = new int[size];
			int i;
			for ( i = 0; i < size; i++ ) {
				buf[i] = temp % 16;				
				temp = temp / 16;
			}

			value = 0;
			int factor = 1;
			for ( i = 0; i < size; i++ ) {
				value += buf[i] * factor;
				factor *= base;
			}

			delete[] buf;
		}

		int asInt() const {
			return value;
		}

		void print() const {
			int * buf = new int[size];
			int cur = value;
			int i;

			for ( i = 0; i < size; i++ ) {
				buf[i] = cur % base;				
				cur = cur / base;
			}
			for ( i = size-1; i >= 0; i-- ) {
				printf("%X", buf[i]);
			}

			delete[] buf;
		}

		void intoArray( int * arr ) const {
			int cur = value;
			for ( int i = 0; i < size; i++ ) {
				arr[i] = cur % base;				
				cur = cur / base;
			}
		}

		void fprint(FILE * out) const {
			int * buf = new int[size];
			int cur = value;
			int i;

			for ( i = 0; i < size; i++ ) {
				buf[i] = cur % base;				
				cur = cur / base;
			}
			for ( i = size-1; i >= 0; i-- ) {
				fprintf(out, "%X", buf[i]);
			}

			delete[] buf;
		}

		friend bool operator==(const cphf_Symbol & x, const cphf_Symbol & y) {
			return x.value == y.value;
		}

		friend cphf_SymbolGenerator;
		friend cphf_Check<cphf_Symbol>;
	};

	class cphf_SymbolGenerator : public arrays::SymbolGenerator<cphf_Symbol> {
		int base;
		int size;
		int count;
	public:
		cphf_SymbolGenerator(int _size, int _base) {
			size = _size;
			base = _base;

			count = util::ipow(base, size);
		}

		virtual int getCount() const {
			return count;
		}

		virtual cphf_Symbol generate(int val) const {
			return cphf_Symbol(size, base, val);
		}

		virtual cphf_Symbol defaultValue() const {
			return cphf_Symbol(size, base);
		}

		virtual cphf_Symbol randomValue() const {
			return cphf_Symbol( size, base, util::random( count ) );
		}
	};
};

