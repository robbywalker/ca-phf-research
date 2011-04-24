#pragma once

#include "../tabu/search.h"
#include "../arrays/symbol.h"
#include "../arrays/arrayTabu.h"
#include "../util/util.h"
#include "../alo_ofeach/tabustatus.h"

#include <list>
#include <set>
#include <utility>

namespace alo_ofeach {
	template <class SYM, class Tuple=arrays::tuple>
	class alo_ofeach_tabuSearcher : public tabu::TabuSearcher< tabuStatus<SYM,Tuple>, arrays::Array<SYM> >
	{
		int * counts;
		int rows;
		int cols;

		int test;
		int syms;

		arrays::TabuList<SYM> tabuList;
	public:
		void initFromFile( arrays::SymbolGenerator<SYM> * sg, const char * stateFile )
		{
			FILE * state = fopen( stateFile, "rt" );
			if ( state == 0 ) {
				printf("Could not open state file!\n");
				exit(1);
			}

			fscanf( state, " %ld ", &first_it );

			int syms, strength;
			arrays::Array<SYM> arr( state, syms, strength );
			arr.gen = sg;
        
			fscanf( state, " %ld %d ", &best_it, &bestScore );
			arrays::Array<SYM> temp( state, syms, strength );
			temp.copyInto( best );

			if ( startWithBest ) {
				printf("Starting with best result.\n");
				temp.copyInto( arr );
				arr.gen = sg;
			}

			arr.print();
			current.reInit( arr, strength );

			fclose( state );
		}

		alo_ofeach_tabuSearcher( arrays::SymbolGenerator<SYM> * sg, int rows, int cols, int syms, int strength, SYM ** searches, int count, const char * stateFile, bool useFile ) : 
		  tabu::TabuSearcher< tabuStatus<SYM,Tuple>, arrays::Array<SYM> >( tabuStatus<SYM,Tuple>( arrays::Array<SYM>( sg, rows, cols, true ), strength, !useFile, searches, count ), stateFile )
		{
			counts = new int[cols];
			this->rows = rows;
			this->cols = cols;
			test = 0;
			this->syms = syms;

			if ( useFile ) {
				initFromFile(sg,stateFile);
			}
		}

		alo_ofeach_tabuSearcher( arrays::SymbolGenerator<SYM> * sg, int rows, int cols, int syms, int strength, SYM ** searches, int count, const char * stateFile, bool useFile, const typename Tuple::initParams & params ) : 
		  tabu::TabuSearcher< tabuStatus<SYM,Tuple>, arrays::Array<SYM> >( tabuStatus<SYM,Tuple>( arrays::Array<SYM>( sg, rows, cols, true ), strength, !useFile, searches, count, params ), stateFile )
		{
			counts = new int[cols];
			this->rows = rows;
			this->cols = cols;
			test = 0;
			this->syms = syms;

			if ( useFile ) {
				initFromFile(sg,stateFile);
			}
		}

		~alo_ofeach_tabuSearcher() {
			delete[] counts;
		}

		// returns false if no moves could be generated
		virtual bool generateMovesAndPerformBest() {
			int total = current.countBadPerColumn( counts );
			int colToChange = util::weightedRandom(counts, total);

			int columnsWithBad = 0;
			if ( fullSearch ) {
				for ( int i = 0; i < cols; i++ ) {
					if ( counts[i] != 0 ) {
						columnsWithBad++;
					}
				}
			} else {
				columnsWithBad = 1;
			}

			int bestDelta = 0;
			int bestI = -1;
			int bestR = -1;
			int bestC = -1;
			int replacedI = -1;

			arrays::SymbolGenerator<SYM> * sGen = current.getSymbolGenerator();
			int count = sGen->getCount();			

			int prob;

			int moves = (count - 1) * rows * columnsWithBad;
			if ( moveLimiting ) {				
				if ( moves > moveLimit ) {
					prob = (int) ((((double) moveLimit) / moves) * RAND_MAX);
				} else {
					prob = RAND_MAX;
				}
			}

			long movesSoFar = 0;

			for ( int c = 0; c < cols; c++ ) {
				if ( c != colToChange && ( !fullSearch || counts[c] == 0 ) ) 
					continue;

				for ( int r = 0; r < rows; r++ ) {
					SYM curS = current.getArrayValue( r, c );

					for ( int i = 0; i < count; i++ ) {
						SYM value = sGen->generate( i );

						if ( ! tabuList.isTabu( r, c, i ) ) {

							if ( ! (value == curS) ) {
								if ( !moveLimiting || rand() <= prob ) { // limit moves to around moveLimit
									int delta = current.calculateEffectOfChange( r, c, value );
									movesSoFar++;

									if ( moves > 3000 && movesSoFar % 1000 == 0) {
										if (movesSoFar == 1000) {
											printf("\n");
										}
										printf("__processed %ld moves of %d within iteration - best delta is %d\n", movesSoFar, moves, bestDelta);
									}

									if ( bestI == -1 || delta < bestDelta ) {
										bestI = i;
										bestR = r;
										bestC = c;
										bestDelta = delta;
										replacedI = curS.asInt();
									} else if ( delta == bestDelta && (util::random(3) == 1)) {
										bestI = i;
										bestR = r;
										bestC = c;
										bestDelta = delta;
										replacedI = curS.asInt();
									}
								}
							}

						}
					}
				}
			}

			SYM value = sGen->generate( bestI );
			current.change( bestR, bestC, value );	

			// update the tabu list
			tabuList.makeChange( bestR, bestC, replacedI );

			// we may wish to consider "column ror" as a sort of larger move - where every entry in the
			// column is changed (rotated right) by a specific value

			return true;
		}

		// save state information
		virtual void saveState(long its) {
			FILE * f = fopen( stateFile, "wt" );

			if ( f == 0 ) {
				printf("Cannot open state file for writing!\n");
				exit(1);
			}

            fprintf(f, "%ld\n", its);
			current.getArr().fprint( f, syms, current.getStrength() );

			fprintf(f, "%ld %d\n", best_it, bestScore );
			best.fprint( f, syms, current.getStrength() );

			fclose( f );
		}
	};

};