#include "utilities.h"

void print_bits(U64 n, bool gridwise) {
	//n = __builtin_bswap64(n);
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