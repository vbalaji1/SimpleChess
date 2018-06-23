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

U64 wRtest;
U64 a1_h8diag;

//LITTLE-ENDIAN RANK-FILE MAPPING
void init_bitboards() {
	//wP = 0x0000006000009F00;
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
	//bQ = 0x0000000080000000;
	bQ = 0x0800000000000000;
	bK = 0x1000000000000000;
	wk_castle = true;
	wq_castle = true;
	bk_castle = true;
	bq_castle = true;



	wRtest = 0x0000000000000001;

	a1_h8diag = 0x8040201008040201;


}

int main() {
	init_bitboards();
	//print_bits(wP, true);
	//print_bits(wR, true);
	//print_bits(wK, true);
	//print_bits(wB, true);
	//print_bits(wQ, true);
	//print_bits(wK, true);
	//print_bits(bP, true);
	//print_bits(bR, true);
	//print_bits(bK, true);
	//print_bits(bB, true);
	//print_bits(bQ, true);
	//print_bits(bK, true);
	char move[3];
	bool side = true;
	while (true) {
		printf("INPUT MOVE:\n");
		scanf("%s", move);
		printf("INPUTTED: %s\n", move);
		input_move(move, side);
		search_driver(!side);
		printf("INPUT computer move: \n");
		scanf("%s", move);
		printf("INPUTTED: %s\n", move);
		input_move(move, !side);
	}
	//Add castling to computer move generation
	//printf("%f\n", total_score(false) - total_score(true));

}