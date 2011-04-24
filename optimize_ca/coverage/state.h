#pragma once

#include "../coverage/recompute.h"
#include "../coverage/memory.h"
#include "../../tabu/util/reportManager.h"

#include <math.h>
#include <utility>

#define DEFAULT_EXP 2

namespace coverage {
	typedef arrays::int_Symbol SYM;

	struct legalMove {
		legalMove() {
			valid = true;
		}

		legalMove(bool valid) {
			this->valid = valid;
		}

		int row;
		int col;
		int value;
		bool valid;
	};

	class fullScore : public std::pair<int,double> {
	public:
		fullScore() : std::pair<int,double>(0,0) {
		}

		fullScore( int height ) : std::pair<int,double>(height,0) {
		}

		fullScore( int height, double score ) : std::pair<int,double>(height,score) {			
		}

		void print() {
			printf("[%d,%0.3f]",first,second);
		}

		void fprint(FILE * out) {
			fprintf(out, "[%d,%0.3f]",first,second);
		}
	};

	bool operator> (const fullScore & a, int b ) {
		return a.first > b;
	}

	bool operator== (const fullScore & a, int b ) {
		return a.first == b;
	}

	class state {
		arrays::Array<SYM> input;
		arrays::Array<SYM> * best;

		int * rowCounts;
		int * flexCounts;
		int ** posCounts;

		int * rowCountsTemp;
		int * flexCountsTemp;
		int ** posCountsTemp;

		int syms;
		int strength;

		// changing this will require a change to the analyze method
		recomputeStore<SYM> covered;
		// changing this will require a change to the analyze method

		coverage::intSet removable;
		coverage::intSet goodRows;

		int scoreValidFor;
		long lScore;
		double score;

		int maxScoreValidFor;
		double maxScore;

		char dir[255];

		util::reportManager report;

		arrays::int_SymbolGenerator * sg;

		int * goodCols;
		int * refs;

	public:
		state( const state & copy ) : input( copy.input ), best( copy.best ), rowCounts( copy.rowCounts ),
			flexCounts( copy.flexCounts ), posCounts( copy.posCounts ), rowCountsTemp( copy.rowCountsTemp ),
			flexCountsTemp( copy.flexCountsTemp ), posCountsTemp( copy.posCountsTemp ), syms( copy.syms ),
			strength( copy.strength ), covered( copy.covered ), removable( copy.removable ), goodRows( copy.goodRows ),
			scoreValidFor( copy.scoreValidFor ), score( copy.score ), maxScoreValidFor( copy.maxScoreValidFor ),
			maxScore( copy.maxScore ), report( copy.report ), sg( copy.sg ), goodCols( copy.goodCols ), refs( copy.refs )
		{
			covered.setArray( &input );
			covered.setRemovable( &removable );
			strcpy( dir, copy.dir );
			(*refs)++;
		}

