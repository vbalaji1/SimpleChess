#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t U64;
typedef uint8_t U8;

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
} Vector;

void print_bits(U64 n, bool gridwise);
void init_vector(Vector *v);
void add(Vector *v, U64 element, int origin);
void clean_vector(Vector *v);

#endif