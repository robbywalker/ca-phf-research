#include <map>

//typedef std::vector<int> intArray;
typedef std::map<int,int> intArray;

intArray cur, last;

void main(void) {
	int iteration = 0;

	intArray::const_iterator it;

	cur[1] = 1;	

	do {
		for ( it = cur.begin(); it != cur.end(); it++ ) {
			last[it->first] = it->second;
		}

		for ( it = last.begin(); it != last.end(); it++ ) {
			if ( it->second != 0 ) {
				cur[ it->first ]++;				
				cur[ it->second ]++;
			}			
		}

		if ( (iteration % 100) == 0 ) {
			printf("%d\n", iteration);
		}

	} while ( ++iteration < 1000 );

	for ( it = cur.begin(); it != cur.end(); it++ ) {
		if ( it->second != 0 ) {
			printf("<%d %d>, ", it->second, it->first);
		}
	}
}