		state( const arrays::Array<SYM> & _input, int _syms, int _strength ) : input(_input), syms(_syms), strength(_strength),
			                                                                   covered( input.getHeight(), input.getWidth(), _syms, _strength, true )
		{
			invalidateScore();
			invalidateMaxScore();

			int i, j;

			sg = new arrays::int_SymbolGenerator( syms );
			best = NULL;

			rowCounts = new int[ input.getHeight() ];
			flexCounts = new int[ input.getHeight() ];
			rowCountsTemp = new int[ input.getHeight() ];
			flexCountsTemp = new int[ input.getHeight() ];
			posCounts = new int*[ input.getHeight() ];
			posCountsTemp = new int*[ input.getHeight() ];
			for ( i = 0; i < input.getHeight(); i++ ) {
				rowCounts[i] = 0;
				flexCounts[i] = input.getWidth();
				posCounts[i] = new int[ input.getWidth() ];
				posCountsTemp[i] = new int[ input.getWidth() ];
				for ( j = 0; j < input.getWidth(); j++ ) {
					posCounts[i][j] = 0;
				}
			}

			goodCols = new int[ input.getWidth() ];

			sprintf(dir, "%d-%d-%d-%d.tmp", input.getHeight(), input.getWidth(), syms, strength );

			covered.setArray( &input );
			covered.setRemovable( &removable );

			if ( covered.isEmpty() ) {
				// perform the analysis
				analyze();
				saveToBest();

			} else {
				// load our state array
				char in2Filename[255];
				sprintf(in2Filename,"%s\\current.ca",dir);
				arrays::Array<SYM> in2(in2Filename, syms, strength );
				in2.copyInto( input );

				// load the "best" array
				loadBest();

				// load status variables
				char rcFilename[255];
				sprintf(rcFilename,"%s\\rowcounts.txt",dir);

				char fcFilename[255];
				sprintf(fcFilename,"%s\\flexcounts.txt",dir);

				char pcFilename[255];
				sprintf(rcFilename,"%s\\poscounts.txt",dir);

				FILE * rc = fopen(rcFilename, "rt");
				FILE * fc = fopen(fcFilename, "rt");
				FILE * pc = fopen(pcFilename, "rt");
				for ( i = 0; i < input.getHeight(); i++ ) {
					fscanf( rc, "%d\n", &(rowCounts[i]) );
					fscanf( fc, "%d\n", &(flexCounts[i]) );

					for ( j = 0; j < input.getWidth(); j++ ) {
						fscanf( pc, "%d ", &(posCounts[i][j]) );
					}
				}
				fclose(rc);
				fclose(pc);
				fclose(fc);

				char remFilename[255];
				sprintf(remFilename, "%s\\removable.txt");

				FILE * rem = fopen(remFilename, "rt");
				int rCount, temp;
				fscanf( rem, "%d\n", &rCount );

				for ( i = 0; i < rCount; i++ ) {
					fscanf( rem, "%d ", &temp );
					removable.insert( temp );
				}

				fclose( rem );

				// recompute the good rows				  
				refreshGoodRows();
				  
			}

			refs = new int;
			*refs = 1;
		}

		~state() {
			if ( --(*refs) == 0 ) {
				delete[] rowCounts;
				delete[] flexCounts;
				delete[] posCounts;
				delete[] goodCols;
				if ( best != NULL )
					delete best;
				delete sg;
				delete refs;
			}
		}

		void getResultInto(arrays::Array<SYM> & s) {
			int newHeight = input.getHeight() - (int) removable.size();

			arrays::Array<SYM> temp( sg, newHeight, input.getWidth(), false );

			int outI = 0;
			for ( int i = 0; i < input.getHeight(); i++ ) {
				if ( removable.count( i ) == 0 ) {
					for ( int j = 0; j < input.getWidth(); j++ ) {
						temp.set(outI, j, input.get(i,j));	
					}
					outI++;
				}
			}

			temp.copyInto( s );
		}

		void flush() {
			covered.persist();
			writeBest();

			// save our state array
			char outFilename[255];
			sprintf(outFilename,"%s\\current.ca",dir);
			FILE * out = fopen( outFilename, "wt" );
			input.fprint( out, syms, strength );

			// save status variables
			char rcFilename[255];
			sprintf(rcFilename,"%s\\rowcounts.txt",dir);

			char fcFilename[255];
			sprintf(fcFilename,"%s\\flexcounts.txt",dir);

			char pcFilename[255];
			sprintf(pcFilename,"%s\\poscounts.txt",dir);

			FILE * rc = fopen(rcFilename, "wt");
			FILE * fc = fopen(fcFilename, "wt");
			FILE * pc = fopen(pcFilename, "wt");
			for ( int i = 0; i < input.getHeight(); i++ ) {
				fprintf( rc, "%d\n", rowCounts[i] );
				fprintf( fc, "%d\n", flexCounts[i] );

				for ( int j = 0; j < input.getWidth(); j++ ) {
					fprintf( pc, "%d ", posCounts[i][j] );
				}

				fprintf( pc, "\n" );
			}
			fclose(rc);
			fclose(pc);
			fclose(fc);

			char remFilename[255];
			sprintf(remFilename, "%s\\removable.txt");

			FILE * rem = fopen(remFilename, "wt");
			fprintf( rem, "%d\n", (int) removable.size() );

			intSet::const_iterator it;
			for ( it = removable.begin(); it != removable.end(); ++it ) {
				fprintf( rem, "%d ", *it );		
			}

			fclose( rem );
		}

		int getRowCount(const int i) {
			return rowCounts[i];
		}

