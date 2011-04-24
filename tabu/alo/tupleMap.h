#pragma once

// a tuple-map is an array indexed by tuples - it takes some work to do this efficiently

#include "../arrays/tuple.h"
#include "../qca/qcatuple.h"

namespace alo {
	template <class T>
	union _tm_elem {
		_tm_elem ** arr;
		T * data;
	};
	
	template <class T>
	class defaultPtrAllocator {
	public:
		static T * allocate() {
			return new T();
		}
	};

	template <class T>
	class nullPtrAllocator {
	public:
		static T * allocate() {
			return NULL;
		}
	};

	template <class T, class Tuple, class Allocator = defaultPtrAllocator<T> >
	class tupleMap {
	};

	template <class T, class Allocator >
	class tupleMap<T, arrays::tuple, Allocator> {
		int t, k;
		_tm_elem<T> * data;
		int * refs;

		// recursively initialize the data store
		_tm_elem<T> * init( int strength, int cols ) {
			_tm_elem<T> * ret;

			if ( strength == 0 ) {
				// stop when we reach the last index
				ret = new _tm_elem<T>();
				ret->data = Allocator::allocate();

			} else {
				int count = cols - strength + 1;
				ret = new _tm_elem<T>();
				ret->arr = new _tm_elem<T>*[ cols - strength + 1 ];
				for ( int i = 0; i < count; i++ ) {
					ret->arr[i] = init( strength - 1, cols - i - 1 );
				}
			}
			return ret;
		}

		_tm_elem<T> * get( const arrays::tuple &t ) const {
			int s = t.strength();
			int last = -1;
			_tm_elem<T> * cur = data;
			for ( int i = 0; i < s; i++ ) {
                cur = cur->arr[ t.get(i) - last - 1 ];
				last = t.get(i);
			}
			return cur;
		}

	public:
		tupleMap( const arrays::tuple::initParams & p ) {
			data = init( p.t, p.k );		
			t = p.t;
			k = p.k;
			refs = new int;
			*refs = 1;
		}		

		tupleMap( const tupleMap<T, arrays::tuple, Allocator> & copy ) {
			data = copy.data;
			t = copy.t;
			k = copy.k;
			refs = copy.refs;
			*refs = *refs + 1;
		}

		~tupleMap() {
			*refs = *refs - 1;
			if ( *refs == 0 ) {
				delete refs;
				// delete the elements
				for ( int i = t; i > 0; i-- ) {
					arrays::tuple tp(i, k - ( t - i ));

					do {
						_tm_elem<T> * e = get( tp );
						if ( i == t ) {
							if ( e->data != NULL )
								delete e->data;
						} else {
							delete[] e->arr;
						}
						delete e;
					} while ( ++tp );
				}

				delete[] data->arr;
				delete data;
			}
		}

		T* & operator[]( const arrays::tuple & t ) {
            return get(t)->data;		
		}

		const T* & operator[]( const arrays::tuple & t ) const {
            return get(t)->data;		
		}
	};

	template <class T, class Allocator >
	class tupleMap<T, qca::qcatuple, Allocator> {
		typedef tupleMap<T, arrays::tuple, Allocator> innerMap;
		typedef nullPtrAllocator<innerMap> nullAlloc;

		tupleMap< innerMap, arrays::tuple, nullAlloc > inner;

		int k, l;

	public:
		tupleMap( const qca::qcatuple::initParams & p ) : 
		  k(p.k), l(p.l), inner( arrays::tuple::initParams( 2, p.k ) ) 
		{
		}		

		tupleMap( tupleMap<T, qca::qcatuple, Allocator> & copy ) : 
		  k(copy.k), l(copy.l), inner( copy.inner ) 
		{
		}

		T* & operator[]( const qca::qcatuple & t ) {
			innerMap * im = inner[ t.getColumnsTuple() ];
			if ( im == NULL ) {
				im = inner[ t.getColumnsTuple() ] = new innerMap( arrays::tuple::initParams( 2, l ) );
			}

            return (*im)[ t.getCopiesTuple() ];
		}

		const T* & operator[]( const qca::qcatuple & t ) const {
			const innerMap * im = inner[ t.getColumnsTuple() ];
			if ( im == NULL ) {
				im = inner[ t.getColumnsTuple() ] = new innerMap( arrays::tuple::initParams( 2, l ) );
			}

            return (*im)[ t.getCopiesTuple() ];
		}
	};
}