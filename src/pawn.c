#include "pawn.h"

U64 p_double;
U64 p_enpassant;

void update_pdouble(U64 bb, int index) {
	U64 mask = 1;
	if (index != -1) {
		p_double = (bb ^ (mask << (index - 1)));
	}
}

//U64 gen_p_enpassant 