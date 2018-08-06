#ifndef STATE_H
#define STATE_H

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

extern bool is_opening;
extern U64 p_enpassant;

bool chk_mate(bool is_white);
bool chk_enp(U64 mv, int dest_index, bool is_white);
void mk_move(U64 bb, int index, bool is_white, piece_t piece);
void mk_castle(Vector *v, bool k_side, bool is_white);
char *mv_to_index(U64 mv, piece_t type);
char *index_to_sq(int index);
void input_move(char *mv, bool is_white);

#endif