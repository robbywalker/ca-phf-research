#pragma once

#include <vector>
#include "../arrays/intsymbol.h"

namespace phf {

	template <class Symbol>
	class phf_Check {
	public:
		phf_Check() {
		}

		inline bool good( const Symbol * slice, int size ) {
			for ( int i = 0; i < (size - 1); i++ ) {
				for ( int j = i+1; j < size; j++ ) {
					if ( slice[i] == slice[j] ) {
						return false;
					}
				}
			}
			return true;
		}

		inline bool good( const std::vector<Symbol> & slice ) {
			int size = (int) slice.size();
			for ( int i = 0; i < (size - 1); i++ ) {
				for ( int j = i+1; j < size; j++ ) {
					if ( slice[i] == slice[j] ) {
						return false;
					}
				}
			}
			return true;
		}
		
	};

	template <>
	class phf_Check<arrays::int_Symbol> {
	public:
		phf_Check() {
		}

		inline bool good( const arrays::int_Symbol * slice, int size ) {
			int val;
			for ( int i = 0; i < (size - 1); i++ ) {
				val = slice[i].value;
				for ( int j = i+1; j < size; j++ ) {
					if ( val == slice[j].value ) {
						return false;
					}
				}
			}
			return true;			            
		}

		inline bool good( const std::vector<arrays::int_Symbol> & slice ) {
			int size = (int) slice.size();
			int val;
			for ( int i = 0; i < (size - 1); i++ ) {
				val = slice[i].value;
				for ( int j = i+1; j < size; j++ ) {
					if ( val == slice[j].value ) {
						return false;
					}
				}
			}
			return true;			            
		}
	};
};