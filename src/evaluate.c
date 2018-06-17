#include "bitboard.h"
#include "mvgen.h"
#include "utilities.h"


const double P_SCORE = 1;
const double RK_SCORE = 5.2;
const double BSHP_SCORE = 3.3;
const double KN_SCORE = 3.2;
const double Q_SCORE = 9.6;

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

double mobility_score(bool is_white) {
	U64 *p = bb_lookup(is_white, P);
	U64 *rk = bb_lookup(is_white, R);
	U64 *bshp = bb_lookup(is_white, B);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *q = bb_lookup(is_white, Q);
	U64 *k = bb_lookup(is_white, K);

	U64 mask = 1;
	Vector *v = (Vector *) malloc(sizeof(Vector));
	init_vector(v);
	v = gen_all_patks(*p, is_white, v);
	v = gen_all_moves(*rk, is_white, R, v);
	v = gen_all_moves(*bshp, is_white, B, v);
	v = gen_all_moves(*kn, is_white, Kn, v);
	v = gen_all_moves(*q, is_white, Q, v);
	v = gen_all_moves(*k, is_white, K, v);
	size_t score = 0;
	for (int i = 0; i < (v->size); i++) {
		U64 bb = v->elements[i];
		score += __builtin_popcountl(bb) - 1;
	}

	U64 *p1 = bb_lookup(!is_white, P);
	U64 *rk1 = bb_lookup(!is_white, R);
	U64 *bshp1 = bb_lookup(!is_white, B);
	U64 *kn1 = bb_lookup(!is_white, Kn);
	U64 *q1 = bb_lookup(!is_white, Q);
	U64 *k1 = bb_lookup(!is_white, K);
	Vector *v1 = (Vector *) malloc(sizeof(Vector));
	init_vector(v1);

	v1 = gen_all_patks(*p1, !is_white, v1);
	v1 = gen_all_moves(*rk1, !is_white, R, v1);
	v1 = gen_all_moves(*bshp1, !is_white, B, v1);
	v1 = gen_all_moves(*kn1, !is_white, Kn, v1);
	v1 = gen_all_moves(*q1, !is_white, Q, v1);
	v1 = gen_all_moves(*k1, !is_white, K, v1);
	size_t opp_score = 0;
	for (int i = 0; i < (v1->size); i++) {
		U64 bb1 = v1->elements[i];
		opp_score += __builtin_popcountl(bb1) - 1;
	}

	double entropy = (double) score / (double) opp_score;
	size_t sq_cntrl = 0;
	size_t opp_sq_cntrl = 0;
	U64 sq = 1;
	for (int i = 0; i < 64; i++) {
		size_t local = 0;
		size_t opp_local = 0;
		for (int j = 0; j < (v->size); j++) {
			U64 atk = v->elements[j];
			if (atk & sq) {
				local++;
			}
		}
		for (int j = 0; j < (v1->size); j++) {
			U64 atk1 = v1->elements[j];
			if (atk1 & sq) {
				opp_local++;
			}
		}
		if (local > opp_local) {
			sq_cntrl++;
		} else if (opp_local > local) {
			opp_sq_cntrl++;
		} 
		sq <<= 1;
	}
	double sq_ratio = (double) sq_cntrl / (double) opp_sq_cntrl;
	entropy *= sq_ratio;
	entropy = log(entropy);
	clean_vector(v);
	clean_vector(v1);
	free(v);
	free(v1);
	return entropy;
}

double total_score(bool is_white) {
	return material_score(is_white) + 0.1 * center_score(is_white) + 0.5 * mobility_score(is_white);
}