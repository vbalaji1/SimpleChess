#include "pawn.h"
#include "bitboard.h"

U64 p_enpassant;

void promotion(int index, bool is_white, piece_t promoted_type) {
	U64 mask = 1;
	if (is_white) {
		U64 *promoted = bb_lookup(is_white, promoted_type);
		*promoted |= (mask << (index + 8));
		U64 *p = bb_lookup(is_white, P);
		*p &= ~(mask << index);
	} else {
		U64 *promoted = bb_lookup(is_white, promoted_type);
		*promoted |= (mask << (index - 8));
		U64 *p = bb_lookup(is_white, P);
		*p &= ~(mask << index);
	}
}

U64 gen_enpassant(U64 bb, bool is_white) {

}
//U64 gen_p_enpassant 