#include "bitboard.h"
#include "utilities.h"
#include "mvgen.h"
#include "evaluate.h"
#include "state.h"

int MAX_DEPTH = 3;
const U64 mask = 1;

double alpha_root;

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
	Vector *vk_castle = (Vector *) malloc(sizeof(Vector));
	init_vector(vk_castle);
	vk_castle = gen_k_castle(is_white);

	Vector *vq_castle = (Vector *) malloc(sizeof(Vector)); 
	init_vector(vq_castle);
	vq_castle = gen_q_castle(is_white);

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
	if (vk_castle->elements[0] && vk_castle->elements[1]) {
		if ((is_white && wk_castle) || (!is_white && bk_castle)) {
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
			bool wk_castle0 = wk_castle;
			bool wq_castle0 = wq_castle;
			bool bk_castle0 = bk_castle;
			bool bq_castle0 = bq_castle;
			mk_castle(vk_castle, true, is_white);
			double k_score = -alpha_beta(-beta, -alpha, depth + 1, !is_white);
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
			wk_castle = wk_castle0;
			wq_castle = wq_castle0;
			bk_castle = bk_castle0;
			bq_castle = bq_castle0;
			if (k_score >= beta) {
				return beta;
			} 
			if (k_score > alpha) {
				alpha = k_score;
			}
		}
	}
	if (vq_castle->elements[0] && vq_castle->elements[1]) {
		if ((is_white && wq_castle) || (!is_white && bq_castle)) {
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
			bool wk_castle0 = wk_castle;
			bool wq_castle0 = wq_castle;
			bool bk_castle0 = bk_castle;
			bool bq_castle0 = bq_castle;
			mk_castle(vq_castle, false, is_white);
			double q_score = -alpha_beta(-beta, -alpha, depth + 1, !is_white);
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
			wk_castle = wk_castle0;
			wq_castle = wq_castle0;
			bk_castle = bk_castle0;
			bq_castle = bq_castle0;
			if (q_score >= beta) {
				return beta;
			} 
			if (q_score > alpha) {
				alpha = q_score;
			}
		}
	}
	clean_vector(vk_castle);
	clean_vector(vq_castle);
	clean_vector(v);
	free(v);
	free(vq_castle);
	free(vk_castle);
	return alpha;
}
void search_driver(bool is_white) {
	U64 best_mv;
	piece_t type;
	int origin;
	double max_score = -INFINITY;
	alpha_root = INFINITY;
	MAX_DEPTH = is_endgame(is_white);

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
	Vector *vk_castle = (Vector *) malloc(sizeof(Vector));
	init_vector(vk_castle);
	vk_castle = gen_k_castle(is_white);

	Vector *vq_castle = (Vector *) malloc(sizeof(Vector)); 
	init_vector(vq_castle);
	vq_castle = gen_q_castle(is_white);

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
				bool wk_castle0 = wk_castle;
				bool wq_castle0 = wq_castle;
				bool bk_castle0 = bk_castle;
				bool bq_castle0 = bq_castle;
				mk_move(move, v->origin[i], is_white, v->piece[i]);
				alpha_root = -alpha_beta(-INFINITY, INFINITY, 0, !is_white);
					print_bits(move, true);
					printf("PIECE: %d\n", v->piece[i]);
					printf("ORIGIN: %d\n", v->origin[i]);
					printf("ROOT: YES\n");
					printf("%f\n", alpha_root);
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
				wk_castle = wk_castle0;
				wq_castle = wq_castle0;
				bk_castle = bk_castle0;
				bq_castle = bq_castle0;
				if (alpha_root > max_score) {
					type = v->piece[i];
					max_score = alpha_root;
					origin = v->origin[i];
					best_mv = move;
				}
			}
		}
	}
	if (vk_castle->elements[0] && vk_castle->elements[1]) {
		if ((is_white && wk_castle) || (!is_white && bk_castle)) {
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
			bool wk_castle0 = wk_castle;
			bool wq_castle0 = wq_castle;
			bool bk_castle0 = bk_castle;
			bool bq_castle0 = bq_castle;
			mk_castle(vk_castle, true, is_white);
			alpha_root = -alpha_beta(-INFINITY, INFINITY, 0, !is_white);
			printf("K_SIDE_CASTLE\n");
					printf("%f\n", alpha_root);
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
			wk_castle = wk_castle0;
			wq_castle = wq_castle0;
			bk_castle = bk_castle0;
			bq_castle = bq_castle0;
			if (alpha_root > max_score) {
					type = vk_castle->piece[0];
					max_score = alpha_root;
					origin = vk_castle->origin[0];
					best_mv = vk_castle->elements[0];
			}
		}
	}
	if (vq_castle->elements[0] && vq_castle->elements[1]) {
		if ((is_white && wq_castle) || (!is_white && bq_castle)) {
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
			bool wk_castle0 = wk_castle;
			bool wq_castle0 = wq_castle;
			bool bk_castle0 = bk_castle;
			bool bq_castle0 = bq_castle;
			mk_castle(vq_castle, false, is_white);
			alpha_root = -alpha_beta(-INFINITY, INFINITY, 0, !is_white);
			printf("K_SIDE_CASTLE\n");
					printf("%f\n", alpha_root);
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
			wk_castle = wk_castle0;
			wq_castle = wq_castle0;
			bk_castle = bk_castle0;
			bq_castle = bq_castle0;
			if (alpha_root > max_score) {
					type = vq_castle->piece[0];
					max_score = alpha_root;
					origin = vq_castle->origin[0];
					best_mv = vq_castle->elements[0];
			}
		}
	}
	clean_vector(vk_castle);
	clean_vector(vq_castle);
	clean_vector(v);
	free(v);
	free(vq_castle);
	free(vk_castle);

	print_bits(best_mv, true);
	printf("%s\n", mv_to_index(best_mv, type));
	printf("PIECE: %d\n", type);
	printf("ORIGIN: %d\n", origin);
	printf("SCORE: %f\n", alpha_root);

}