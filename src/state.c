#include "utilities.h"
#include "bitboard.h"
#include "mvgen.h"

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