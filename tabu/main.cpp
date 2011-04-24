#include "arrays/array.h"
#include "arrays/intsymbol.h"
#include "arrays/tuple.h"

#include "alo/checkArray.h"
#include "alo/tabuStatus.h"
#include "alo/tabusearcher.h"

#include "alo_ofeach/tabustatus.h"
#include "alo_ofeach/tabusearcher.h"

#include "sums/check.h"

#include "cphf/iterable_utuple.h"
/*#include "cphf/cphfsymbol.h"
#include "cphf/check.h"
#include "cphf/less_space_check.h"
#include "cphf/medium_space_check.h"*/

#include "math/galois_field.h"

#include "words/lettersymbol.h"
#include "words/check.h"

#include "phf/check.h"

#include "qca/qcatuple.h"

//#include "phf-c1/phfc1symbol.h"
//#include "phf-c1/check.h"

#include "multicheck/tabusearcher.h"

#include <stdlib.h>
#include <time.h>
#include <io.h>

#include "globals.h"
#include <GetPot>

typedef arrays::int_Symbol SYM;
typedef phf::phf_Check<SYM> ROW_CHECK_PHF;
typedef sums::sums_Check<SYM> ROW_CHECK_SUMS;

/*typedef cphf::cphf_Symbol C_SYM;
typedef cphf::cphf_Check<C_SYM> ROW_CHECK_CPHF;
typedef cphf::cphf_less_space_Check<C_SYM> ROW_CHECK_LS_CPHF;
typedef cphf::cphf_medium_space_Check<C_SYM> ROW_CHECK_MS_CPHF;*/

typedef words::letter_Symbol L_SYM;
typedef words::word_Check<L_SYM> ROW_CHECK_WORDS;

/*typedef phf_c1::phf_c1_Symbol PHF_C1_SYM;
typedef phf_c1::phf_c1_CheckT1<PHF_C1_SYM> PHF_C1_CHECK1;
typedef phf_c1::phf_c1_CheckT2_1<PHF_C1_SYM> PHF_C1_CHECK2_1;
typedef phf_c1::phf_c1_CheckT2_2<PHF_C1_SYM> PHF_C1_CHECK2_2;
typedef phf_c1::phf_c1_CheckT3<PHF_C1_SYM> PHF_C1_CHECK3;*/

void doTabuPHF(int rows, int cols, int syms, int strength) {
	char stateFile[50];
	sprintf(stateFile, "inprogress/%d-%d-%d-%d.phf.state", rows, cols, syms, strength);

	srand( (unsigned)time( NULL ) );
	
	alo::alo_tabuSearcher<SYM,ROW_CHECK_PHF> ts(&arrays::int_SymbolGenerator(syms),rows,cols,syms,strength,stateFile,(_access(stateFile, 0) != -1) && !startOver);
	ts.search();

	if ( ts.getBestScore() == 0 ) {
		arrays::Array<SYM> result;
		ts.getBest().copyInto( result );

		char filename[255];
		sprintf(filename, "%d-%d-%d-%d.phf", rows, cols, syms, strength);
		FILE * out = fopen(filename, "wt");
		result.fprint( out, syms, strength );
		fclose(out);
	}
}

