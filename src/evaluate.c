#include "bitboard.h"
#include "mvgen.h"
#include "utilities.h"

const double P_SCORE = 1;
const double RK_SCORE = 5.2;
const double BSHP_SCORE = 3.3;
const double KN_SCORE = 3.2;
const double Q_SCORE = 9.6;

double material_score(bool is_white) {
	U64 *p = bb_lookup(!is_white, P);
	U64 *rk = bb_lookup(!is_white, R);
	U64 *bshp = bb_lookup(!is_white, B);
	U64 *kn = bb_lookup(!is_white, Kn);
	U64 *q = bb_lookup(!is_white, Q);

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

double center_control(bool is_white) {
	U64 *p = bb_lookup(is_white, P);
	U64 *rk = bb_lookup(is_white, R);
	U64 *bshp = bb_lookup(is_white, B);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *q = bb_lookup(is_white, Q);
	U64 *k = bb_lookup(is_white, K);

	U64 center = 0x0000001818000000;
	Vector *v = (Vector *) malloc(sizeof(Vector));
	init_vector(v);
	v = gen_all_moves(*p, is_white, P, v);
	v = gen_all_moves(*rk, is_white, R, v);
	v = gen_all_moves(*bshp, is_white, B, v);
	v = gen_all_moves(*kn, is_white, Kn, v);
	v = gen_all_moves(*q, is_white, Q, v);
	v = gen_all_moves(*k, is_white, K, v);

	U64 bb;
	size_t score = 0;
	for (int i = 0; i < (v->size); i++) {
		bb = v->elements[i];
		score += __builtin_popcountl(bb & center);
		printf("%d\n", score);
	}
	clean_vector(v);
	free(v);
	return (double) score;
}