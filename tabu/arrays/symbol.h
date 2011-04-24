#pragma once

namespace arrays {

	/*class Symbol {
	protected:
		Symbol() { } // abstract - cannot be created

		virtual bool equals( const Symbol & y ) const = 0;
	public:
		friend bool operator==(const Symbol & x, const Symbol & y) {
			return x.equals( y );
		}		

		virtual void print() const = 0;
	};*/

	template <class Symbol>
	class SymbolGenerator {
	protected:
		SymbolGenerator() { } // abstract - cannot be created
	public:
		virtual int getCount() const = 0;

		virtual Symbol generate(int val) const = 0;

		virtual Symbol defaultValue() const = 0;

		virtual Symbol randomValue() const = 0;
	};

};