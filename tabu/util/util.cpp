#include "util.h"
#include <stdlib.h>

namespace util {

	// return a random number between 0 and max - not including max
	// does this with equal probability 
	int random( int max ) {
		int div = (int) ((double)RAND_MAX) / max;

		int newMax = div * max;

		int v = 0;
		do {
			v = rand();
		} while ( v >= newMax );

		return ( v / div );
	}

	// weighted random number
	int weightedRandom( int * weights, int total ) {
		int k = random( total );

		int i = 0;
		while ( k >= weights[i] ) {
			k -= weights[i];
			i++;
		}

		return i;
	}

	// integer power
	int ipow( int base, int exp ) {
		if ( exp == 0 ) return 1;
		int cur = base;
		for ( int i = 0; i < exp - 1; i++ ) {
			cur *= base;
		}
		return cur;
	}

	long lpow( long base, int exp ) {
		if ( exp == 0 ) return 1;
		long cur = base;
		for ( int i = 0; i < exp - 1; i++ ) {
			cur *= base;
		}
		return cur;
	}

	long factorial( int n ) {
		long ret = 1;
		for ( int i = 2; i <= n; i++ ) {
			ret *= i;
		}
		return ret;		
	}

	long choose( int n, int c ) {
		return factorial(n) / ( factorial(c) * factorial(n-c) );
	}
};