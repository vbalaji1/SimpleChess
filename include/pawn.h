#ifndef PAWN_H
#define PAWN_H

#include "utilities.h"
#include "bitboard.h"

extern U64 p_enpassant;

extern U64 wP;
extern U64 wR;
extern U64 wKn;
extern U64 wB;
extern U64 wQ;
extern U64 wK;

extern U64 bP;
extern U64 bR;
extern U64 bKn;
extern U64 bB;
extern U64 bQ;
extern U64 bK;

void promotion(int index, bool is_white, piece_t promoted_type);
U64 gen_enpassant(U64 bb, bool is_white);

#endif