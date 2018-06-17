#include "utilities.h"
#include "bitboard.h"
#include "mvgen.h"
#include "pawn.h"

bool chk_mate(bool is_white) {
	U64 *p = bb_lookup(is_white, P);
	U64 *rk = bb_lookup(is_white, R);
	U64 *bshp = bb_lookup(is_white, B);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *q = bb_lookup(is_white, Q);
	U64 *k = bb_lookup(is_white, K);

	Vector *v = (Vector *) malloc(sizeof(Vector));
	init_vector(v);
	v = gen_all_moves(*p, is_white, P, v);
	v = gen_all_moves(*rk, is_white, R, v);
	v = gen_all_moves(*bshp, is_white, B, v);
	v = gen_all_moves(*kn, is_white, Kn, v);
	v = gen_all_moves(*q, is_white, Q, v);
	v = gen_all_moves(*k, is_white, K, v);
	for (size_t i = 0; i < (v->size); i++) {
		if ((v->elements[i]) != 0) {
			clean_vector(v);
			free(v);
			return false;
		}
	}
	clean_vector(v);
	free(v);
	return true;
}

void chk_pcapture(U64 mv, int dest_index, bool is_white, bool enp) {

}

void mk_move(U64 mv, int index, bool is_white, piece_t piece) {
	U64 *update = bb_lookup(is_white, piece);

	p_enpassant = 0;
	U64 *p = bb_lookup(!is_white, P);
	U64 *rk = bb_lookup(!is_white, R);
	U64 *bshp = bb_lookup(!is_white, B);
	U64 *kn = bb_lookup(!is_white, Kn);
	U64 *q = bb_lookup(!is_white, Q);
	U64 *k = bb_lookup(!is_white, K);

	U64 mask = 1;
	int dest_index = 0;
	for (int i = 0; i < 64; i++) {
		if (mv & (mask << i)) {
			dest_index = i;
		}
	}

	*update &= ~(mask << index);
	*update |= mv;

	int diff = dest_index - index;
	if (piece == P && (diff = 16 || diff == -16)) {
		p_enpassant |= mv;
	}

	if (piece == K && is_white) {
		wk_castle = false;
	} else if (piece == K && (!is_white)) {
		bk_castle = false;
	} else if (piece == R && is_white && index == 0) {
		wq_castle = false;
	} else if (piece == R && is_white && index == 7) {
		wk_castle = false;
	} else if (piece == R && (!is_white) && index == 63) {
		bk_castle = false;
	} else if (piece == R && (!is_white) && index == 56) {
		bq_castle = false;
	}

	if (piece != P) {
		if (*p & mv) {
			*p &= ~(mask << dest_index);
		} else if (*rk & mv) {
			*rk &= ~(mask << dest_index);
		} else if (*bshp & mv) {
			*bshp &= ~(mask << dest_index);
		} else if (*kn & mv) {
			*kn &= ~(mask << dest_index);
		} else if (*q & mv) {
			*q &= ~(mask << dest_index);
		}
	} else {
		chk_pcapture(mv, dest_index, is_white);
	}

}

void mk_castle(Vector *v, bool k_side, bool is_white) {
	U64 bb;
	U64 mask = 1;
	U64 *k = bb_lookup(is_white, K);
	U64 *rk = bb_lookup(is_white, R); 
	*k = v->elements[0];
	if (is_white) {
		if (k_side) {
			*rk &= ~(mask << 7);
		} else {
			*rk &= ~(mask); 
		}
	} else {
		if (k_side) {
			*rk &= ~(mask << 63);
		} else {
			*rk &= ~(mask << 56);
		}
	}
	*rk |= v->elements[1];
}

int sq_to_index(char file, char rank) {
	int row = rank - '0';
	row = 8 * (row - 1);
	if (file == 'a') {
		row += 0;
	} else if (file == 'b') {
		row += 1;
	} else if (file == 'c') {
		row += 2;
	} else if (file == 'd') {
		row += 3;
	} else if (file == 'e') {
		row += 4;
	} else if (file == 'f') {
		row += 5;
	} else if (file == 'g') {
		row += 6;
	} else if (file  == 'h') {
		row += 7;
	}
	return row;
}

void input_move(char *mv, bool is_white) {
	Vector *v = (Vector *) malloc(sizeof(Vector));
	init_vector(v);
 	if (strcmp(mv, "O-O") == 0) {
		if (can_castle(true, is_white)) {
			v = gen_k_castle(is_white);
			if (v->elements[0] != 0) {
				mk_castle(v, true, is_white);
			}
			wk_castle = is_white ? false : wk_castle;
			bk_castle = is_white ? bk_castle : false;
			clean_vector(v);
			free(v);
			return;
		}
	} else if (strcmp(mv, "O-O-O") == 0) {
		if (can_castle(false, is_white)) {
			v = gen_q_castle(is_white);
			if (v->elements[0] != 0) {
				mk_castle(v, false, is_white);
			}
			wq_castle = is_white ? false : wq_castle;
			bq_castle = is_white ? bq_castle : false;
			clean_vector(v);
			free(v);
			return;
		} 
	}
	size_t len = strlen(mv);
	int index;
	piece_t piece;
	U64 mask = 1;
	if (len == 2) {
		char first = mv[0];
		char second = mv[1];
		index = sq_to_index(first, second);
		piece = P;
	} else if (len == 3) {
		char first = mv[0];
		char second = mv[1];
		char third = mv[2];
		index = sq_to_index(second, third);
		if (first == 'Q') {
			piece = Q;
		} else if (first == 'B') {
			piece = B;
		} else if (first == 'N') {
			piece = Kn;
		} else if (first == 'R') {
			piece = R;
		} else if (first == 'K') {
			piece = K;
		}
	}
	U64 dest = 0;
	dest |= mask << index;
	U64 *piece_to_mv = bb_lookup(is_white, piece);
	v = gen_all_moves(*piece_to_mv, is_white, piece, v);
	U64 bb;
	U64 move;
	for (int i = 0; i < (v->size); i++) {
		bb = v->elements[i];
		move = (bb >> index) & mask;
		if (move) {
			mk_move(dest, v->origin[i], is_white, piece);
			clean_vector(v);
			free(v);
			return;
		}
	}
}