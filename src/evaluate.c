#include "evaluate.h"
#include "bitboard.h"
#include "mvgen.h"
#include "utilities.h"
#include "state.h"


const double P_SCORE = 1;
const double RK_SCORE = 5.2;
const double BSHP_SCORE = 3.3;
const double KN_SCORE = 3.2;
const double Q_SCORE = 9.6;

int p_sq[] = {0,  0,  0,  0,  0,  0,  0,  0,
50, 50, 50, 50, 50, 50, 50, 50,
10, 10, 20, 30, 30, 20, 10, 10,
 5,  5, 10, 27, 25, 10,  5,  5,
 0,  0,  0, 24, 21,  0,  0,  0,
 5, -5,-10,  0,  0,-10, -5,  5,
 5, 10, 10,-20,-20, 10, 10,  5,
 0,  0,  0,  0,  0,  0,  0,  0};

int kn_sq[] = {-50,-40,-30,-30,-30,-30,-40,-50,
-40,-20,  0,  0,  0,  0,-20,-40,
-30,  0, 12, 15, 15, 11,  0,-30,
-30,  5, 16, 20, 22, 17,  5,-30,
-30,  0, 14, 23, 20, 15,  0,-30,
-30,  5, 10, 14, 16, 11,  5,-30,
-40,-20,  0,  5,  5,  0,-20,-40,
-50,-40,-30,-30,-30,-30,-40,-50};

int b_sq[] = {-20,-10,-10,-10,-10,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5, 10, 10,  5,  0,-10,
-10,  5,  5, 10, 10,  5,  5,-10,
-10,  0, 10, 10, 10, 10,  0,-10,
-10, 10, 10, 10, 10, 10, 10,-10,
-10,  5,  0,  0,  0,  0,  5,-10,
-20,-10,-10,-10,-10,-10,-10,-20};

int r_sq[] = {  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0};

int q_sq[] = {-20,-10,-10, -5, -5,-10,-10,-20,
-10,  0,  0,  0,  0,  0,  0,-10,
-10,  0,  5,  5,  5,  5,  0,-10,
 -5,  0,  5,  5,  5,  5,  0, -5,
  0,  0,  5,  5,  5,  5,  0, -5,
-10,  5,  5,  5,  5,  5,  0,-10,
-10,  0,  5,  0,  0,  0,  0,-10,
-20,-10,-10, -5, -5,-10,-10,-20};

int k_sq[] = {-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-30,-40,-40,-50,-50,-40,-40,-30,
-20,-30,-30,-40,-40,-30,-30,-20,
-10,-20,-20,-20,-20,-20,-20,-10,
 20, 20,  0,  0,  0,  0, 20, 20,
 20, 30, 10,  0,  0, 10, 30, 20};

double material_score(bool is_white) {
	U64 *p = bb_lookup(is_white, P);
	U64 *rk = bb_lookup(is_white, R);
	U64 *bshp = bb_lookup(is_white, B);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *q = bb_lookup(is_white, Q);

	U64 mask = 1;
	double score = 0;
	
	for (int i = 0; i < 64; i++) {
		if ((*p >> i) & mask) {
			score += P_SCORE;
		}
		if ((*rk >> i) & mask) {
			score += RK_SCORE;
		}
		if ((*bshp >> i) & mask) {
			score += BSHP_SCORE;
		}
		if ((*kn >> i) & mask) {
			score += KN_SCORE;
		}
		if ((*q >> i) & mask) {
			score += Q_SCORE;
		}
	}
	return score;
}

double center_score(bool is_white) {
	U64 *p = bb_lookup(is_white, P);
	U64 *rk = bb_lookup(is_white, R);
	U64 *bshp = bb_lookup(is_white, B);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *q = bb_lookup(is_white, Q);
	U64 *k = bb_lookup(is_white, K);

	U64 center = 0x0000001818000000;
	U64 mask = 1;
	Vector *v = (Vector *) malloc(sizeof(Vector));
	init_vector(v);
	v = gen_all_moves(*p, is_white, P, v);
	v = gen_all_moves(*rk, is_white, R, v);
	v = gen_all_moves(*bshp, is_white, B, v);
	v = gen_all_moves(*kn, is_white, Kn, v);
	v = gen_all_moves(*q, is_white, Q, v);
	v = gen_all_moves(*k, is_white, K, v);
	size_t score = 0;
	if (is_white) {
		for (int i = 0; i < 64; i++) {
			U64 piece = (*p & (mask << i));
			if (piece) {
				if (i % 8 == 0) {
					score += __builtin_popcountl((*p << 7) & center);
				} else if (i % 8 == 7) {
					score += __builtin_popcountl((*p << 9) & center);
				} else {
					score += __builtin_popcountl((*p << 7) & center);
					score += __builtin_popcountl((*p << 9) & center);
				}
			}
		}
	} else {
		for (int i = 0; i < 64; i++) {
			U64 piece = (*p & (mask << i));
			if (piece) {
				if (i % 8 == 0) {
					score += __builtin_popcountl((*p >> 9) & center);
				} else if (i % 8 == 7) {
					score += __builtin_popcountl((*p >> 7) & center);
				} else {
					score += __builtin_popcountl((*p >> 9) & center);
					score += __builtin_popcountl((*p >> 7) & center);
				}
			}
		}
	}
	for (int i = 0; i < (v->size); i++) {
		U64 bb = v->elements[i];
		score += __builtin_popcountl(bb & center);
	}
	clean_vector(v);
	free(v);
	return (double) score;
}