		int getRowFlexCount(const int i) {
			if ( removable.count( i ) == 0 )
				return flexCounts[i];
			else
				return 0;
		}

		int getFlexibleCount() {
			return (int) getScore( 1 );
		}

		int getPositionCount(const int i, const int j) {
			return posCounts[i][j];
		}

		fullScore getScore() {
			return fullScore( input.getHeight() - (int) removable.size(), 
				              getMaxScore( DEFAULT_EXP ) - getScore( DEFAULT_EXP ) );
		}

		double getScore(const int exp) {
			getLScore(exp);
			return score;
		}

		inline long getLScore(const int exp) {
			if ( scoreValidFor == exp ) {
				return lScore;
			}

			lScore = 0;

			for ( int k = 0; k < input.getHeight(); k++ ) {
				if ( removable.count( k ) == 0 ) {
					lScore += util::lpow( flexCounts[k], exp );
				}
			}

			score = pow( (double) lScore, 1.0 / exp );
			scoreValidFor = exp;

			return lScore;
		}

		double getMaxScore(const int exp) {
			if ( maxScoreValidFor == exp )
				return maxScore;

			maxScore = util::lpow(input.getWidth()-1, exp) * ( input.getHeight() - (long) removable.size() );
			maxScore = pow( maxScore, 1.0 / exp );

			maxScoreValidFor = exp;

			return maxScore;
		}

		int collapseRows() {
			int count = 0;
			std::vector<SYM> slice(strength);

			// find out if any rows have become collapsible
			cphf::iterable_utuple u(syms, strength);
			for ( int i = 0; i < input.getHeight(); i++ ) {
				if ( removable.count(i) == 0 && rowCounts[i] == 0 ) {
					#ifdef DEBUG_ASSERTS
						if ( flexCounts[i] != input.getWidth() ) {
							printf("Fatal error: rowCounts is 0 but flexCounts is not full\n");
							exit(1);
						}
					#endif DEBUG_ASSERTS

					// remove the row
					collapseRow( i );
					count++;							
				}
			}

			if ( count > 0 ) {
				invalidateScore();
				invalidateMaxScore();
				refreshGoodRows();
			}

			return count;
		}

		void saveToBest( ) {
			int newHeight = input.getHeight() - (int) removable.size();

			if ( best != NULL ) delete best;
			best = new arrays::Array<SYM>( sg, newHeight, input.getWidth(), false );

			int outI = 0;
			for ( int i = 0; i < input.getHeight(); i++ ) {
				if ( removable.count( i ) == 0 ) {
					for ( int j = 0; j < input.getWidth(); j++ ) {
						best->set(outI, j, input.get(i,j));	
					}
					outI++;
				}
			}
		}

		int getPosition( const int row, const int col ) {
			return input.get( row, col ).asInt();
		}

