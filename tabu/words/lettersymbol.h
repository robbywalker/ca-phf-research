#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "../arrays/symbol.h"
#include "../util/util.h"

namespace words {
	class letter_SymbolGenerator;

	class letter_Symbol {
		char value;

	public:
		letter_Symbol() {
			value = 'a';
		}

		letter_Symbol(int val) {
			value = 'a' + val;
		}

		letter_Symbol(FILE * f, int syms, int strength) {
            fscanf(f, " %c ", &value);
			if ( 'A' <= value && value <= 'Z' ) {
				value = value - 'A' + 'a';
			}
		}

		int asInt() const {
			return value - 'a';
		}

		char asChar() const {
			return value;
		}

		void print() const {
			printf("%c", value);
		}

		void fprint(FILE * out) const {
			fprintf(out, "%c", value);
		}

		friend bool operator==(const letter_Symbol & x, const letter_Symbol & y) {
			return x.value == y.value;
		}

		friend letter_SymbolGenerator;
	};

	class letter_SymbolGenerator : public arrays::SymbolGenerator<letter_Symbol> {
	public:
		letter_SymbolGenerator() {
		}

		virtual int getCount() const {
			return 26;
		}

		virtual letter_Symbol generate(int val) const {
			return letter_Symbol(val);
		}

		virtual letter_Symbol defaultValue() const {
			return letter_Symbol();
		}

		virtual letter_Symbol randomValue() const {
			return letter_Symbol( util::random( 26 ) );
		}
	};
};