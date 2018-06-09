#include "mvgen.h"
#include "bitboard.h"

int validate_sq(U64 bb, bool is_white) {
	if (is_white) {
		if ((bb & wP) || (bb & wR) || (bb & wK) || (bb & wKn) || (bb & wB) || (bb & wQ)) {
			return 0; 
		} else if ((bb & bP) || (bb & bR) || (bb & bK) || (bb & bKn) || (bb & bB) || (bb & bQ)) {
			return 1;
		} else {
			return 2;
		}
	} else {
		if ((bb & bP) || (bb & bR) || (bb & bK) || (bb & bKn) || (bb & bB) || (bb & bQ)) {
			return 0;
		} else if ((bb & wP) || (bb & wR) || (bb & wK) || (bb & wKn) || (bb & wB) || (bb & wQ)) {
			return 1;
		} else {
			return 2;
		}
	}
}

U64 chk_self_legality(U64 move, U64 old, int index, bool is_white) {
	U64 mask = 1;
	for (int i = 0; i < 64; i++) {
		U64 piece = (move & (mask << i));
		if (piece) {
			
		}
	}
}

void gen_all_moves(U64 bb, bool is_white, piece_t type) {
	U64 mask = 1;
	for (int i = 0; i < 64; i++) {
		U64 piece = (bb & (mask << i));
		if (piece) {
			U64 move = gen_mv_piece(piece, i, is_white, type);
			chk_self_legality(move, bb, i, is_white);
		}
	}
}