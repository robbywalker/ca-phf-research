#pragma once

#include <set>
#include <deque>
#include "../globals.h"

namespace arrays {

	struct move {
		int i, j;
		int val;

		move( int _i, int _j, int _val ) : i(_i), j(_j), val(_val) {
		}

		static int compare( const move & a, const move & b ) {
			if ( a.i != b.i ) {
				return a.i - b.i;
			} else if ( a.j != b.j ) {
				return a.j - b.j;
			} else {
				return a.val - b.val;
			}
		}

		friend bool operator<( const move & a, const move & b ) {
			return (compare(a,b) < 0);
		}

		friend bool operator>( const move & a, const move & b ) {
			return (compare(a,b) > 0);
		}

		friend bool operator==( const move & a, const move & b ) {
			return (compare(a,b) == 0);
		}

		friend bool operator!=( const move & a, const move & b ) {
			return (compare(a,b) != 0);
		}
	};

	struct stateptr {
		move state;
		stateptr * next;
		stateptr * prev;

		stateptr() : state(-1,-1,-1), next(NULL), prev(NULL) {
		}

		void reset() {
			stateptr * cur = this;
			stateptr * temp;

			while ( cur != NULL ) {
				cur->state.val = -1;
				temp = cur->next;
				cur->next = NULL;
				cur->prev = NULL;
				cur = temp;
			}
		}
	};

	template <class SYM>
	class TabuList {
		std::set<move> disallowed;
		std::deque<move> tabu;
		stateptr ** states;

		const Array<SYM> * array;
		int size;

		void init() {
			printf("Using tabu size: %d\n", size);

			states = new stateptr*[ array->getHeight() ];
			for ( int i = 0; i < array->getHeight(); i++ ) {
				states[i] = new stateptr[ array->getWidth() ];
				for ( int j = 0; j < array->getWidth(); j++ ) {
					states[i][j].state.i = i;
					states[i][j].state.j = j;	
				}
			}
		}

		void destroy() {
			if ( array != NULL ) {
				for ( int i = 0; i < array->getHeight(); i++ ) {
					delete[] states[i];
				}
				delete[] states;
			}
		}

	public:
		TabuList() {
			array = NULL;
			size = tabuCount;			
		}

		TabuList( const Array<SYM> * _array, int _size ) : array(_array), size(_size) {
			init();
		}

		~TabuList() {
			destroy();
		}

		void setArray( const Array<SYM> * _array ) {
			destroy();

			array = _array;			
			init();			
		}

		// report the old value AFTER making the change to the array
		void makeChange( int i, int j, int oldVal ) {
			// update the tabu list
			if ( tabu.size() == size ) {
				tabu.pop_back();
			}

			tabu.push_front( move(i,j,oldVal) );

			// now, we update the disallowed list
			disallowed.clear();

			stateptr * stateFront = NULL;
			int stateCount = 0;
			//int tc = 0;

			std::deque<move>::const_iterator it;
			for ( it = tabu.begin(); it != tabu.end(); it++ ) {
				int row = it->i; int col = it->j;

				if ( ( it->val == array->get(row, col).asInt() ) /*&& (states[row][col].state.val != -1)*/ ) {
					// we need to remove this from the list
					if ( states[row][col].next != NULL ) {
						states[row][col].next->prev = states[row][col].prev;
					}

					if ( states[row][col].prev != NULL ) {
						states[row][col].prev->next = states[row][col].next;
					} else {
						stateFront = states[row][col].next;
					}

					states[row][col].next = NULL;
					states[row][col].prev = NULL;
					states[row][col].state.val = -1;

					stateCount--;

					/*if ( stateFront == NULL && stateCount != 0 ) {
						printf("FATAL: stateFront is NULL but stateCount = %d\n", stateCount);
					}*/

				} else {
					// insert/update
				
					if ( states[row][col].state.val == -1 ) {
						// we need to add this to the list
						states[row][col].state.val = it->val;
						stateCount++;

						/*if ( it->val == -1 ) {
							printf("Error!  Setting val to -1.");
						}*/

						states[row][col].next = stateFront;
						states[row][col].prev = NULL;

						if ( stateFront != NULL ) {
							stateFront->prev = & (states[row][col]);
						}

						stateFront = & (states[row][col]);

					} else {
						// we just need to update the list
						states[row][col].state.val = it->val;
					}
				}

				if ( stateCount == 1 ) {
					disallowed.insert( stateFront->state );
					//tc++;
				}
			}

			//printf(" (%d)", tc);

			if ( stateCount != 0 ) {
				stateFront->reset();
			}
		}

		bool isTabu( int i, int j, int val ) const {
			return disallowed.count( move(i,j,val) ) != 0;
		}

	};

};