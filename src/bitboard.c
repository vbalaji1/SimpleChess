#include "bitboard.h"
#include "mvgen.h"
#include "utilities.h"

U64 *bb_lookup(bool is_white, piece_t type) {
	if (is_white) {
		switch (type) {
			case P:
				{
					return &wP;
				}
			case R:
				{
					return &wR;
				}
			case Kn:
				{
					return &wKn;
				}
			case B:
				{
					return &wB;
				}
			case Q:
				{
					return &wQ;
				}
			case K:
				{
					return &wK;
				}
		}
	} else {
		switch (type) {
			case P: 
				{
					return &bP;
				}
			case R:
				{
					return &bR;
				}
			case Kn:
				{
					return &bKn;
				}
			case B:
				{
					return &bB;
				}
			case Q:
				{
					return &bQ;
				}
			case K:
				{
					return &bK;
				}
		}
	}
}

bool can_castle(bool k_side, bool is_white) {
	if (is_white) {
		if (k_side) {
			return wk_castle;
		} else {
			return wq_castle;
		}
	} else {
		if (k_side) {
			return bk_castle;
		} else {
			return bq_castle;
		}
	}
}

U64 gen_rk(U64 bb, int index, bool is_white) {
	U64 mask = 1;
	int prev = index % 8;
	int next = 7 - prev;
	for (int i = 1; i <= prev; i++) {
		U64 sq = (mask << (index - i));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 0) {
			break;
		} else if (occupied == 1) {
			bb = sq | bb;
			break;
		} else if (occupied == 2) {
			bb = sq | bb;
		}
	}
	for (int i = 1; i <= next; i++) {
		U64 sq = (mask << (index + i));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 0) {
			break;
		} else if (occupied == 1) {
			bb = sq | bb;
			break;
		} else if (occupied == 2) {
			bb = sq | bb;
		}
	}

	int rank = index / ((U64) 8);
	prev = rank % 8;                  
	next = 7 - prev;
	for (int i = 1; i <= prev; i++) {
		U64 sq = (mask << (index - (i * 8)));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 0) {
			break;
		} else if (occupied == 1) {
			bb = sq | bb;
			break;
		} else if (occupied == 2) {
			bb = sq | bb;
		}
	}
	for (int i = 1; i <= next; i++) {
		U64 sq = (mask << (index + (i * 8)));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 0) {
			break;
		} else if (occupied == 1) {
			bb = sq | bb;
			break;
		} else if (occupied == 2) {
			bb = sq | bb;
		}
	}
	return bb;
}

U64 gen_bshp(U64 bb, int index, bool is_white) {
	U64 mask = 1;
	int rank = index / ((U64) 8);
	int bottom_dist = rank % 8;
	int top_dist = 7 - bottom_dist;
	int ls_dist = index % 8;
	int rs_dist = 7 - ls_dist;

	int sw = (top_dist < ls_dist) ? top_dist : ls_dist;
	int se = (top_dist < rs_dist) ? top_dist : rs_dist;
	int nw = (bottom_dist < ls_dist) ? bottom_dist : ls_dist;
	int ne = (bottom_dist < rs_dist) ? bottom_dist : rs_dist;

	for (int i = 1; i <= sw; i++) {
		U64 sq = (mask << (index + (i * 7)));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 0) {
			break;
		} else if (occupied == 1) {
			bb = sq | bb;
			break;
		} else if (occupied == 2) {
			bb = sq | bb;
		}

	}
	for (int i = 1; i <= se; i++) {
		U64 sq = (mask << (index + (i * 9)));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 0) {
			break;
		} else if (occupied == 1) {
			bb = sq | bb;
			break;
		} else if (occupied == 2) {
			bb = sq | bb;
		}
	}
	for (int i = 1; i <= nw; i++) {
		U64 sq = (mask << (index - (i * 9)));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 0) {
			break;
		} else if (occupied == 1) {
			bb = sq | bb;
			break;
		} else if (occupied == 2) {
			bb = sq | bb;
		}
	}
	for (int i = 1; i <= ne; i++) {
		U64 sq = (mask << (index - (i * 7)));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 0) {
			break;
		} else if (occupied == 1) {
			bb = sq | bb;
			break;
		} else if (occupied == 2) {
			bb = sq | bb;
		}
	}

	return bb;
}

