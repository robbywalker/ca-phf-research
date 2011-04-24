#pragma once

#include "../../tabu/tabu/search.h"
#include "../../tabu/arrays/symbol.h"
#include "../../tabu/util/util.h"
#include "../../tabu/globals.h"
#include "../../tabu/arrays/arrayTabu.h"
#include "state.h"

#include <list>
#include <set>
#include <utility>

namespace coverage {
	typedef tabu::TabuSearcher< state, arrays::Array<SYM>, fullScore > coverage_tabu_base;

	class coverage_tabuSearcher : public coverage_tabu_base
	{
		int cols, rows, syms, strength;

		arrays::TabuList<SYM> tabuList;
	public:
		coverage_tabuSearcher( const arrays::Array<SYM> & input, int syms, int strength ) : 
		  coverage_tabu_base( state(input,syms,strength), NULL ),
			  tabuList( &input, tabuCount )
		{
			this->syms = syms;
			this->strength = strength;
			this->cols = input.getWidth();
			this->rows = input.getHeight();

			printf("Removing easily removable rows.\n");
			int rCount = current.collapseRows();
			printf("Removed %d to leave %d.\n\n", rCount, (input.getHeight() - rCount));

			bestScore = current.getScore();

			current.saveToBest();
			current.writeBest();
		}

		~coverage_tabuSearcher() {
		}

		// returns false if no moves could be generated
		virtual bool generateMovesAndPerformBest() {
			int bestC = -1;

			long bestDelta = 0;
			int bestI = -1;
			int replacedI = -1;
			bool collapsedRow = false;

			int bestR = -1;
			bool hasMoves = false;
			int movesTried = 0;
			int r;

			do {
				r = current.getRandomRow();
				if ( r == -1 )
					return false;

//				if ( current.getRowFlexCount( r ) ) {
//					hasMoves = true;

					for ( int c = 0; c < cols && !collapsedRow; c++ ) {

						if ( current.getPositionCount( r, c ) == 0 ) {
							for ( int i = 0; i < syms && !collapsedRow; i++ ) {
								if ( i != current.getPosition(r,c) ) {

									if ( ! tabuList.isTabu( r, c, i ) ) {
										// here is a move
										long delta = current.computePositionChangeDelta( r, c, i, DEFAULT_EXP, &collapsedRow );

										if ( bestI == -1 || delta > bestDelta || collapsedRow ) {
											bestI = i;
											bestC = c;
											bestR = r;
											bestDelta = delta;
											replacedI = current.getPosition(r,c);
										}  else if ( delta == bestDelta && (util::random(3) == 1)) {
											bestI = i;
											bestC = c;
											bestR = r;
											bestDelta = delta;
											replacedI = current.getPosition(r,c);
										}
									}
								}
							}
						}
					}
				

				if ( bestR == -1 ) {
					printf("Got stuck!\n");
					return false;
				}

			} while ( bestR == -1 );

			if ( collapsedRow ) {
				current.saveToBest();
				current.writeBest();
			}

			current.changePosition( bestR, bestC, bestI );
			tabuList.makeChange( bestR, bestC, replacedI );
			return true;
		}

		virtual void saveState(long its) {
			current.flush();

			// more things here!
		}
	};

};