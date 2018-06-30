#include "utilities.h"
#include "bitboard.h"
#include "pawn.h"

#define INDEX(is_white, piece) (is_white ? piece : piece + 6)

void print_bits(U64 n, bool gridwise) {
	U64 mask = 1;
	for (U64 i = 0; i < 64; i++) {
		if (gridwise && (i % 8 == 0)) {
			printf("\n");
		}
		(n & mask) ? printf("1 ") : printf("0 ");
		n >>= 1;
	}
    printf("\n");
}

void init_vector(Vector *v) {
	v->elements = (U64 *) malloc(sizeof(U64) * 10);
	v->size = 0;
	v->max_capacity = 10;
	v->origin = (int *) malloc(sizeof(int) * 10);
	v->piece = (piece_t *) malloc(sizeof(piece_t) * 10);
}

void add(Vector *v, U64 element, int origin, piece_t type) {
	if (v->size == v->max_capacity) {
		v->max_capacity *= 2;
		v->elements = (U64 *) realloc(v->elements, v->max_capacity * sizeof(U64));
		v->origin = (int *) realloc(v->origin, v->max_capacity * sizeof(int));
		v->piece = (piece_t *) realloc(v->piece, v->max_capacity * sizeof(piece_t));
	} 
	v->elements[v->size] = element;
	v->origin[v->size] = origin;
	v->piece[v->size++] = type;
}

void clean_vector(Vector *v) {
	free(v->elements);
	v->elements = NULL;
	v->size = 0;
	v->max_capacity = 0;
	free(v->origin);
	v->origin = NULL;
	free(v->piece);
	v->piece = NULL;
}

/*U64 zobrist_hash(bool is_white) {
	U64 mask = 1;
	U64 p = *(bb_lookup(is_white, P));
	U64 rk = *(bb_lookup(is_white, R));
	U64 bshp = *(bb_lookup(is_white, B));
	U64 kn = *(bb_lookup(is_white, Kn));
	U64 q = *(bb_lookup(is_white, Q));
	U64 k = *(bb_lookup(is_white, K));
	U64 hash = 0;

	for (int i = 0; i < 64; i++) {
		U64 bit = (p & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, P) * 64 + i];
		}
		bit = (rk & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, R) * 64 + i];
		}
		bit = (bshp & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, B) * 64 + i];
		}
		bit = (kn & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, K) * 64 + i];
		}
		bit = (q & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, Q) * 64 + i];
		}
		bit = (k & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, K) * 64 + i];
		}
	}

	p = *(bb_lookup(!is_white, P));
	rk = *(bb_lookup(!is_white, R));
	bshp = *(bb_lookup(!is_white, B));
	kn = *(bb_lookup(!is_white, Kn));
	q = *(bb_lookup(!is_white, Q));
	k = *(bb_lookup(!is_white, K));

	for (int i = 0; i < 64; i++) {
		U64 bit = (p & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, P) * 64 + i];
		}
		bit = (rk & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, R) * 64 + i];
		}
		bit = (bshp & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, B) * 64 + i];
		}
		bit = (kn & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, K) * 64 + i];
		}
		bit = (q & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, Q) * 64 + i];
		}
		bit = (k & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, K) * 64 + i];
		}
	}
	hash ^= is_white ? hashes[768] : 0;
	hash ^= wk_castle ? hashes[769] : 0;
	hash ^= wq_castle ? hashes[770] : 0;
	hash ^= bk_castle ? hashes[771] : 0;
	hash ^= bq_castle ? hashes[772] : 0;
	int index = __builtin_ffsl(p_enpassant) - 1;
	int file = index % 8;
	hash ^= hashes[773 + file]; 
	return hash;
}*/