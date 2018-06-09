#ifndef PAWN_H
#define PAWN_H

#include "utilities.h"
#include "bitboard.h"

extern U64 p_enpassant;

void promotion(U64 bb, piece_t piece);

#endif