#include "bitboard.h"
#include "mvgen.h"
#include "utilities.h"
#include "state.h"


const double P_SCORE = 1;
const double RK_SCORE = 5.2;
const double BSHP_SCORE = 3.3;
const double KN_SCORE = 3.2;
const double Q_SCORE = 9.6;

int p_sq[] = {0,  0,  0,  0,  0,  0,  0,  0,
50, 50, 50, 50, 50, 50, 50, 50,
10, 10, 20, 30, 30, 20, 10, 10,
 5,  5, 10, 27, 25, 10,  5,  5,
 0,  0,  0, 24, 21,  0,  0,  0,
 5, -5,-10,  0,  0,-10, -5,  5,
 5, 10, 10,-20,-20, 10, 10,  5,
 0,  0,  0,  0,  0,  0,  0,  0};

int kn_sq[] = {-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  0,  0,  0,-20,-40,
-30,  0, 12, 15, 15, 11,  0,-30,
-30,  5, 16, 20, 22, 17,  5,-30,
-30,  0, 14, 23, 20, 15,  0,-30,
-30,  5, 10, 14, 16, 11,  5,-30,
-40,-20,  0,  5,  5,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50};

int b_sq[] = {-20,-10,-10,-10,-10,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  5,  0,  0,  0,  0,  5,-10,
-20,-10,-10,-10,-10,-10,-10,-20};

int r_sq[] = {  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0};

int q_sq[] = {-20,-10,-10, -5, -5,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5,  5,  5,  5,  0,-10,
 -5,  0,  5,  5,  5,  5,  0, -5,
  0,  0,  5,  5,  5,  5,  0, -5,
-10,  5,  5,  5,  5,  5,  0,-10,
-10,  0,  5,  0,  0,  0,  0,-10,
-20,-10,-10, -5, -5,-10,-10,-20};

int k_sq[] = {-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-20,-30,-30,-40,-40,-30,-30,-20,
-10,-20,-20,-20,-20,-20,-20,-10,
 20, 20,  0,  0,  0,  0, 20, 20,
 20, 30, 10,  0,  0, 10, 30, 20};

double material_score(bool is_white) {
	U64 *p = bb_lookup(is_white, P);
	U64 *rk = bb_lookup(is_white, R);
	U64 *bshp = bb_lookup(is_white, B);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *q = bb_lookup(is_white, Q);

	U64 mask = 1;
	double score = 0;
	
	for (int i = 0; i < 64; i++) {
		if ((*p >> i) & mask) {
			score += P_SCORE;
		}
		if ((*rk >> i) & mask) {
			score += RK_SCORE;
		}
		if ((*bshp >> i) & mask) {
			score += BSHP_SCORE;
		}
		if ((*kn >> i) & mask) {
			score += KN_SCORE;
		}
		if ((*q >> i) & mask) {
			score += Q_SCORE;
		}
	}
	return score;
}

double center_score(bool is_white) {
	U64 *p = bb_lookup(is_white, P);
	U64 *rk = bb_lookup(is_white, R);
	U64 *bshp = bb_lookup(is_white, B);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *q = bb_lookup(is_white, Q);
	U64 *k = bb_lookup(is_white, K);

	U64 center = 0x0000001818000000;
	U64 mask = 1;
	Vector *v = (Vector *) malloc(sizeof(Vector));
	init_vector(v);
	v = gen_all_moves(*p, is_white, P, v);
	v = gen_all_moves(*rk, is_white, R, v);
	v = gen_all_moves(*bshp, is_white, B, v);
	v = gen_all_moves(*kn, is_white, Kn, v);
	v = gen_all_moves(*q, is_white, Q, v);
	v = gen_all_moves(*k, is_white, K, v);
	size_t score = 0;
	if (is_white) {
		for (int i = 0; i < 64; i++) {
			U64 piece = (*p & (mask << i));
			if (piece) {
				if (i % 8 == 0) {
					score += __builtin_popcountl((*p << 7) & center);
				} else if (i % 8 == 7) {
					score += __builtin_popcountl((*p << 9) & center);
				} else {
					score += __builtin_popcountl((*p << 7) & center);
					score += __builtin_popcountl((*p << 9) & center);
				}
			}
		}
	} else {
		for (int i = 0; i < 64; i++) {
			U64 piece = (*p & (mask << i));
			if (piece) {
				if (i % 8 == 0) {
					score += __builtin_popcountl((*p >> 9) & center);
				} else if (i % 8 == 7) {
					score += __builtin_popcountl((*p >> 7) & center);
				} else {
					score += __builtin_popcountl((*p >> 9) & center);
					score += __builtin_popcountl((*p >> 7) & center);
				}
			}
		}
	}
	for (int i = 0; i < (v->size); i++) {
		U64 bb = v->elements[i];
		score += __builtin_popcountl(bb & center);
	}
	clean_vector(v);
	free(v);
	return (double) score;
}

double total_score(bool is_white) {
	U64 mask = 1;
	double endgame = -20000.0;
	double total_score = 2 * material_score(is_white) + 0.1 * center_score(is_white);
	U64 *p = bb_lookup(is_white, P);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *b = bb_lookup(is_white, B);
	U64 *rk = bb_lookup(is_white, R);
	U64 *q = bb_lookup(is_white, Q);
	U64 *k = bb_lookup(is_white, K);

	double bonus = 0.0;
	for (int i = 0; i < 64; i++) {
		int idx = is_white ? 63 - i : i;
		U64 piece = (*p & (mask << i));
		if (piece) {
			bonus = (double) p_sq[63 - i] / (double) 100;
			total_score += bonus;
		}
		piece = (*kn & (mask << i));
		if (piece) {
			bonus = (double) kn_sq[idx] / (double) 100;
			total_score += bonus;
		}
		piece = (*b & (mask << i));
		if (piece) {
			bonus = (double) b_sq[idx] / (double) 100;
			total_score += bonus;
		}
		piece = (*rk & (mask << i));
		if (piece) {
			bonus = (double) r_sq[idx] / (double) 100;
			total_score += bonus;
		}
		piece = (*q & (mask << i));
		if (piece) {
			bonus = (double) q_sq[idx] / (double) 100;
			total_score += bonus;
		}
		piece = (*k & (mask << i));
		if (piece) {
			bonus = (double) k_sq[idx] / (double) 100;
			total_score += bonus;
		}
	}
 	if (chk_mate(is_white)) {
		total_score += endgame;
	}
	return total_score;
}