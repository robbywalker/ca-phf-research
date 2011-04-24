#pragma once

#include <vector>

namespace phf_c1 {

	template <class Symbol>
	class phf_c1_CheckT1 {
	public:
		phf_c1_CheckT1() {
		}

		inline bool good( const std::vector<Symbol> slice ) {
			return ! slice[0].getType();
		}		
	};

	template <class Symbol>
	class phf_c1_CheckT2_1 {
	public:
		phf_c1_CheckT2_1() {
		}

		inline bool good( const std::vector<Symbol> slice ) {
			return (!slice[0].getType()) && slice[1].getType() && (! slice[0].intersects( slice[1] ));
		}		
	};

	template <class Symbol>
	class phf_c1_CheckT2_2 {
	public:
		phf_c1_CheckT2_2() {
		}

		inline bool good( const std::vector<Symbol> slice ) {
			return (!slice[1].getType()) && slice[0].getType() && (! slice[0].intersects( slice[1] ));
		}		
	};

	template <class Symbol>
	class phf_c1_CheckT3 {
	public:
		phf_c1_CheckT3() {
		}

		inline bool good( const std::vector<Symbol> slice ) {
			return slice[0].ok3( slice[1], slice[2] );
		}		
	};

};