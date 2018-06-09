#ifndef BITBOARD_H
#define BITBOARD_H

#include "utilities.h"

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

U64 gen_mv_piece(U64 piece, int index, bool is_white, piece_t type);

#endif