U64 gen_qn(U64 bb, int index, bool is_white) {
	bb = gen_rk(bb, index, is_white);
	bb = gen_bshp(bb, index, is_white);
	return bb;
}

U64 gen_k(U64 bb, int index, bool is_white) {
	U64 mask = 1;
	U64 full = gen_qn(bb, index, is_white);
	U64 sq1 = (mask << (index - 1));
	U64 sq2 = (mask << (index + 1));
	U64 sq3 = (mask << (index - 8));
	U64 sq4 = (mask << (index + 8));
	U64 sq5 = (mask << (index - 7));
	U64 sq6 = (mask << (index + 7));
	U64 sq7 = (mask << (index - 9));
	U64 sq8 = (mask << (index + 9));
	U64 limit = sq1 | sq2 | sq3 | sq4 | sq5 | sq6 | sq7 | sq8 | bb;
	return (full & limit);
}

U64 gen_kn(U64 bb, int index, bool is_white) {
	U64 mask = 1;
	int rank = index / ((U64) 8);
	int bottom_dist = rank % 8;
	int top_dist = 7 - bottom_dist;
	int ls_dist = index % 8;
	int rs_dist = 7 - ls_dist;

	U64 bb1 = 0;
	U64 bb2 = 0;
	U64 bb3 = 0;
	U64 bb4 = 0;
	U64 bb5 = 0;
	U64 bb6 = 0;
	U64 bb7 = 0;
	U64 bb8 = 0;
	if (top_dist >= 2) {
		if (ls_dist > 0) {
			U64 sq = (bb << 15);
			if (validate_sq(sq, is_white) != 0) {
				bb1 = sq | bb;
			}   
		}
		if (rs_dist > 0) {
			U64 sq = (bb << 17);
			if (validate_sq(sq, is_white) != 0) {
				bb2 = sq | bb;
			}
		}
	}
	if (bottom_dist >= 2) {
		if (ls_dist > 0) {
			U64 sq = (bb >> 17);
			if (validate_sq(sq, is_white) != 0) {
				bb3 = sq | bb;
			}
		} 
		if (rs_dist > 0) {
			U64 sq = (bb >> 15);
			if (validate_sq(sq, is_white)!= 0) {
				bb4= sq | bb;
			}
		}
	}
	if (rs_dist >= 2) {
		if (bottom_dist > 0) {
			U64 sq = (bb >> 6);
			if (validate_sq(sq, is_white)!= 0) {
				bb5 = sq | bb;
			}
		}
		if (top_dist > 0) {
			U64 sq = (bb << 10);
			if (validate_sq(sq, is_white) != 0) {
				bb6 = sq | bb;
			}
		}
	}
	if (ls_dist >= 2) {
		if (bottom_dist > 0) {
			U64 sq = (bb >> 10);
			if (validate_sq(sq, is_white) != 0) {
				bb7 = sq | bb;
			}
		}
		if (top_dist > 0) {
			U64 sq = (bb << 6);
			if (validate_sq(sq, is_white) != 0) {
				bb8 = sq | bb;
			}
		}
	}
	bb = bb | bb1 | bb2 | bb3 | bb4 | bb5 | bb6 | bb7 | bb8;
	return bb;
}

U64 gen_p_noncapture(U64 bb, int index, bool eligible, bool is_white) {
	U64 mask = 1;
	if (is_white) {
		U64 sq = (mask << (index + 8));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 2) {
			bb = sq | bb;
		}
		sq = (mask << (index + 16));
		occupied = validate_sq(sq, is_white);
		if (occupied == 2) {
			bb = eligible ? (sq | bb) : bb;
		}
	} else {
		U64 sq = (mask << (index - 8));
		int occupied = validate_sq(sq, is_white);
		if (occupied == 2) {
			bb = sq | bb;
		}
		sq = (mask << (index - 16));
		occupied = validate_sq(sq, is_white);
		if (occupied == 2) {
			bb = eligible ? (sq | bb) : bb;
		}
	}
	return bb; 
}

