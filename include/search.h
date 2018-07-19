#ifndef SEARCH_H
#define SEARCH_H

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

extern HashEntry transpositions[1299827];

void search_driver(bool is_white);

#endif