		int changePosition( int row, int col, int newVal ) {
			if ( posCounts[row][col] != 0 ) {
				printf("Fatal error: attempting to change non-flexible position (%d, %d).\n", row, col);
				exit(1);
			}
			
			// change the value 
			int oldVal = input.get(row, col).asInt();
			input.set(row,col,newVal);

			if ( oldVal == newVal ) {
				printf("Fatal error: non-change.\n");
				exit(1);
			}

			// now, iterate over the affected tuples
			arrays::tuple t(strength - 1, input.getWidth() - 1);
			arrays::tuple fullTuple(strength, input.getWidth());
			std::vector<SYM> slice(strength);
			cphf::iterable_utuple oldU(syms, strength), newU(syms, strength);

			int j, uRow;

			do {		
				fullTuple.initViaUpgrade(t, col);

				input.sliceInto(row, fullTuple, slice);
				for ( j = 0; j < strength; j++ ) {
					if ( fullTuple[j] == col ) {
						oldU[j] = oldVal;
						newU[j] = newVal;
					} else {
						oldU[j] = slice[j].asInt();
						newU[j] = slice[j].asInt();
					}
				}

				// test for new flexible positions caused by gain of newU
				input.set(row,col,oldVal);
				uRow = covered.getUnique(fullTuple,newU);
				input.set(row,col,newVal);

				covered.setCovered(fullTuple,newU,row);
				covered.unsetCovered(fullTuple,oldU,row);
				if ( uRow != -1 ) {
					rowCounts[ uRow ]--;

					for ( j = 0; j < strength; j++ ) {
						posCounts[ uRow ][ fullTuple[ j ] ]--;
						if ( posCounts[ uRow ][ fullTuple[ j ] ] == 0 ) {
							flexCounts[uRow]++;
						}
					}
				}

				// test for new uniques caused by loss of oldU		
				uRow = covered.getUnique(fullTuple,oldU);
				if ( uRow != -1 ) {
					rowCounts[ uRow ]++;

					for ( j = 0; j < strength; j++ ) {
						posCounts[ uRow ][ fullTuple[ j ] ]++;
						if ( posCounts[ uRow ][ fullTuple[ j ] ] == 1 ) {
							flexCounts[uRow]--;
						}
					}
				}

			} while (++t);

			invalidateScore();

			int collapseCount = collapseRows();

			refreshGoodRows();			
			
			return collapseCount;
		}


		
		// this method will return one of two things:
		//   1. an accurate delta, with collapsedRow = false
		//   2. delta = 0, but collapsedRow = true
		// it is not possible to compute an accurate delta when it involves a row collapse
		long computePositionChangeDelta( int row, int col, int newVal, int exp, bool * collapsedRow ) {
			*collapsedRow = false;

			#ifdef DEBUG_ASSERTS
				if ( posCounts[row][col] != 0 ) {
					printf("Fatal error: attempting to change non-flexible position.\n");
					exit(1);
				}
			#endif
		
			// pretend to change the value 			
			int oldVal = input.get(row, col).asInt();

			#ifdef DEBUG_ASSERTS
				if ( oldVal == newVal ) {
					printf("Fatal error: non-change.\n");
					exit(1);
				}
			#endif

			// create some fake state variables
			int i, j;
			for ( i = 0; i < input.getHeight(); i++ ) {
				rowCountsTemp[i] = rowCounts[i];
				flexCountsTemp[i] = flexCounts[i];
				for ( j = 0; j < input.getWidth(); j++ ) {
					posCountsTemp[i][j] = posCounts[i][j];
				}
			}

			// now, iterate over the affected tuples
			arrays::tuple t(strength - 1, input.getWidth() - 1);
			arrays::tuple fullTuple(strength, input.getWidth());
			cphf::iterable_utuple oldU(syms, strength), newU(syms, strength);

			int uRow;

			do {		
				fullTuple.initViaUpgrade(t, col);

				for ( j = 0; j < strength; j++ ) {
					if ( fullTuple[j] == col ) {
						oldU[j] = oldVal;
						newU[j] = newVal;
					} else {
						oldU[j] = newU[j] = input.get( row, fullTuple[j] ).asInt();
					}
				}

				// test for new flexible positions caused by gain of newU
				uRow = covered.getUnique(fullTuple,newU);

				if ( uRow != -1 ) {
					rowCountsTemp[ uRow ]--;

					for ( j = 0; j < strength; j++ ) {
						posCountsTemp[ uRow ][ fullTuple[ j ] ]--;
						if ( posCountsTemp[ uRow ][ fullTuple[ j ] ] == 0 ) {
							flexCountsTemp[uRow]++;
						}
					}
				}

				// test for new uniques caused by loss of oldU		
				if ( covered.getCount(fullTuple,oldU) == 2 ) {
					uRow = covered.getDuo(fullTuple,oldU,row);
					rowCountsTemp[ uRow ]++;

					for ( j = 0; j < strength; j++ ) {
						posCountsTemp[ uRow ][ fullTuple[ j ] ]++;
						if ( posCountsTemp[ uRow ][ fullTuple[ j ] ] == 1 ) {
							flexCountsTemp[uRow]--;
						}
					}
				}

			} while (++t);

			for ( i = 0; i < input.getHeight(); i++ ) {
				if ( removable.count(i) == 0 && rowCountsTemp[i] == 0 ) {
					*collapsedRow = true;
					return 0;
				}
			}

			return getFakeScore(exp) - getLScore(exp);
		}

		int getRandomRow() {
			if ( goodRows.size() == 0 ) {
				// return the "no moves" identifier
				return -1;
			}

			// choose a move randomly
			int sel = util::random( (int) goodRows.size() );
			coverage::intSet::iterator it = goodRows.begin();

			int s;
			for ( s = 0; s < sel; s++, ++it ) {
			}

			return *it;
		}

