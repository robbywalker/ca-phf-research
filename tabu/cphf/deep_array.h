#pragma once

#include "utuple.h"
#include "cphfsymbol.h"
#include <vector>

namespace cphf {
	union _da_elem {
		_da_elem ** arr;
		std::vector<bool> * vec;
	};

	class deep_array {
		_da_elem * root;
		int depth;
		int size;

		void _init( _da_elem * cur, int depth ) {
			if ( depth != 1 ) {
				cur->arr = new _da_elem*[size];
				for ( int i = 0; i < size; i++ ) {
					cur->arr[i] = new _da_elem;
					_init( cur->arr[i], depth-1 );
				}
			} else {
				cur->vec = new std::vector<bool>( size, false );
			}
		}
	public:
		deep_array() {
		}
		
		void init( int _size, int _depth ) {
			depth = _depth;
			size = _size;

			root = new _da_elem;
			_init( root, depth );
		}

		void put( const utuple & u, bool value ) {
			_da_elem * cur = root;
			for ( int i = 0; i < depth-1; i++ ) {
				cur = cur->arr[u[i]];
			}
			cur->vec->at(u[depth-1]) = value;
		}

		bool get( const utuple & u ) const {
			_da_elem * cur = root;
			for ( int i = 0; i < depth-1; i++ ) {
				cur = cur->arr[u[i]];
			}
			return cur->vec->at(u[depth-1]);
		}

		bool get( const std::vector<cphf_Symbol> & u ) const {
			_da_elem * cur = root;
			for ( int i = 0; i < depth-1; i++ ) {
				cur = cur->arr[u[i].asInt()];
			}
			return cur->vec->at(u[depth-1].asInt());
		}
	};
};