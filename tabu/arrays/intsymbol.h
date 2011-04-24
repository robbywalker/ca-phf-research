#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "../util/util.h"

namespace phf {
	template <class Symbol> class phf_Check;
};

namespace arrays {
	class int_SymbolGenerator;

	class int_Symbol {
		int value;

	public:
		inline int_Symbol() : value(0) {
		}

		inline int_Symbol(int val) : value(val) {
		}

		int_Symbol(FILE * f, int syms, int strength) {
            fscanf(f, " %d ", &value);
		}

		inline int asInt() const {
			return value;
		}

		inline void print() const {
			printf("%d", value);
		}

		inline void fprint(FILE * out) const {
			fprintf(out, "%d", value);
		}

		inline friend bool operator==(const int_Symbol & x, const int_Symbol & y) {
			return x.value == y.value;
		}

		inline friend bool operator!=(const int_Symbol & x, const int_Symbol & y) {
			return x.value != y.value;
		}

		friend int_SymbolGenerator;
		friend phf::phf_Check<int_Symbol>;
	};

	class int_SymbolGenerator : public SymbolGenerator<int_Symbol> {
		int count;
	public:
		int_SymbolGenerator(int count) {
			this->count = count;
		}

		virtual int getCount() const {
			return count;
		}

		virtual int_Symbol generate(int val) const {
			return int_Symbol(val);
		}

		virtual int_Symbol defaultValue() const {
			return int_Symbol();
		}

		virtual int_Symbol randomValue() const {
			return int_Symbol( util::random( count ) );
		}
	};
};