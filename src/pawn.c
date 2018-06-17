#include "pawn.h"
#include "bitboard.h"
#include "mvgen.h"

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
	U64 mask = 1;
	int index = __builtin_ffsl(bb) - 1;
	int enp_index = __builtin_ffsl(p_enpassant) - 1;
	int diff = index - enp_index;
	if (is_white) {
		if (index % 8 == 0 && diff == -1) {
			if (validate_sq(bb << 9, true) == 2) {
				return bb << 9;
			}
		} else if (index % 8 == 7 && diff == 1) {
			if (validate_sq(bb << 7, true) == 2) {
				return bb << 7;
			}
		} else if (diff == 1) {
			if (validate_sq(bb << 7, true) == 2) {
				bb |= (bb << 7);
				bb &= ~(mask << index);
				return bb;
			}
		} else if (diff == -1) {
			if (validate_sq(bb << 9, true) == 2) {
				bb |= (bb << 9);
				bb &= ~(mask << index);
				return bb;
			}
		}
	} else {
		if (index % 8 == 0 && diff == -1) {
			if (validate_sq(bb >> 7, false) == 2) {
				return bb >> 7;
			}
		} else if (index % 8 == 7 && diff == 1) {
			if (validate_sq(bb >> 9, false) == 2) {
				return bb >> 9;
			}
		} else if (diff == 1) {
			if (validate_sq(bb >> 9, false) == 2) {
				bb |= (bb >> 9);
				bb &= ~(mask << index);
				return bb;
			}
		} else if (diff == -1) {
			if (validate_sq(bb >> 7, false) == 2) {
				bb |= (bb >> 7);
				bb &= ~(mask << index);
				return bb;
			}
		}
	}
	return ((U64) 0);
}
