#ifndef MVGEN_H
#define MVGEN_H

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

int validate_sq(U64 bb, bool is_white);
U64 chk_self_legality(U64 move, int index, bool is_white, piece_t type);
Vector *gen_all_moves(U64 bb, bool is_white, piece_t type, Vector *v);

#endif