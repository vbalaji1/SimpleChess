#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t U64;

typedef enum piece {
	P,
	R,
	Kn,
	B,
	Q,
	K
} piece_t;

void print_bits(U64 n, bool gridwise);

#endif