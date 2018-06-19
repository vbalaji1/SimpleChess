#include "bitboard.h"
#include "utilities.h"
#include "mvgen.h"
#include "evaluate.h"
#include "state.h"

const int MAX_DEPTH = 3;
const U64 mask = 1;

bool side;

double ab_min(double alpha, double beta, int depth, bool is_white);

double ab_max(double alpha, double beta, int depth, bool is_white) {
	if (depth == MAX_DEPTH) {
		return total_score(side) - total_score(!side); 
	} 
	Vector *v = (Vector *) malloc(sizeof(Vector)); 
	init_vector(v); 

	if (is_white) {
		v = gen_all_moves(wP, is_white, P, v);
		v = gen_all_moves(wKn, is_white, Kn, v);
		v = gen_all_moves(wB, is_white, B, v);
		v = gen_all_moves(wR, is_white, R, v);
		v = gen_all_moves(wQ, is_white, Q, v);
		v = gen_all_moves(wK, is_white, K, v);
	} else {
		v = gen_all_moves(bP, is_white, P, v);
		v = gen_all_moves(bKn, is_white, Kn, v);
		v = gen_all_moves(bB, is_white, B, v);
		v = gen_all_moves(bR, is_white, R, v);
		v = gen_all_moves(bQ, is_white, Q, v);
		v = gen_all_moves(bK, is_white, K, v); 
	}

	for (int i = 0; i < (v->size); i++) {
		U64 bb = v->elements[i];
		for (int j = 0; j < 64; j++) {
			U64 move = (bb & (mask << j));
			if (move) {
				U64 wP0 = wP;
				U64 wKn0 = wKn;
				U64 wB0 = wB;
				U64 wR0 = wR;
				U64 wQ0 = wQ;
				U64 wK0 = wK;
				U64 bP0 = bP;
				U64 bKn0 = bKn;
				U64 bB0 = bB;
				U64 bR0 = bR;
				U64 bQ0 = bQ;
				U64 bK0 = bK;
				mk_move(move, v->origin[i], is_white, v->piece[i]);
				double score = ab_min(alpha, beta, depth + 1, !is_white);
				if (score >= beta) {
					return beta;
				} 
				if (score > alpha) {
					alpha = score;
				}
				wP = wP0;
				wKn = wKn0;
				wB = wB0;
				wR = wR0;
				wQ = wQ0;
				wK = wK0;
				bP = bP0;
				bKn = bKn0;
				bB = bB0;
				bR = bR0;
				bQ = bQ0;
				bK = bK0;
			}
		}
	}
	clean_vector(v);
	free(v);
	return alpha;
}

double ab_min(double alpha, double beta, int depth, bool is_white) {
	if (depth == MAX_DEPTH) {
		return total_score(side) - total_score(!side);
	}
	Vector *v = (Vector *) malloc(sizeof(Vector)); 
	init_vector(v); 

	if (is_white) {
		v = gen_all_moves(wP, is_white, P, v);
		v = gen_all_moves(wKn, is_white, Kn, v);
		v = gen_all_moves(wB, is_white, B, v);
		v = gen_all_moves(wR, is_white, R, v);
		v = gen_all_moves(wQ, is_white, Q, v);
		v = gen_all_moves(wK, is_white, K, v);
	} else {
		v = gen_all_moves(bP, is_white, P, v);
		v = gen_all_moves(bKn, is_white, Kn, v);
		v = gen_all_moves(bB, is_white, B, v);
		v = gen_all_moves(bR, is_white, R, v);
		v = gen_all_moves(bQ, is_white, Q, v);
		v = gen_all_moves(bK, is_white, K, v); 
	}

	for (int i = 0; i < (v->size); i++) {
		U64 bb = v->elements[i];
		for (int j = 0; j < 64; j++) {
			U64 move = (bb & (mask << j));
			if (move) {
				U64 wP0 = wP;
				U64 wKn0 = wKn;
				U64 wB0 = wB;
				U64 wR0 = wR;
				U64 wQ0 = wQ;
				U64 wK0 = wK;
				U64 bP0 = bP;
				U64 bKn0 = bKn;
				U64 bB0 = bB;
				U64 bR0 = bR;
				U64 bQ0 = bQ;
				U64 bK0 = bK;
				mk_move(move, v->origin[i], is_white, v->piece[i]);
				double score = ab_max(alpha, beta, depth + 1, !is_white);
				if (score <= alpha) {
					return alpha;
				}
				if (score < beta) {
					beta = score;
				}
				wP = wP0;
				wKn = wKn0;
				wB = wB0;
				wR = wR0;
				wQ = wQ0;
				wK = wK0;
				bP = bP0;
				bKn = bKn0;
				bB = bB0;
				bR = bR0;
				bQ = bQ0;
				bK = bK0;
			}
		}
	}
	clean_vector(v);
	free(v);
	return beta;
}