double rk_score(bool is_white) {
	U64 mask = 1;
	bool connected = false;
	int semi_open = 0;
	int open = 0;
	int idx, idx2;
	idx = idx2 = -1;
	U64 *rk = bb_lookup(is_white, R);
	U64 *p = bb_lookup(is_white, P);
	U64 *opp_p = bb_lookup(!is_white, P);
	int rk_count = __builtin_popcountl(*rk);
	if (rk_count == 1 || rk_count == 2) {
		for (int i = 0; i < 64; i++) {
			U64 piece = (*rk & (mask << i));
			if (piece && idx == -1) {
				idx = i;
			} else if (piece && idx2 == -1) {
				idx2 = i;
				break;
			}
		}
	}
	if (idx != -1) {
		int prev = idx % 8;
		int next = 7 - prev;
		for (int i = 1; i <= prev; i++) {
			U64 slide = (mask << (idx - i));
			if (validate_sq(slide, is_white) == 2) {
				continue;
			} else if (validate_sq(slide, is_white) == 1) {
				break;
			} else if (validate_sq(slide, is_white) == 0) {
				if (slide & *rk) {
					connected = true;
					break;
				} else {
					break;
				}
			}
		}
		if (!connected) {
			for (int i = 1; i <= next; i++) {
				U64 slide = (mask << (idx + i));
				if (validate_sq(slide, is_white) == 2) {
					continue;
				} else if (validate_sq(slide, is_white) == 1) {
					break;
				} else if (validate_sq(slide, is_white) == 0) {
					if (slide & *rk) {
						connected = true;
						break;
					} else {
						break;
					}
				}
			}
		}
		int rank = idx / ((U64) 8);
		prev = rank % 8;                  
		next = 7 - prev;
		if (!connected) {
			for (int i = 1; i <= prev; i++) {
				U64 slide = (mask << (idx - (i * 8)));
				if (validate_sq(slide, is_white) == 2) {
					continue;
				} else if (validate_sq(slide, is_white) == 1) {
					break;
				} else if (validate_sq(slide, is_white) == 0) {
					if (slide & *rk) {
						connected = true;
						break;
					} else {
						break;
					}
				}
			}
		}
		if (!connected) {
			for (int i = 1; i <= next; i++) {
				U64 slide = (mask << (idx + (i * 8)));
				if (validate_sq(slide, is_white) == 2) {
					continue;
				} else if (validate_sq(slide, is_white) == 1) {
					break;
				} else if (validate_sq(slide, is_white) == 0) {
					if (slide & *rk) {
						connected = true;
						break;
					} else {
						break;
					}
				}
			}
		}
		bool open_file[7] = { true };
		for (int i = 0; i < 64; i++) {
			U64 self = (*p & (mask << i));
			if (self) {
				int file = (idx - i) % 8;
				if (file == 0) {
					open_file[file] = false; 
				}
				if (rk_count == 2) {
					file = (idx2 - i) % 8;
					if (file == 0) {
						open_file[file] = false;
					}
				}
			}
		}
		for (int i = 0; i < 64; i++) {
			U64 piece = (*opp_p & (mask << i));
			if (piece) {
				int file = (idx - i) % 8;
				if (file == 0 && open_file[file]) {
					semi_open++;
				}
				if (rk_count == 2) {
					file = (idx2 - i) % 8;
					if (file == 0 && open_file[file]) {
						semi_open++;
					}
				}
			}
		}
		int rank2 = idx2 / ((U64) 8);
		int prev2 = rank2 % 8;
		int next2 = 7 - prev2;
		bool clear = true;
		for (int i = 1; i <= prev; i++) {
			U64 op = (mask << (idx - (i * 8)));
			if (validate_sq(op, is_white) != 2) {
				clear = false;
			}
		}
		for (int i = 1; i <= next; i++) {
			U64 op = (mask << (idx + (i * 8)));
			if (validate_sq(op, is_white) != 2) {
				clear = false;
			}
		}
		open = clear ? open + 1 : open;
		clear = true;
		if (rk_count == 2) {
			for (int i = 1; i <= prev2; i++) {
				U64 op = (mask << (idx2 - (i * 8)));
				if (validate_sq(op, is_white) != 2) {
					clear = false;
				}
			}
			for (int i = 1; i <= next2; i++) {
				U64 op = (mask << (idx2 + (i * 8)));
				if (validate_sq(op, is_white) != 2) {
					clear = false;
				}
			}
			open = clear ? open + 1 : open;
		}
	}
	double connect_bonus = connected ? 0.15 : 0.0;
	return connect_bonus + open * 0.15 + semi_open * 0.1;
}