/*void doTabuPHF_C1(int rows, int cols, int syms, int size) {
	printf("Searching for strength 3 PHF-C1.\n");
	// STRENGTH 3 ONLY

	char stateFile[50];
	sprintf(stateFile, "inprogress/%d-%d-%d.phf-c1.state", rows, cols, syms);

	srand( (unsigned)time( NULL ) );

	phf_c1::phf_c1_SymbolGenerator symG( syms, size );
	arrays::Array<PHF_C1_SYM> arr( &symG, rows, cols, true );
    
	tabu::tabuStatus<PHF_C1_SYM> ** statuses;

	int count;

	if ( syms - size < size ) {
		printf( "Size too big.\n" );
		exit(1);
	}

	if ( syms >= 3 + size ) {
		statuses = new tabu::tabuStatus<PHF_C1_SYM>* [4];
		
		statuses[3] = new alo::tabuStatus<PHF_C1_CHECK1,PHF_C1_SYM>(arr,1,false,new PHF_C1_CHECK1());
		(( alo::tabuStatus<PHF_C1_CHECK1,PHF_C1_SYM>*) statuses[3])->memorizeStrength();

		count = 4;
	} else {
		statuses = new tabu::tabuStatus<PHF_C1_SYM>* [3];
		count = 3;		
	}

	statuses[0] = new alo::tabuStatus<PHF_C1_CHECK2_1,PHF_C1_SYM>(arr,2,false,new PHF_C1_CHECK2_1());
	(( alo::tabuStatus<PHF_C1_CHECK1,PHF_C1_SYM>*) statuses[0])->memorizeStrength();

	statuses[1] = new alo::tabuStatus<PHF_C1_CHECK2_2,PHF_C1_SYM>(arr,2,false,new PHF_C1_CHECK2_2());
	(( alo::tabuStatus<PHF_C1_CHECK1,PHF_C1_SYM>*) statuses[1])->memorizeStrength();

	statuses[2] = new alo::tabuStatus<PHF_C1_CHECK3,PHF_C1_SYM>(arr,3,false,new PHF_C1_CHECK3());
	(( alo::tabuStatus<PHF_C1_CHECK1,PHF_C1_SYM>*) statuses[2])->memorizeStrength();	
	
	multicheck::multicheck_tabuSearcher<PHF_C1_SYM> ts( arr,rows,cols,syms,count,statuses,stateFile,(_access(stateFile, 0) != -1) && !startOver);
	ts.search();
}*/

void doTabuPCA(int rows, int cols, int syms ) {
	printf("Searching for strength 3 PCA.\n");
	// STRENGTH 3 ONLY

	SYM ** searches;

	searches = new SYM*[ syms * ( syms - 1 ) * 3 ];
	int c = 0;

	int i, j;

	for ( i = 0; i < syms; i++ ) {
		for ( j = 0; j < syms; j++ ) {
			if ( j == i ) continue;

			searches[c] = new SYM[ 3 ];
			searches[c][0] = i;
			searches[c][1] = i;
			searches[c][2] = j;

			c++;

			searches[c] = new SYM[ 3 ];
			searches[c][0] = i;
			searches[c][1] = j;
			searches[c][2] = i;

			c++;

			searches[c] = new SYM[ 3 ];
			searches[c][0] = j;
			searches[c][1] = i;
			searches[c][2] = i;

			c++;
		}
	}

	char stateFile[50];
	sprintf(stateFile, "inprogress/%d-%d-%d.pca.state", rows, cols, syms);

	srand( (unsigned)time( NULL ) );
	
	alo_ofeach::alo_ofeach_tabuSearcher<SYM> ts(&arrays::int_SymbolGenerator(syms),rows,cols,syms,3,searches,c,stateFile,(_access(stateFile, 0) != -1) && !startOver);
	ts.search();
}

