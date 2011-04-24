#include "../tabu/arrays/array.h"

#include "../tabu/cphf/cphfsymbol.h"
#include "../tabu/cphf/check.h"

#include "../tabu/util/util.h"

#include "../tabu/math/galois_field.h"

#include <stdlib.h>
#include <time.h>

typedef cphf::cphf_Symbol C_SYM;
typedef cphf::cphf_Check<C_SYM> ROW_CHECK_CPHF;


void main(int argc, char* argv[]) {
	int syms, strength;
	arrays::Array<C_SYM> input( argv[1], syms, strength );

	int i, j, k, l;
	int max = util::ipow( syms, strength );

	int newRowCount = input.getHeight() * ( max - syms ) + syms;

	printf("%d %d %d %d\n", newRowCount, input.getWidth(), syms, strength);

	for ( i = 0; i < syms; i++ ) {
		for ( j = 0; j < input.getWidth(); j++ ) {
			printf("%d ", i);
		}
		printf("\n");
	}
	printf("\n");

	for ( i = 0; i < input.getHeight(); i++ ) {
		for ( k = syms; k < max; k++ ) {
			for ( j = 0; j < input.getWidth(); j++ ) {

				int symval = input.get(i,j).asInt();
				int ktemp = k;

				math::GFNum sum(syms);
				sum = ktemp % syms;
				ktemp /= syms;


				for ( l = 1; l < strength; l++ ) {
					sum = sum + math::GFNum(( ktemp % syms ),syms) * math::GFNum(( symval % syms ),syms);

					ktemp /= syms;
					symval /= syms;					
				}

				printf("%d ", sum);

			}	
			printf("\n");
		}
		printf("\n");
	}
}