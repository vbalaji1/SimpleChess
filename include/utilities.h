#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef uint64_t U64;

extern bool wk_castle;
extern bool wq_castle;
extern bool bk_castle;
extern bool bq_castle;

extern U64 p_enpassant;
extern bool use_transpose;
extern bool pc_display;

typedef enum piece {
	P,
	R,
	Kn,
	B,
	Q,
	K
} piece_t;

typedef struct vector {
	U64 *elements;
	size_t size;
	size_t max_capacity;
	int *origin;
	piece_t* piece; 
} Vector;

typedef struct hashentry {
	U64 key;
	int depth;
	bool ancient;
	double score;
	bool valid;
} HashEntry;

void print_bits(U64 n, bool gridwise);
void init_vector(Vector *v);
void add(Vector *v, U64 element, int origin, piece_t type);
void clean_vector(Vector *v);
void piece_display();
void init_zobrist();
void clean_zobrist();
U64 zobrist_hash(bool is_white);

#endif