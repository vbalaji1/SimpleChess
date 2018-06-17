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

extern U64 p_enpassant;

bool chk_mate(bool is_white);
void mk_move(U64 bb, int index, bool is_white, piece_t piece);
void input_move(char *mv, bool is_white);

#endif