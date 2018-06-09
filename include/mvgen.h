#ifndef MVGEN_H
#define MVGEN_H

#include "utilities.h"

int validate_sq(U64 bb, bool is_white);
U64 chk_self_legality(U64 move, U64 old, int index, bool is_white);
void gen_all_moves(U64 bb, bool is_white, piece_t type);

#endif