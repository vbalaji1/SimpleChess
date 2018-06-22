#include "bitboard.h"
#include "utilities.h"
#include "mvgen.h"
#include "evaluate.h"
#include "state.h"

const int MAX_DEPTH = 3;
const U64 mask = 1;

double alpha_root;
double beta_root; 

bool debug;

double ab_min(double alpha, double beta, int depth, bool is_white);

double ab_max(double alpha, double beta, int depth, bool is_white) {

	if (depth == MAX_DEPTH || chk_mate(is_white) || chk_mate(!is_white)) {
		return total_score(!is_white) - total_score(is_white); 
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
				print_bits(move, true);
					printf("PIECE: %d\n", v->piece[i]);
					printf("ORIGIN: %d\n", v->origin[i]);
					printf("%f\n", score);
					printf("DEPTH: %d\n", depth);
					printf("MIN/MAX: %s\n", "max");
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

	if (depth == MAX_DEPTH || chk_mate(is_white) || chk_mate(!is_white)) {
		return total_score(!is_white) - total_score(is_white);
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
				print_bits(move, true);
					printf("PIECE: %d\n", v->piece[i]);
					printf("ORIGIN: %d\n", v->origin[i]);
					printf("%f\n", score);
					printf("DEPTH: %d\n", depth);
					printf("MIN/MAX: %s\n", "min");
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

double alpha_beta(double alpha, double beta, int depth, bool is_white) {

	if (depth == MAX_DEPTH || chk_mate(is_white) || chk_mate(!is_white)) {
	
		return total_score(is_white) - total_score(!is_white); 
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
				double score = -alpha_beta(-beta, -alpha, depth + 1, !is_white);
				/*if (debug) {
					print_bits(move, true);
					printf("PIECE: %d\n", v->piece[i]);
					printf("ORIGIN: %d\n", v->origin[i]);
					printf("%f\n", score);
					printf("DEPTH: %d\n", depth);
				}*/
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
				if (score >= beta) {
					return beta;
				} 
				if (score > alpha) {
					alpha = score;
				}
			}
		}
	}
	clean_vector(v);
	free(v);
	return alpha;
}
void search_driver(bool is_white) {
	U64 best_mv;
	piece_t type;
	int origin;
	double max_score = -INFINITY;
	alpha_root = INFINITY;

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
				alpha_root = -alpha_beta(-INFINITY, INFINITY, 0, !is_white);
					print_bits(move, true);
					printf("PIECE: %d\n", v->piece[i]);
					printf("ORIGIN: %d\n", v->origin[i]);
					printf("ROOT: YES\n");
					printf("%f\n", alpha_root);
				debug = false;
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
				if (alpha_root > max_score) {
					type = v->piece[i];
					max_score = alpha_root;
					origin = v->origin[i];
					best_mv = move;
				}
			}
		}
	}
	clean_vector(v);
	free(v);

	print_bits(best_mv, true);
	printf("PIECE: %d\n", type);
	printf("ORIGIN: %d\n", origin);
	printf("SCORE: %f\n", alpha_root);

}