double p_score(bool is_white) {
	U64 mask = 1;
	double dub_penalty = 0.0;
	double iso_penalty = 0.0;
	bool isolated = true;
	U64 *p = bb_lookup(is_white, P);
	for (int i = 0; i < 64; i++) {
		U64 piece = (*p & (mask << i));
		if (piece) {
			int rank = i / 8;
			int prev = rank % 8;
			int next = 7 - prev;
			for (int j = 1; j <= prev; j++) {
				U64 sq = (mask << (i - (j * 8)));
				U64 adj_l = (mask << (i - 1 - (j * 8)));
				U64 adj_r = (mask << (i + 1 - (j * 8)));
				if (sq & *p) {
					dub_penalty += -0.25;
				}
				if (i % 8 == 0) {
					if (adj_r & *p) {
						isolated = false;

					}
				} else if (i % 7 == 0) {
					if (adj_l & *p) {
						isolated = false;
					}
				} else {
					if ((adj_r & *p) || (adj_l && *p)) {
						isolated = false;
					}
				}
			} 
			for (int j = 1; j <= next; j++) {
				U64 sq = (mask << (i + (j * 8)));
				U64 adj_l = (mask << (i - 1 + (j * 8)));
				U64 adj_r = (mask << (i + 1 + (j * 8)));
				if (sq & *p) {
					dub_penalty += -0.25;
				}
				if (i % 8 == 0) {
					if (adj_r & *p) {
						isolated = false;
					}
				} else if (i % 7 == 0) {
					if (adj_l & *p) {
						isolated = false;
					}
				} else {
					if ((adj_r & *p) || (adj_l & *p)) {
						isolated = false;
					}
				}
			}
			U64 adj = (mask << (i - 1));
			if (adj & *p) {
				isolated = false;
			}
			adj = (mask << (i + 1));
			if (adj & *p) {
				isolated = false;
			}
			if (isolated) {
				iso_penalty += -0.10;
			}
			isolated = true;
		}
	}
	return dub_penalty + iso_penalty;
}
double total_score(bool is_white) {
	U64 mask = 1;
	double endgame = -20000.0;
	double total_score = 2 * material_score(is_white) + 0.1 * center_score(is_white)
		+ rk_score(is_white);
	if (is_endgame(is_white) == MAX_DEPTH) {
		total_score += p_score(is_white);
	}
	U64 *p = bb_lookup(is_white, P);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *b = bb_lookup(is_white, B);
	U64 *rk = bb_lookup(is_white, R);
	U64 *q = bb_lookup(is_white, Q);
	U64 *k = bb_lookup(is_white, K);

	double bonus = 0.0;
	for (int i = 0; i < 64; i++) {
		int idx = is_white ? 63 - i : i;
		U64 piece = (*p & (mask << i));
		if (piece) {
			bonus = (double) p_sq[idx] / (double) 100;
			total_score += bonus;
		}
		piece = (*kn & (mask << i));
		if (piece) {
			bonus = (double) kn_sq[idx] / (double) 100;
			total_score += bonus;
		}
		piece = (*b & (mask << i));
		if (piece) {
			bonus = (double) b_sq[idx] / (double) 100;
			total_score += bonus;
		}
		piece = (*rk & (mask << i));
		if (piece) {
			bonus = (double) r_sq[idx] / (double) 100;
			total_score += bonus;
		}
		piece = (*q & (mask << i));
		if (piece) {
			bonus = (double) q_sq[idx] / (double) 100;
			total_score += bonus;
		}
		piece = (*k & (mask << i));
		if (piece) {
			bonus = (double) k_sq[idx] / (double) 100;
			total_score += bonus;
		}
	}
 	if (chk_mate(is_white)) {
		total_score += endgame;
	}
	return total_score;
}


int is_endgame(bool is_white) {
	U64 *p = bb_lookup(is_white, P);
	U64 *rk = bb_lookup(is_white, R);
	U64 *bshp = bb_lookup(is_white, B);
	U64 *kn = bb_lookup(is_white, Kn);
	U64 *q = bb_lookup(is_white, Q);
	U64 *k = bb_lookup(is_white, K);

	U64 *p1 = bb_lookup(!is_white, P);
	U64 *rk1 = bb_lookup(!is_white, R);
	U64 *bshp1 = bb_lookup(!is_white, B);
	U64 *kn1 = bb_lookup(!is_white, Kn);
	U64 *q1 = bb_lookup(!is_white, Q);
	U64 *k1 = bb_lookup(!is_white, K);

	size_t w_total = __builtin_popcountl(*p) + __builtin_popcountl(*rk)
		+ __builtin_popcountl(*bshp) + __builtin_popcountl(*kn) 
		+ __builtin_popcountl(*q) + __builtin_popcountl(*k);
	size_t b_total = __builtin_popcountl(*p1) + __builtin_popcountl(*rk1)
		+ __builtin_popcountl(*bshp1) + __builtin_popcountl(*kn1) 
		+ __builtin_popcountl(*q1) + __builtin_popcountl(*k1);

	if (w_total + b_total < 5) {
		return 20;
	} else if (w_total + b_total <= 10) {
		return 15;
	} else {
		return MAX_DEPTH;
	}
}