U64 gen_p_capture(U64 bb, U64 opp, bool is_white) {
	int index = __builtin_ffsl(bb) - 1;
	if (is_white) {
		if (index % 8 == 0) {
			return ((bb << 9) & opp);
		} else if (index % 8 == 7) {
			return ((bb << 7) & opp);
		} else if ((validate_sq(bb << 7, true) != 0) && (validate_sq(bb << 9, true) != 0)) {
			return (((bb << 7) & opp) | ((bb << 9) & opp));
		}
	} else {
		if (index % 8 == 0) {
			return ((bb >> 7) & opp);
		} else if (index % 8 == 7) {
			return ((bb >> 9) & opp);
		} else if ((validate_sq(bb >> 7, false) != 0) && (validate_sq(bb >> 9, false) != 0)) {
			return (((bb >> 7) & opp) | ((bb >> 9) & opp));
		}
	}
}

U64 gen_p_atks(U64 bb, bool is_white) {
	int index = __builtin_ffsl(bb) - 1;
	if (is_white) {
		if (index % 8 == 0) {
			return (bb << 9);
		} else if (index % 8 == 7) {
			return (bb << 7);
		} else if ((validate_sq(bb << 7, true) != 0) && (validate_sq(bb << 9, true) != 0)) {
			return ((bb << 7) | (bb << 9));
		}
	} else {
		if (index % 8 == 0) {
			return (bb >> 7);
		} else if (index % 8 == 7) {
			return (bb >> 9);
		} else if ((validate_sq(bb >> 7, false) != 0) && (validate_sq(bb >> 9, false) != 0)) {
			return ((bb >> 7) | (bb >> 9));
		}
	}
}