		legalMove generateRandomMove() {
			legalMove m;
			int s;

			m.row = getRandomRow();

			if ( m.row == -1 ) {
				// return the "no moves" identifier
				return legalMove( false );
			}

			int colCount = 0;
			for ( s = 0; s < input.getWidth(); s++ ) {
				if ( posCounts[m.row][s] == 0) {
					goodCols[colCount++] = s;
				}
			}

			m.col = goodCols[ util::random( colCount ) ];

			int curVal = input.get( m.row, m.col ).asInt();
			m.value = util::random( syms - 1 );

			if ( m.value >= curVal )
				m.value++;

			return m;
		}
		int getBestHeight() {
			if ( best == NULL ) {
				return input.getHeight();
			} else {
				return best->getHeight();
			}
		}
		void writeBest() {
			if ( best == NULL ) 
				saveToBest();

			char buffer[255];
			
			sprintf(buffer, "%s\\%d-%d-%d.best.ca", dir, strength, best->getWidth(), syms );

			FILE * out = fopen( buffer, "wt" );
			best->fprint(out, syms, strength);

			fclose( out );
		}

	protected:
		void loadBest() {
			if ( best != NULL ) delete best;

			char buffer[255];			
			sprintf(buffer, "%s\\%d-%d-%d.best.ca", dir, strength, best->getWidth(), syms );

			int tSyms, tStrength;
			best = new arrays::Array<SYM>(buffer, tSyms, tStrength);
		}

		void invalidateScore() {
			scoreValidFor = 0;
		}

		void invalidateMaxScore() {
			maxScoreValidFor = 0;
		}

		void analyze() {
			printf("Analyzing the array.\n");

			arrays::tuple t(strength, input.getWidth());
			cphf::iterable_utuple u(syms, strength);

			int i, j;

			do {
				if ( report.reportRequired() ) {
					printf("Current tuple: ");
					t.print();
					printf("\n");

					report.reported();
				}

				for ( i = 0; i < input.getHeight(); i++ ) {
					for ( j = 0; j < strength; j++ ) {
						u[j] = input.get( i, t[ j ] ).asInt();
					}

					// this may be commented only when using recompute store
					//covered.setCovered( t, u, i );
				}

				cphf::iterable_utuple uIt( syms, strength );
				do {
					if ( covered.isUnique( t, uIt ) ) {
						int row = covered.getUnique( t, uIt );
						rowCounts[ row ]++;

						for ( j = 0; j < strength; j++ ) {
							posCounts[ row ][ t[ j ] ]++;

							if ( posCounts[ row ][ t[ j ] ] == 1 ) {
								flexCounts[row]--;
							}
						}
					}
				} while ( uIt++ );
			} while ( ++t );

			refreshGoodRows();
		}

		void refreshGoodRows() {
			goodRows.clear();
			for ( int k = 0; k < input.getHeight(); k++ ) {
				if ( removable.count( k ) == 0 ) {
					if ( flexCounts[k] > 0 )
						goodRows.insert( k );
				}
			}
		}

		void collapseRow(int row) {
			std::vector<SYM> slice(strength);
			cphf::iterable_utuple u(syms, strength);

			removable.insert( row );

			int j;

			// update uniqueness state
			arrays::tuple t(strength, input.getWidth());
			do {
				input.sliceInto(row, t, slice);
				for ( j = 0; j < strength; j++ ) {
					u[j] = slice[j].asInt();
				}

				covered.unsetCovered( t, u, row );

				int nRow = covered.getUnique(t, u);
				if ( nRow != -1 ) {
					rowCounts[ nRow ]++;

					for ( j = 0; j < strength; j++ ) {
						posCounts[ nRow ][ t[ j ] ]++;
						if ( posCounts[ nRow ][ t[ j ] ] == 1 ) {
							flexCounts[nRow]--;
						}
					}
				}
			} while ( ++t );
		}

		long getFakeScore(const int exp) {
			long fakeScore = 0;

			for ( int k = 0; k < input.getHeight(); k++ ) {
				if ( removable.count( k ) == 0 ) {
					fakeScore += util::lpow( flexCountsTemp[k], exp );
				}
			}

			return fakeScore;
		}
	};


};