#include "../tabu/arrays/array.h"

#include "../tabu/alo/tabuStatus.h"

#include "../tabu/cphf/cphfsymbol.h"
#include "../tabu/cphf/less_space_check.h"

#include "../tabu/math/galois_field.h"

#include <stdlib.h>

typedef cphf::cphf_Symbol C_SYM;
typedef cphf::cphf_less_space_Check<C_SYM> ROW_CHECK_LS_CPHF;

void main(int argc, char* argv[]) {
	int syms, strength;
	arrays::Array<C_SYM> input( argv[1], syms, strength );	
	input.gen = new cphf::cphf_SymbolGenerator( strength - 1, syms );

	ROW_CHECK_LS_CPHF checker( strength, syms );
	alo::tabuStatus<ROW_CHECK_LS_CPHF, C_SYM> ts( input, strength, true, &checker );

	ts.print();
}