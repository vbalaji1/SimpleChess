#include "bitboard.h"
#include "utilities.h"
#include "pawn.h"
#include "mvgen.h"
#include "state.h"
#include "evaluate.h"
#include "search.h"

U64 wP;
U64 wR;
U64 wKn;
U64 wB;
U64 wQ;
U64 wK;

U64 bP;
U64 bR;
U64 bKn;
U64 bB;
U64 bQ;
U64 bK;

bool wk_castle;
bool wq_castle;
bool bk_castle;
bool bq_castle;

//LITTLE-ENDIAN RANK-FILE MAPPING
void init_bitboards() {
	wP = 0x000000000000FF00;
	wR = 0x0000000000000081;
	wKn = 0x0000000000000042;
	wB = 0x0000000000000024;
	wQ = 0x0000000000000008;
	wK = 0x0000000000000010;
	bP = 0x00FF000000000000;
	bR = 0x8100000000000000;
	bKn = 0x4200000000000000;
	bB = 0x2400000000000000;
	bQ = 0x0800000000000000;
	bK = 0x1000000000000000;
	wk_castle = true;
	wq_castle = true;
	bk_castle = true;
	bq_castle = true;
}

int main() {
	init_bitboards();
	char move[3];
	/*while (true) {
		printf("INPUT MOVE:\n");
		scanf("%s", move);
		if (strcmp(move, "end") == 0) {
			break;
		}
		printf("INPUTTED: %s\n", move);
		input_move(move, true);
		search_driver(false);
		printf("INPUT computer move: \n");
		scanf("%s", move);
		printf("INPUTTED: %s\n", move);
		input_move(move, false);
		//Fix RK_score
		printf("RK_SCORE: %f\n", rk_score(false));
		printf("P_SCORE: %f\n", p_score(false));
	}*/
	input_move("e4", true);
	input_move("e5", false);
	search_driver(true);
}