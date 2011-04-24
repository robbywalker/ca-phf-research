#pragma once

#include <vector>

namespace sums {

	template <class Symbol>
	class sums_Check {
		int target;
	public:
		

		sums_Check(int _target) : target(_target){
		}

		inline bool good( const std::vector<Symbol> slice ) {
			int size = (int) slice.size();
			int sum = 0;
			for ( int i = 0; i < size; i++ ) {
				sum += slice[i].asInt();
			}
			return (sum == sums_Check<Symbol>::target);
		}

		inline bool good( const Symbol * slice, int size ) {
			int sum = 0;
			for ( int i = 0; i < size; i++ ) {
				sum += slice[i].asInt();
			}
			return (sum == sums_Check<Symbol>::target);
		}
		
	};


};