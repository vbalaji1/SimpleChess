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
	/*static char move[3];
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
	}*/
	input_move("d4", true);
	input_move("e6", false);
	input_move("Nf3", true);
	input_move("Nc6", false);
	input_move("c3", true);
	input_move("Nge7", false);
	input_move("e4", true);
	input_move("a6", false);
	input_move("Bg4", true);
	input_move("f6", false);
	input_move("Be3", true);
	input_move("d6", false);
	input_move("Bd3", true);
	input_move("Bd7", false);
	input_move("O-O", true);
	input_move("d5", false);
	input_move("Nbd2", true);
	input_move("b5", false);
	input_move("e5", true);
	input_move("e5", false);
	input_move("e5", true);
	input_move("Ng8", false);
	input_move("Bg5", true);
	input_move("Be7", false);
	input_move("Be3", true);
	input_move("Nh6", false);
	input_move("h3", true);
	input_move("Nf7", false);
	input_move("Bf4", true);
	input_move("g5", false);
	input_move("Bh2", true);
	input_move("Kf8", false);
	input_move("Bg3", true);
	input_move("Bc5", false);
	input_move("Nb3", true);
	input_move("Bb6", false);
	input_move("Qd2", true);
	input_move("Qe7", false);
	input_move("a3", true);
	input_move("Rd8", false);
	input_move("Rd1", true);
	input_move("Bc8", false);
	input_move("Qe2", true);
	input_move("Bb7", false);
	input_move("Bc2", true);
	input_move("Re8", false);
	input_move("Rac1", true);
	input_move("Rd8", false);
	input_move("h4", true);
	input_move("g4", false);
	input_move("Ne1", true);
	input_move("Nh6", false);
	input_move("Qd2", true);
	input_move("Kg7", false);
	input_move("Qf4", true);
	input_move("Rhf8", false);
	input_move("Qg5", true);
	input_move("Qg5", false);
	input_move("g5", true);
	input_move("Nf5", false);
	input_move("Bf5", true);
	input_move("Rf5", false);
	input_move("Nd3", true);
	input_move("Kg8", false);
	input_move("Nbc5", true);
	input_move("Bc8", false);
	input_move("Ra1", true);
	input_move("Re8", false);
	input_move("b4", true);
	input_move("Rd8", false);
	input_move("Rb1", true);
	input_move("Re8", false);
	input_move("a4", true);
	input_move("a4", false);
	input_move("g6", true);
	input_move("g6", false);
	input_move("Ra1", true);
	input_move("a3", false);
	input_move("Ra3", true);
	input_move("Rh5", false);
	input_move("Nf4", true);
	input_move("Rf5", false);
	input_move("Ncd3", true);
	input_move("Ne5", false);
	input_move("Ne5", true);
	input_move("Re5", false);
	input_move("Nd3", true);
	input_move("Re4", false);
	input_move("Ra2", true);
	input_move("Rc4", false);
	input_move("Rc1", true);
	input_move("Bb7", false);
	input_move("Ne5", true);
	input_move("Re4", false);
	input_move("Nd7", true);
	input_move("Kf7", false);
	input_move("Rd1", true);
	input_move("Rd8", false);
	input_move("Nc5", true);
	input_move("Bc5", false);
	input_move("Bc7", true);
	input_move("Rd7", false);
	input_move("Bh2", true);
	input_move("Bb6", false);
	input_move("R2a1", true);
	input_move("Rc4", false);
	input_move("Rac1", true);
	input_move("Bc7", false);
	input_move("Bg3", true);
	input_move("Bg3", false);
	input_move("Rc2", true);
	input_move("Bc7", false);
	input_move("R1d2", true);
	input_move("g3", false);
	input_move("Rd3", true);
	input_move("Kg8", false);
	input_move("Re3", true);
	input_move("Re7", false);
	input_move("R2e2", true);
	input_move("Bc8", false);
	input_move("R2c2", true);
	input_move("Bf4", false);
	input_move("Rf3", true);
	input_move("f2", false);
	input_move("R2f2", true);
	input_move("Bh6", false);
	input_move("R2b2", true);
	input_move("Re4", false);
	input_move("Kf2", true);
	input_move("Bg5", false);
	input_move("Rc2", true);
	input_move("Bh4", false);
	input_move("g3", true);
	input_move("Bg5", false);
	input_move("Re2", true);
	input_move("Re2", false);
	input_move("Ke2", true);
	input_move("e5", false);
	input_move("Kf2", true);
	input_move("e4", false);
	input_move("c4", true);
	input_move("f3", false);
	input_move("b5", true);
	input_move("c4", false);
	input_move("a6", true);
	input_move("Ba6", false);
	input_move("Kf1", true);
	input_move("Be3", false);
	input_move("Ke1", true);
	input_move("Rd7", false);
	input_move("g4", true);
	input_move("Bb5", false);
	input_move("g5", true);
	input_move("Bc6", false);
	input_move("Kf1", true);
	input_move("Rd2", false);
	input_move("Ke1", true);
	input_move("Bg5", false);
	input_move("Kf1", true);
	input_move("Be3", false);
	input_move("Ke1", true);
	input_move("f2", false);
	input_move("Kf1", true);
	input_move("Rd1", false);
	input_move("Ke2", true);
	input_move("f1", false);
	input_move("Ke3", true);
	input_move("Qg2", false);
	input_move("Kf4", true);
	input_move("Rd3", false);
	input_move("Ke5", true);
	input_move("Qf1", false);
	input_move("Ke6", true);
	input_move("Rd2", false);
	input_move("Ke6", true);
	input_move("Qa1", false);
	input_move("Kf4", true);
	input_move("Qb2", false);
	input_move("Kg5", true);
	input_move("Qa1", false);
	input_move("Kh4", true);
	input_move("Qb2", false);
	input_move("Kg5", true);
	search_driver(false);
	//Add castling to computer move generation
	//printf("%f\n", total_score(false) - total_score(true));

}