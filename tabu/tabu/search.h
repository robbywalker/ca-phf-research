#pragma once

#include "intscore.h"
#include "tabuStatus.h"
#include <time.h>
#include <conio.h>

namespace tabu {
	template <class State, class Result, class ScoreType = intScore>
	class TabuSearcher {
	protected:
		Result best;
		ScoreType bestScore;
		long best_it;
		long first_it;

		const char * stateFile;
		State current;
	public:
		TabuSearcher(State start, const char * stateFile) :
			current( start )
		{
			start.getResultInto(best);
			bestScore = start.getScore();
			best_it = 0;
			first_it = 0;
			this->stateFile = stateFile;
		}

		void resetBest() {
			bestScore = current.getScore();
		}

		const Result& getBest() {
			return best;
		}

		ScoreType getBestScore() {
			return bestScore;
		}

		// RETURNS:
		//	  0 - nothing important happened
		//	  1 - a new best score was found
		//	  -1 - there were no available moves
		//    -2 - stopped!
		int iterate() {
			bool pause = false;
			while ( _kbhit() ) {
				int c = _getch();
				if ( c == 'p' || c == 'P' ) {
					pause = true;					
				}
				if ( c == 's' || c == 'S' ) {
					return -2;
				}
			}

			if ( pause ) {
				printf("\nPausing the search... press any key to resume...\n");
				_getch();
				printf("Resuming!\n");
			}

			if ( !this->generateMovesAndPerformBest() ) {
				return -1;
			}

			if ( current.getScore() < bestScore ) {
				bestScore = current.getScore();
				current.getResultInto(best);
				return 1;
			}

			return 0;
		}

		virtual bool generateMovesAndPerformBest() = 0;

		void search() {
			time_t start_time;
			time( &start_time );

			time_t last_report;
			time( &last_report );
			int reports = 0;

			long its = first_it;
			int res;
			while ( bestScore > 0 ) {
				if ( its == first_it ) {
					printf("Starting score is: ");
					current.getScore().print();
					printf("\n");

					printf("performing first iteration\n");
				}
				res = iterate();
				if ( its == first_it ) {
					printf("done performing first iteration\n");
				}
				its++;
				if ( res == 1 && bestScore > 0 ) {
					printf("\n**** FOUND NEW BEST SCORE = ");
					bestScore.print();
					printf(" (at iteration %ld)!\n", its);

					best.print();
					best_it = its;
					time( &last_report );

					printf("**** NEW BEST SCORE = ");
					bestScore.print();
					printf(" (at iteration %ld)!", its);

				} else if ( res == -1 ) {
					printf("\n!!!! STUCK! (at iteration %ld)\n", its);
					break;
				
				} else if ( res == -2 ) {
					printf("\n!!!! Stopped by user (at iteration %ld) with current score ", its);
					current.getScore().print();
					printf("\n");
					break;
				
				} else {
					time_t now;
					time( &now );

					if ( now - last_report >= 10 ) {
						printf("\n-- ITERATION %ld CURRENT SCORE ", its);
						current.getScore().print();

						reports++;
						last_report = now;
					}					

					if ( reports == 15 ) {
						printf("\n#### CURRENT BEST SCORE = ");
						bestScore.print();
						printf(" (found at iteration %ld)!\n", best_it);

						best.print();	

						printf("#### CURRENT BEST SCORE = ");
						bestScore.print();
						printf(" (found at iteration %ld)!\n", best_it);

						reports = 0;
					}

					if ( current.getScore() == bestScore ) {
						printf(" -TIE- ");
					}
				}

				if ( bestScore > 0 ) {
					if ( iterationLimiting ) {
						if ( (its - first_it) >= iterationLimit ) {
							printf("\n!!!\n!!!!!!! Iteration limit reached - stopping search.\n!!!\n\n");
							break;
						}
					}

					if ( timeLimiting ) {
						time_t now;
						time( &now );

						if ( (now - start_time) >= timeLimit ) {
							printf("\n!!!\n!!!!!!! Time limit reached - stopping search after %ld iterations.\n!!!\n\n", its);
							break;
						}
					}
				}
			}

			if ( bestScore == 0 )
				printf("\n\n********** FOUND SCORE = 0 (at iteration %ld)!\n", its);
			else {
				if ( outputState )
					saveState(its);
				
				printf("\n********** BEST SCORE = ");
				bestScore.print();
				printf(" ; AFTER %ld ITERATIONS\n", its);
			}

			best.print();
			//current.print();
		}

		virtual void saveState(long its) {
			// by default, do nothing
		}
	};
};