/*void doTabuQCPHF(int rows, int cols, int syms, int copies ) {
	qca::qcaTupleParams p;
	p.k = cols;
	p.l = copies;

	cols = cols * copies;

	char stateFile[50];
	sprintf(stateFile, "inprogress/%d-%d-%d-%d.cphf.state", rows, cols, syms, copies);

	srand( (unsigned)time( NULL ) );

	bool found = false;
	arrays::Array<C_SYM> result;	

	bool useFile = (_access(stateFile, 0) != -1) && !startOver;

	if ( syms >= 5 ) {
		if ( util::ipow( syms, 6 ) <= 5000000 ) {
			printf("Using medium memory system.\n");

			ROW_CHECK_MS_CPHF checker( 4, syms );

			alo::alo_tabuSearcher<C_SYM,ROW_CHECK_MS_CPHF,qca::qcatuple> ts( &cphf::cphf_SymbolGenerator(3,syms),rows,cols,syms,4,stateFile,useFile,&checker,p);
			ts.search();

			if ( ts.getBestScore() == 0 ) {
				found = true;
				ts.getBest().copyInto( result );
			}
		} else {
			printf("Using lowest memory system.\n");

			ROW_CHECK_LS_CPHF checker( 4, syms );

			alo::alo_tabuSearcher<C_SYM,ROW_CHECK_LS_CPHF,qca::qcatuple> ts(&cphf::cphf_SymbolGenerator(3,syms),rows,cols,syms,4,stateFile,useFile,&checker,p);
			ts.search();

			if ( ts.getBestScore() == 0 ) {
				found = true;
				ts.getBest().copyInto( result );
			}
		}

	} else {
		printf("Using highest memory system.\n");

		ROW_CHECK_CPHF checker( 4, syms );
	
		alo::alo_tabuSearcher<C_SYM,ROW_CHECK_CPHF,qca::qcatuple> ts(&cphf::cphf_SymbolGenerator(3,syms),rows,cols,syms,4,stateFile,useFile,&checker,p);
		ts.search();

		if ( ts.getBestScore() == 0 ) {
			found = true;
			ts.getBest().copyInto( result );
		}
	}

	/*if ( found ) {
		char filename[255];
		sprintf(filename, "%d-%d-%d-%d.qcphf", rows, cols, syms, copies);
		FILE * out = fopen(filename, "wt");
		result.fprint( out, syms, strength );
		fclose(out);
	}* /

}*/

/*void doTabuQCA(int rows, int cols, int syms, int copies ) {
	SYM ** searches;

	qca::qcaTupleParams p;
	p.k = cols;
	p.l = copies;

	cols = cols * copies;

	if ( syms != 2 ) {
		printf("Only v = 2 supported right now.\n");
		return;
	}

	/*qca::qcatuple tuple(p);
	do {
		tuple.print();
		printf("\n");
	} while ( ++tuple );

	return;* /


	searches = new SYM*[ 8 ];

	searches[0] = new SYM[ 4 ];
	searches[0][0] = 1;
	searches[0][1] = 0;
	searches[0][2] = 0;
	searches[0][3] = 0;

	searches[1] = new SYM[ 4 ];
	searches[1][0] = 0;
	searches[1][1] = 1;
	searches[1][2] = 0;
	searches[1][3] = 0;

	searches[2] = new SYM[ 4 ];
	searches[2][0] = 0;
	searches[2][1] = 0;
	searches[2][2] = 1;
	searches[2][3] = 0;

	searches[3] = new SYM[ 4 ];
	searches[3][0] = 0;
	searches[3][1] = 0;
	searches[3][2] = 0;
	searches[3][3] = 1;

	searches[4] = new SYM[ 4 ];
	searches[4][0] = 0;
	searches[4][1] = 1;
	searches[4][2] = 1;
	searches[4][3] = 1;

	searches[5] = new SYM[ 4 ];
	searches[5][0] = 1;
	searches[5][1] = 0;
	searches[5][2] = 1;
	searches[5][3] = 1;

	searches[6] = new SYM[ 4 ];
	searches[6][0] = 1;
	searches[6][1] = 1;
	searches[6][2] = 0;
	searches[6][3] = 1;

	searches[7] = new SYM[ 4 ];
	searches[7][0] = 1;
	searches[7][1] = 1;
	searches[7][2] = 1;
	searches[7][3] = 0;

	char stateFile[50];
	sprintf(stateFile, "inprogress/%d-%d-%d-%d.qca.state", rows, cols, copies, syms);

	srand( (unsigned)time( NULL ) );
	
	alo_ofeach::alo_ofeach_tabuSearcher<SYM,qca::qcatuple> ts(&arrays::int_SymbolGenerator(syms),rows,cols,syms,4,searches,8,stateFile,(_access(stateFile, 0) != -1) && !startOver,p);
	ts.search();
}*/

