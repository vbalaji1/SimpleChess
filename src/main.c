#include "bitboard.h"
#include "utilities.h"
#include "pawn.h"
#include "mvgen.h"

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


U64 wRtest;
U64 a1_h8diag;

//LITTLE-ENDIAN RANK-FILE MAPPING
void init_bitboards() {
	//wP = 0x000000001000EF00;
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

	wRtest = 0x0000040000000000; 


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
	print_bits(wKn, true);
	print_bits(wP, true);
	print_bits(bP, true);
	gen_all_moves(wKn, true, Kn);
}