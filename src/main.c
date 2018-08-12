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
	is_opening = true;
}

int main() {
	init_bitboards();
	char move[3];
	char usr[1];
	bool tp_setup = false;
	do {
		printf("USE TRANSPOSITION TABLE?\nYES : y\nNO: n\n");
		scanf("%s", usr);
		if (strcmp(usr, "y") == 0) {
			use_transpose = true;
			tp_setup = true;
		} else if (strcmp(usr, "n") == 0) {
			use_transpose = false;
			tp_setup = true;
		}
	} while (!tp_setup); 

	bool pc_setup = false;
	do {
		printf("USE PIECE LOCATION DISPLAY?\nYES : y\nNO: n\n");
		scanf("%s", usr);
		if (strcmp(usr, "y") == 0) {
			pc_display = true;
			pc_setup = true;
		} else if (strcmp(usr, "n") == 0) {
			pc_display = false;
			pc_setup = true;
		}
	} while (!pc_setup);

	bool color_setup = false;
	bool side;
	do {
		printf("PLAY BLACK OR WHITE?\nBLACK : b\nWHITE: w\n");
		scanf("%s", usr);
		if (strcmp(usr, "w") == 0) {
			side = false;
			color_setup = true;
		} else if (strcmp(usr, "b") == 0) {
			side = true;
			color_setup = true;
		}
	} while (!color_setup);

	printf("ENTER 'end' DURING YOUR MOVE TO END THE GAME\n");
	if (pc_display) {
		piece_display();
	}
	if (side) {
		while (true) {
			search_driver(true);
			if (pc_display) {
				piece_display();
			}
			printf("INPUT YOUR MOVE:\n");
			scanf("%s", move);
			if (strcmp(move, "end") == 0) {
				remove("../latest");
				break;
			}
			input_move(move, false);
			if (pc_display) {
				piece_display();
			}
		}
	} else {
		while (true) {
			printf("INPUT YOUR MOVE:\n");
			scanf("%s", move);
			if (strcmp(move, "end") == 0) {
				remove("../latest");
				break;
			}
			input_move(move, true);
			if (pc_display) {
				piece_display();
			}
			search_driver(false);
			if (pc_display) {
				piece_display();
			}
		}
	}
}