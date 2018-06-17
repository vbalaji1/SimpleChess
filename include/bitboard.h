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

extern bool wk_castle;
extern bool wq_castle;
extern bool bk_castle;
extern bool bq_castle;

U64 *bb_lookup(bool is_white, piece_t type);
bool can_castle(bool k_side, bool is_white);
U64 gen_p_capture(U64 bb, U64 opp, bool is_white);
Vector *gen_k_castle(bool is_white);
Vector *gen_q_castle(bool is_white);
U64 gen_mv_piece(U64 piece, int index, bool is_white, piece_t type);

#endif