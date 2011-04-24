#pragma once

namespace tabu {
	template <class Symbol, class Score = intScore >
	class tabuStatus {

	public:
		tabuStatus() {
		}

		virtual void reInit( arrays::Array<Symbol> & _arr, int _strength ) = 0;

		virtual Score getScore() = 0;

		virtual void getResultInto(arrays::Array<Symbol> & s) = 0;

		virtual Symbol getArrayValue( int i, int j ) const = 0;

		virtual arrays::SymbolGenerator<Symbol> * getSymbolGenerator() = 0;

		virtual const arrays::Array<Symbol> & getArr() = 0;

		virtual const arrays::Array<Symbol> * getArrayPtr() = 0;

		virtual void print() const = 0;

		virtual int countBadPerColumn( int * counts ) = 0;

		virtual int calculateEffectOfChange( int row, int col, Symbol newValue ) = 0;

		virtual int change( int row, int col, Symbol newValue ) = 0;

		virtual int getStrength() = 0;

		virtual int getSymbolCount() = 0;
	};
};