void doTabuCA(int rows, int cols, int syms, int strength ) {
	SYM ** searches;

	int count = util::ipow(syms, strength);

	searches = new SYM*[count];

	int c = 0;
	int i;
	cphf::iterable_utuple u(syms, strength);

	do {
		searches[c] = new SYM[strength];

		for ( i = 0; i < strength; i++ ) {
			searches[c][i] = u[i];
		}

		c++;
	} while ( u++ );

	printf("Listed %d searches - should be %d\n", c, count );

	char stateFile[50];
	sprintf(stateFile, "inprogress/%d-%d-%d.pca.state", rows, cols, syms);

	srand( (unsigned)time( NULL ) );
	
	alo_ofeach::alo_ofeach_tabuSearcher<SYM> ts(&arrays::int_SymbolGenerator(syms),rows,cols,syms,3,searches,c,stateFile,(_access(stateFile, 0) != -1) && !startOver);
	ts.search();
}

void doTabuSums(int rows, int cols, int syms, int strength, int target) {
	char stateFile[50];
	sprintf(stateFile, "inprogress/%d-%d-%d-%d-%d.sums.state", rows, cols, syms, strength, target);

	srand( (unsigned)time( NULL ) );

	ROW_CHECK_SUMS checker( target );
	
	alo::alo_tabuSearcher<SYM,ROW_CHECK_SUMS> ts(&arrays::int_SymbolGenerator(syms),rows,cols,syms,strength,stateFile,(_access(stateFile, 0) != -1) && !startOver, &checker);
	ts.search();
}

/*void doTabuCPHF(int rows, int cols, int syms, int strength) {
	char stateFile[50];
	sprintf(stateFile, "inprogress/%d-%d-%d-%d.cphf.state", rows, cols, syms, strength);

	srand( (unsigned)time( NULL ) );

	bool found = false;
	arrays::Array<C_SYM> result;	

	bool useFile = (_access(stateFile, 0) != -1) && !startOver;

	if ( strength > 4 || ( strength == 4 && syms >= 5 ) ) {
		if ( util::ipow( syms, (strength-1)*2 ) <= 5000000 ) {
			printf("Using medium memory system.\n");

			ROW_CHECK_MS_CPHF checker( strength, syms );

			alo::alo_tabuSearcher<C_SYM,ROW_CHECK_MS_CPHF> ts( &cphf::cphf_SymbolGenerator(strength-1,syms),rows,cols,syms,strength,stateFile,useFile,&checker);
			ts.search();

			if ( ts.getBestScore() == 0 ) {
				found = true;
				ts.getBest().copyInto( result );
			}
		} else {
			printf("Using lowest memory system.\n");

			ROW_CHECK_LS_CPHF checker( strength, syms );

			alo::alo_tabuSearcher<C_SYM,ROW_CHECK_LS_CPHF> ts(&cphf::cphf_SymbolGenerator(strength-1,syms),rows,cols,syms,strength,stateFile,useFile,&checker);
			ts.search();

			if ( ts.getBestScore() == 0 ) {
				found = true;
				ts.getBest().copyInto( result );
			}
		}

	} else {
		printf("Using highest memory system.\n");

		ROW_CHECK_CPHF checker( strength, syms );
	
		alo::alo_tabuSearcher<C_SYM,ROW_CHECK_CPHF> ts(&cphf::cphf_SymbolGenerator(strength-1,syms),rows,cols,syms,strength,stateFile,useFile,&checker);
		ts.search();

		if ( ts.getBestScore() == 0 ) {
			found = true;
			ts.getBest().copyInto( result );
		}
	}

	if ( found ) {
		char filename[255];
		sprintf(filename, "%d-%d-%d-%d.cphf", rows, cols, syms, strength);
		FILE * out = fopen(filename, "wt");
		result.fprint( out, syms, strength );
		fclose(out);
	}
}*/