Vector *gen_k_castle(bool is_white) {

	U64 *p = bb_lookup(!is_white, P);
	U64 *rk = bb_lookup(!is_white, R);
	U64 *bshp = bb_lookup(!is_white, B);
	U64 *kn = bb_lookup(!is_white, Kn);
	U64 *q = bb_lookup(!is_white, Q);
	U64 *k = bb_lookup(!is_white, K);

	U64 king = *(bb_lookup(is_white, K));
	U64 rook = *(bb_lookup(is_white, R));   

	U64 mask = 1;
	U64 atk = 0;

	Vector *v = (Vector *) malloc(sizeof(Vector));
	init_vector(v);

	if (validate_sq(king << 1, is_white) != 2) {
		add(v, ((U64) 0), -1, (piece_t) NULL);
		return v;
	}
	if (validate_sq(king << 2, is_white) != 2) {
		add(v, ((U64) 0), -1, (piece_t) NULL);
		return v;
	}

	for (int i = 0; i < 64; i++) {
		U64 piece = (*rk & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, R);
			if (((atk & (king << 1)) || (atk & (king << 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
		piece = (*bshp & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, B);
			if (((atk & (king << 1)) || (atk & (king << 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
		piece = (*kn & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, Kn);
			if (((atk & (king << 1)) || (atk & (king << 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
		piece = (*q & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, Q);
			if (((atk & (king << 1)) || (atk & (king << 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
		piece = (*k & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, K);
			if (((atk & (king << 1)) || (atk & (king << 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
	}
	if (is_white) {
		add(v, king << 2, 4, K);
	} else {
		add(v, king << 2, 60, K);
	}
	if (is_white) {
		rook &= ~(mask << 7);
		rook |= mask << 5;
		add(v, rook, 7, R);
	} else { 
		rook &= ~(mask << 63);
		rook |= mask << 61;
		add(v, rook, 63, R);
	}
	return v;
}

Vector *gen_q_castle(bool is_white) {

	U64 *p = bb_lookup(!is_white, P);
	U64 *rk = bb_lookup(!is_white, R);
	U64 *bshp = bb_lookup(!is_white, B);
	U64 *kn = bb_lookup(!is_white, Kn);
	U64 *q = bb_lookup(!is_white, Q);
	U64 *k = bb_lookup(!is_white, K);

	U64 king = *(bb_lookup(is_white, K));
	U64 rook = *(bb_lookup(is_white, R));

	U64 mask = 1;
	U64 atk = 0;

	Vector *v = (Vector *) malloc(sizeof(Vector));
	init_vector(v);

	if (validate_sq(king >> 1, is_white) != 2) {
		add(v, ((U64) 0), -1, (piece_t) NULL);
		return v;
	}
	if (validate_sq(king >> 2, is_white) != 2) {
		add(v, ((U64) 0), -1, (piece_t) NULL);
		return v; 
	}
	if (is_white) {
		if (validate_sq((rook & mask) << 1, is_white) != 2) {
			add(v, ((U64) 0), -1, (piece_t) NULL);
			return v;
		}
	} else {
		if (validate_sq((rook & (mask << 56)) << 1, is_white) != 2) {
			add(v, ((U64) 0), -1, (piece_t) NULL);
			return v;
		} 
	}
	
	for (int i = 0; i < 64; i++) {
		U64 piece = (*rk & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, R);
			if (((atk & (king >> 1)) || (atk & (king >> 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
		piece = (*bshp & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, B);
			if (((atk & (king >> 1)) || (atk & (king >> 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
		piece = (*kn & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, Kn);
			if (((atk & (king >> 1)) || (atk & (king >> 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
		piece = (*q & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, Q);
			if (((atk & (king >> 1)) || (atk & (king >> 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
		piece = (*k & (mask << i));
		if (piece) {
			atk = gen_mv_piece(piece, i, !is_white, K);
			if (((atk & (king >> 1)) || (atk & (king >> 2)))) {
				add(v, ((U64) 0), -1, (piece_t) NULL);
				return v;
			}
		}
	}
	if (is_white) {
		add(v, king >> 2, 4, K);
	} else {
		add(v, king >> 2, 60, K);
	}
	if (is_white) {
		rook &= ~(mask);
		rook |= mask << 3;
		add(v, rook, 0, R);
	} else {
		rook &= ~(mask << 56);
		rook |= mask << 59;
		add(v, rook, 56, R);
	}
	return v;
} 

U64 gen_mv_piece(U64 piece, int index, bool is_white, piece_t type) {
	U64 mask = 1;
	if (piece) {
		U64 final = 0;
		switch (type) {
			case K:
				{
					final = gen_k(piece, index, is_white) ^ piece;
					return final;
				}
			case Q:
				{
					final = gen_qn(piece, index, is_white) ^ piece;
					return final;
				}
			case Kn:
				{
					final = gen_kn(piece, index, is_white) ^ piece;
					return final;
				}
			case R:
				{
					final = gen_rk(piece, index, is_white) ^ piece;
					return final;
				}
			case B:
				{
					final = gen_bshp(piece, index, is_white) ^ piece;
					return final;
				}
			case P:
				{
					int rank = index / 8;
					bool eligible = (rank == 6 || rank == 1) ? true : false;
					final = gen_p_noncapture(piece, index, eligible, is_white);
					if (is_white) {
						final = final | gen_p_capture(piece, bK, is_white);
						final = final | gen_p_capture(piece, bR, is_white);
						final = final | gen_p_capture(piece, bQ, is_white);
						final = final | gen_p_capture(piece, bKn, is_white);
						final = final | gen_p_capture(piece, bB, is_white);
						final = final | gen_p_capture(piece, bP, is_white);
					} else {
						final = final | gen_p_capture(piece, wK, is_white);
						final = final | gen_p_capture(piece, wR, is_white);
						final = final | gen_p_capture(piece, wQ, is_white);
						final = final | gen_p_capture(piece, wKn, is_white);
						final = final | gen_p_capture(piece, wB, is_white);
						final = final | gen_p_capture(piece, wP, is_white);
					}
					final = final ^ piece;
					return final;
				}		
		}
	}
}