double negamax(int depth, bool is_white) {
	if (depth == MAX_DEPTH) {
		return total_score(side) - total_score(!side);
	}
	double max_score = -INFINITY;

	Vector *v = (Vector *) malloc(sizeof(Vector)); 
	init_vector(v); 

	if (is_white) {
		v = gen_all_moves(wP, is_white, P, v);
		v = gen_all_moves(wKn, is_white, Kn, v);
		v = gen_all_moves(wB, is_white, B, v);
		v = gen_all_moves(wR, is_white, R, v);
		v = gen_all_moves(wQ, is_white, Q, v);
		v = gen_all_moves(wK, is_white, K, v);
	} else {
		v = gen_all_moves(bP, is_white, P, v);
		v = gen_all_moves(bKn, is_white, Kn, v);
		v = gen_all_moves(bB, is_white, B, v);
		v = gen_all_moves(bR, is_white, R, v);
		v = gen_all_moves(bQ, is_white, Q, v);
		v = gen_all_moves(bK, is_white, K, v); 
	}

	for (int i = 0; i < (v->size); i++) {
		U64 bb = v->elements[i];
		for (int j = 0; j < 64; j++) {
			U64 move = (bb & (mask << j));
			if (move) {
				U64 wP0 = wP;
				U64 wKn0 = wKn;
				U64 wB0 = wB;
				U64 wR0 = wR;
				U64 wQ0 = wQ;
				U64 wK0 = wK;
				U64 bP0 = bP;
				U64 bKn0 = bKn;
				U64 bB0 = bB;
				U64 bR0 = bR;
				U64 bQ0 = bQ;
				U64 bK0 = bK;
				mk_move(move, v->origin[i], is_white, v->piece[i]);
				double score = -negamax(depth + 1, !is_white);
				if (score > max_score) {
					max_score = score;
				}
				wP = wP0;
				wKn = wKn0;
				wB = wB0;
				wR = wR0;
				wQ = wQ0;
				wK = wK0;
				bP = bP0;
				bKn = bKn0;
				bB = bB0;
				bR = bR0;
				bQ = bQ0;
				bK = bK0;
			}
		}
	}
	clean_vector(v);
	free(v);
	return max_score;

}
void search_driver(bool is_white) {
	U64 best_mv;
	piece_t type;
	double max_score = -INFINITY;
	side = false;

	Vector *v = (Vector *) malloc(sizeof(Vector)); 
	init_vector(v); 

	if (is_white) {
		v = gen_all_moves(wP, is_white, P, v);
		v = gen_all_moves(wKn, is_white, Kn, v);
		v = gen_all_moves(wB, is_white, B, v);
		v = gen_all_moves(wR, is_white, R, v);
		v = gen_all_moves(wQ, is_white, Q, v);
		v = gen_all_moves(wK, is_white, K, v);
	} else {
		v = gen_all_moves(bP, is_white, P, v);
		v = gen_all_moves(bKn, is_white, Kn, v);
		v = gen_all_moves(bB, is_white, B, v);
		v = gen_all_moves(bR, is_white, R, v);
		v = gen_all_moves(bQ, is_white, Q, v);
		v = gen_all_moves(bK, is_white, K, v);
	}

	for (int i = 0; i < (v->size); i++) {
		U64 bb = v->elements[i];
		for (int j = 0; j < 64; j++) {
			U64 move = (bb & (mask << j));
			if (move) {
				U64 wP0 = wP;
				U64 wKn0 = wKn;
				U64 wB0 = wB;
				U64 wR0 = wR;
				U64 wQ0 = wQ;
				U64 wK0 = wK;
				U64 bP0 = bP;
				U64 bKn0 = bKn;
				U64 bB0 = bB;
				U64 bR0 = bR;
				U64 bQ0 = bQ;
				U64 bK0 = bK;
				mk_move(move, v->origin[i], is_white, v->piece[i]);
				double score = ab_max(-INFINITY, INFINITY, 0, is_white);
				if (score > max_score) {
					type = v->piece[i];
					max_score = score;
					best_mv = move;
				}
				wP = wP0;
				wKn = wKn0;
				wB = wB0;
				wR = wR0;
				wQ = wQ0;
				wK = wK0;
				bP = bP0;
				bKn = bKn0;
				bB = bB0;
				bR = bR0;
				bQ = bQ0;
				bK = bK0;
			}
		}
	}
	clean_vector(v);
	free(v);

	print_bits(best_mv, true);
	printf("PIECE: %d\n", type);
	printf("SCORE: %f\n", max_score);

}