void doTabuWords( int rows, int cols, int strength, const char * filename ) {
	char stateFile[50];
	sprintf(stateFile, "inprogress/%d-%d-%d.words.state", rows, cols, strength);

	srand( (unsigned)time( NULL ) );

	ROW_CHECK_WORDS checker( filename, strength );
	
	alo::alo_tabuSearcher<L_SYM,ROW_CHECK_WORDS> ts(&words::letter_SymbolGenerator(),rows,cols,26,strength,stateFile,(_access(stateFile, 0) != -1) && !startOver, &checker);
	ts.search();
}

void main(int argc, char* argv[]) {
	GetPot cl(argc, argv);

	if ( argc == 1 || cl.search("--help") ) {
		printf("Options include: \n");
		printf("\t--phf");
		printf("\t--word");
		printf("\t--sum");
		return;
	}

	int rows, cols, syms, strength;
	int sumTarget;
	const char * wordListFilename;

	int skip = 0;

	if ( cl.search("--phf") ) {
		mode = MODE_PHF;
	} else if ( cl.search("--pca") ) {
		mode = MODE_PCA;
	} else if ( cl.search("--ca") ) {
		mode = MODE_CA;
	} else if ( cl.search("--word") ) {
		mode = MODE_WORD;
		wordListFilename = cl.next("");
		skip++;
	} else if ( cl.search("--sum") ) {
		mode = MODE_SUM;
		sumTarget = cl.next(0);
		skip++;
	} else if ( cl.search("--qca") ) {
		mode = MODE_QCA;
	} else if ( cl.search("--qcphf") ) {
		mode = MODE_QCPHF;
	} else if ( cl.search("--phfc1") ) {
		mode = MODE_PHF_C1;
	}

	if ( cl.search("--movelimit") ) {
		moveLimiting = true;
		moveLimit = cl.next(0);
		skip++;
	}

	if ( cl.search("--timelimit") ) {
		timeLimiting = true;
		timeLimit = cl.next(0);
		skip++;
	}

	if ( cl.search("--fullsearch") || cl.search("--fs") ) {
		fullSearch = true;
	}

	if ( cl.search("--nooutput") ) {
		outputState = false;
	}

	if ( cl.search("--startover") ) {
		startOver = true;
	}

	if ( cl.search("--startwithbest") || cl.search("--swb") ) {
		startWithBest = true;
	}

	if ( cl.search("--tabusize") ) {
		tabuCount = cl.next(0);
		skip++;
	}

	if ( cl.search("--iterationlimit") ) {
		iterationLimiting = true;
		iterationLimit = cl.next(0);
		skip++;
	}

	int i;
	for ( i = 0; i < skip; i++ ) {
		cl.next_nominus();
	}

	rows = atoi( argv[ cl.next_nominus() ] );
	cols = atoi( argv[ cl.next_nominus() ] );

	if ( mode != MODE_WORD ) {
		syms = atoi( argv[ cl.next_nominus() ] );
	}

	if ( mode != MODE_PCA ) {
		strength = atoi( argv[ cl.next_nominus() ] );
	}

	switch ( mode ) {
//		case MODE_CPHF:
//			doTabuCPHF( rows, cols, syms, strength );
//			break;
		case MODE_PHF:
			doTabuPHF( rows, cols, syms, strength );
			break;
		case MODE_WORD:
			doTabuWords( rows, cols, strength, wordListFilename );
			break;
		case MODE_SUM:
			doTabuSums( rows, cols, syms, strength, sumTarget );
			break;
		case MODE_PCA:
			doTabuPCA( rows, cols, syms );
			break;
		case MODE_CA:
			doTabuCA( rows, cols, syms, strength );
			break;
//		case MODE_QCA:
//			doTabuQCA( rows, cols, syms, strength ); // strength = copies
//			break;
//		case MODE_QCPHF:
//			doTabuQCPHF( rows, cols, syms, strength ); // strength = copies
//			break;
//		case MODE_PHF_C1:
//			doTabuPHF_C1( rows, cols, syms, strength );
//			break;
		default:
			printf("Invalid mode: %d\n", mode);
			break;
	}
}