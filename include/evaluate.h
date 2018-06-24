#ifndef EVALUATE_H
#define EVALUATE_H

#include <stdbool.h>

extern int MAX_DEPTH;

double material_score(bool is_white);
double center_score(bool is_white);
double rk_score(bool is_white);
double p_score(bool is_white);
double total_score(bool is_white);
int is_endgame(bool is_white); 

#endif