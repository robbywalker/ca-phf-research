#include "../tabu/arrays/array.h"
#include "../tabu/arrays/intsymbol.h"
#include "../tabu/arrays/tuple.h"

#include "../tabu/cphf/iterable_utuple.h"

#include "../tabu/util/reportManager.h"

#include "coverage/state.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>

typedef arrays::int_Symbol SYM;

#define SCORE_EXP 1
#define ALPHA 0.965
#define FACTOR 0.12
#define MAX_TEMP 0.0001

inline double maxd( double a, double b ) {
	if ( a > b ) return a;
	else return b;
}

void main(int argc, char* argv[]) {
	srand( (unsigned)time( NULL ) );

	int syms, strength;
	arrays::Array<SYM> input( argv[1], syms, strength );

	coverage::state cState(input, syms, strength);

	printf("Removing easily removable rows.\n");
	int rCount = cState.collapseRows();
	printf("Removed %d.\n\n", rCount);

	cState.saveToBest();
	cState.writeBest();

	long movesPerTemperature = (int) (input.getWidth() * input.getHeight() * FACTOR);
	long curMove = 0;
	long totalMoves = 0;
	long movesTaken = 0;

	double temperature = MAX_TEMP;

	double score = cState.getScore(SCORE_EXP);

	util::reportManager report;

	double bestScore = score;

	printf("Starting score: %0.7f\n", score);

	bool collapsedRow, keep;
	double delta;
	do {
		coverage::legalMove move = cState.generateRandomMove();
		if ( move.valid == false ) {
			printf("No more valid moves exist.\n");
			break;
		}

		// peform the move
		delta = cState.computePositionChangeDelta( move.row, move.col, move.value, SCORE_EXP, &collapsedRow );

		// determine whether to keep it
		keep = false;
		if ( collapsedRow || ( delta > 0 ) ) {
			// keep it
			keep = true;
		} else {
			// keep it with some probability

			// TODO: this scoring system needs to be revamped
			double p = exp( (-1 * ( - delta )) / (score * temperature) );
			if ( rand() < RAND_MAX * p ) {
				keep = true;
			}			
		}

		if ( keep ) {			
			cState.changePosition( move.row, move.col, move.value );

			score += delta;
			if ( score > bestScore || collapsedRow ) {
				bestScore = score;
				cState.saveToBest();
				cState.writeBest();
			}			

			if ( collapsedRow ) {
				bestScore = score = cState.getScore(SCORE_EXP);

				if ( collapsedRow ) {
					printf("****Height is now: %d! (resetting temperature to %0.7f)\n", cState.getBestHeight(), MAX_TEMP);

					//reset the temp
					temperature = MAX_TEMP;
					curMove = 0;
				}
			}

			movesTaken++;
		}

		// USER INTERFACE - SHOULD WE PAUSE OR STOP?
		bool pause = false;
		bool stop = false;
		while ( _kbhit() && !stop ) {
			int c = _getch();
			if ( c == 'p' || c == 'P' ) {
				pause = true;					
			}
			if ( c == 's' || c == 'S' ) {
				printf("Stopping!\n");
				stop = true;
				break;
			}
		}

		if ( stop )
			break;

		if ( pause ) {
			printf("\nPausing the search... press any key to resume...\n");
			_getch();
			printf("Resuming!\n");
		}


		// test if the temperature should be lowered
		totalMoves++;
		if ( ++curMove == movesPerTemperature ) {
			temperature *= ALPHA;
			curMove = 0;
			printf("Setting temperature to %0.7f\n", temperature);
		}

		// USER INTERFACE - REPORT EVERY 5 SECONDS
		if ( report.reportRequired() ) {
            printf("Considered %ld moves, taken %ld, score is %0.7f\n", totalMoves, movesTaken, score);
			report.reported();
		}
	} while ( temperature > 0.0000001 );

	printf("Final score was: %ld\n", score);

	printf("Writing cached data to disk\n");
	cState.flush();

	printf("Original had %d rows\n", input.getHeight() );
	printf("Result has %d rows!\n", cState.getBestHeight() );
}