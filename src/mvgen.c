#include "mvgen.h"
#include "bitboard.h"

U64 old;
U64 *capture_addr;

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

bool update_capture(U64 bb, bool is_white) {
	U64 *p = bb_lookup(!is_white, P);
	U64 *rk = bb_lookup(!is_white, R);
	U64 *bshp = bb_lookup(!is_white, B);
	U64 *kn = bb_lookup(!is_white, Kn);
	U64 *q = bb_lookup(!is_white, Q);
	
	U64 mask = 1;
	if (bb & *p) {
		for (int i = 0; i < 64; i++) {
			U64 sq = (bb & (mask << i));
			if (sq) {
				old = *p;
				*p &= ~(mask << i);
				capture_addr = p;
				return true;
			}
		}
	} else if (bb & *rk) {
		for (int i = 0; i < 64; i++) {
			U64 sq = (bb & (mask << i));
			if (sq) {
				old = *rk;
				*rk &= ~(mask << i);
				capture_addr = rk;
				return true;
			}
		}
	} else if (bb & *bshp) {
		for (int i = 0; i < 64; i++) {
			U64 sq = (bb & (mask << i));
			if (sq) {
				old = *bshp;
				*bshp &= ~(mask << i);
				capture_addr = bshp;
				return true;
			}
		}
	} else if (bb & *kn) {
		for (int i = 0; i < 64; i++) {
			U64 sq = (bb & (mask << i));
			if (sq) {
				old = *kn;
				*kn &= ~(mask << i);
				capture_addr = kn;
				return true;
			}
		}
	} else if (bb & *q) {
		for (int i = 0; i < 64; i++) {
			U64 sq = (bb & (mask << i));
			if (sq) {
				old = *q;
				*q &= ~(mask << i);
				capture_addr = q;
				return true;
			}
		}
	}
	return false;
}

U64 chk_self_legality(U64 move, int index, bool is_white, piece_t type) {
	U64 mask = 1;
	for (int i = 0; i < 64; i++) {
		U64 piece = (move & (mask << i));
		if (piece) {
			U64 *piece_bb = bb_lookup(is_white, type);
			U64 copy = *piece_bb;
			*piece_bb &= ~(mask << index);
			*piece_bb |= piece;
			bool captured = update_capture(piece, is_white);

			U64 p = *(bb_lookup(!is_white, P));
			U64 rk = *(bb_lookup(!is_white, R));
			U64 bshp = *(bb_lookup(!is_white, B));
			U64 kn = *(bb_lookup(!is_white, Kn));
			U64 q = *(bb_lookup(!is_white, Q));
			U64 k = *(bb_lookup(!is_white, K));

			U64 atks = 0;
			for (int j = 0; j < 64; j++) {
				U64 new_piece = (rk & (mask << j));
				if (new_piece) {
					atks = gen_mv_piece(new_piece, j, !is_white, R);
					if (atks & *(bb_lookup(is_white, K))) {
						move &= ~(mask << i);
						break;
					} 
				}
				new_piece = (bshp & (mask << j));
				if (new_piece) {
					atks = gen_mv_piece(new_piece, j, !is_white, B);
					if (atks & *(bb_lookup(is_white, K))) {
						move &= ~(mask << i);
						break;
					}
				}
				new_piece = (kn & (mask << j));
				if (new_piece) {
					atks = gen_mv_piece(new_piece, j, !is_white, Kn);
					if (atks & *(bb_lookup(is_white, K))) {
						move &= ~(mask << i);
						break;
					}
				}
				new_piece = (q & (mask << j));
				if (new_piece) {
					atks = gen_mv_piece(new_piece, j, !is_white, Q);
					if (atks & *(bb_lookup(is_white, K))) {
						move &= ~(mask << i);
						break;
					}
				}
				new_piece = (k & (mask << j));
				if (new_piece) {
					atks = gen_mv_piece(new_piece, j, !is_white, K);
					if (atks & *(bb_lookup(is_white, K))) {
						printf("PROBLEM:\n");
						print_bits(atks, true);
						move &= ~(mask << i);
						break;
					}
				}
				new_piece = (p & (mask << j));
				if (new_piece) {
					atks = gen_p_capture(new_piece, *(bb_lookup(is_white, K)), !is_white);
					if (atks & *(bb_lookup(is_white, K))) {
						move &= ~(mask << i);
						break;
					}
				}
			}
			*piece_bb = copy;
			if (captured) {
				*capture_addr = old;
			}
		}
	}
	return move;
}

Vector *gen_all_moves(U64 bb, bool is_white, piece_t type, Vector *v) {
	U64 mask = 1;
	for (int i = 0; i < 64; i++) {
		U64 piece = (bb & (mask << i));
		if (piece) {
			U64 move = gen_mv_piece(piece, i, is_white, type);
			if (type == Kn) {
				print_bits(move, true);
			}
			move = chk_self_legality(move, i, is_white, type);
			if (type == Kn) {
				print_bits(move, true);
			}
			add(v, move, i, type);
		}
	}
	return v;
}

Vector *gen_all_patks(U64 bb, bool is_white, Vector *v) {
	U64 mask = 1;
	for (int i = 0; i < 64; i++) {
		U64 piece = (bb & (mask << i));
		if (piece) {
			U64 move = gen_p_atks(piece, is_white);
			move = chk_self_legality(move, i, is_white, P);
			add(v, move, i, P);
		}
	}
	return v;
}