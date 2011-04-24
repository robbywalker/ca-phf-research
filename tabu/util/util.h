#pragma once

namespace util {

	int random(int max);
	int weightedRandom( int * weights, int total );

	int ipow( int base, int exp );
	long lpow( long base, int exp );

	long factorial( int n );
	long choose( int n, int c );
};