#include "utilities.h"
#include "bitboard.h"
#include "pawn.h"
#include "mt64.h"

#define INDEX(is_white, piece) (is_white ? piece : piece + 6)

U64 hashes[781];
HashEntry transpositions[1299827];
bool use_transpose;
bool pc_display;

void print_bits(U64 n, bool gridwise) {
	U64 mask = 1;
	for (U64 i = 0; i < 64; i++) {
		if (gridwise && (i % 8 == 0)) {
			printf("\n");
		}
		(n & mask) ? printf("1 ") : printf("0 ");
		n >>= 1;
	}
    printf("\n");
}

void init_vector(Vector *v) {
	v->elements = (U64 *) malloc(sizeof(U64) * 10);
	v->size = 0;
	v->max_capacity = 10;
	v->origin = (int *) malloc(sizeof(int) * 10);
	v->piece = (piece_t *) malloc(sizeof(piece_t) * 10);
}

void add(Vector *v, U64 element, int origin, piece_t type) {
	if (v->size == v->max_capacity) {
		v->max_capacity *= 2;
		v->elements = (U64 *) realloc(v->elements, v->max_capacity * sizeof(U64));
		v->origin = (int *) realloc(v->origin, v->max_capacity * sizeof(int));
		v->piece = (piece_t *) realloc(v->piece, v->max_capacity * sizeof(piece_t));
	} 
	v->elements[v->size] = element;
	v->origin[v->size] = origin;
	v->piece[v->size++] = type;
}

void clean_vector(Vector *v) {
	free(v->elements);
	v->elements = NULL;
	v->size = 0;
	v->max_capacity = 0;
	free(v->origin);
	v->origin = NULL;
	free(v->piece);
	v->piece = NULL;
}

void piece_display() {
	char file[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	char rank[] = {'1', '2', '3', '4', '5', '6', '7', '8'};
	U64 mask = 1;
	U64 *k = bb_lookup(true, K);
	U64 *q = bb_lookup(true, Q);
	U64 *bshp = bb_lookup(true, B);
	U64 *kn = bb_lookup(true, Kn);
	U64 *rk = bb_lookup(true, R);
	U64 *p = bb_lookup(true, P);
	U64 *nk = bb_lookup(false, K);
	U64 *nq = bb_lookup(false, Q);
	U64 *nbshp = bb_lookup(false, B);
	U64 *nkn = bb_lookup(false, Kn);
	U64 *nrk = bb_lookup(false, R);
	U64 *np = bb_lookup(false, P);

	FILE *fp;
	fp = fopen("../latest", "w+");
	int total = 0;
	char **pcs = (char **) malloc(sizeof(char *) * 64);
	char **locations = (char **) malloc(sizeof(char *) * 64);
	for (int i = 0; i < 64; i++) {
		locations[i] = (char *) malloc(sizeof(char) * 4);
		pcs[i] = (char *) malloc(sizeof(char) * 4);
	}
	for (int i = 0; i < 64; i++) {
		U64 occupied = (*k & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "wK");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*q & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "wQ");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*bshp & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "wB");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*kn & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "wKn");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*rk & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "wR");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*p & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "wP");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*nk & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "bK");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*nq & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "bQ");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*nbshp & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "bB");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*nkn & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "bKn");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*nrk & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "bR");
			sprintf(locations[total], sq);
			total++;
		}
		occupied = (*np & (mask << i));
		if (occupied) {
			char sq[2];
			sq[0] = file[i % 8];
			sq[1] = rank[i / 8];
			sprintf(pcs[total], "bP");
			sprintf(locations[total], sq);
			total++;
		}
	}
	for (int i = 0; i < total; i++) {
		fprintf(fp, "%s;", pcs[i]);
	}
	fprintf(fp, "\n");
	for (int i = 0; i < total; i++) {
		fprintf(fp, "%s;", locations[i]);
	}
	//free(pcs);
	//free(locations);
	fclose(fp);
}

void init_zobrist() {
	for (int i = 0; i < 781; i++) {
		hashes[i] = genrand64_int64();
	}
	for (int i = 0; i < 1299827; i++) {
		HashEntry *h = (HashEntry *) malloc(sizeof(HashEntry));
		h->key = 0;
		h->depth = 0;
		h->ancient = false;
		h->score = 0;
		h->valid = false;
		transpositions[i] = *h;
	}
}

U64 zobrist_hash(bool is_white) {
	U64 mask = 1;
	U64 p = *(bb_lookup(is_white, P));
	U64 rk = *(bb_lookup(is_white, R));
	U64 bshp = *(bb_lookup(is_white, B));
	U64 kn = *(bb_lookup(is_white, Kn));
	U64 q = *(bb_lookup(is_white, Q));
	U64 k = *(bb_lookup(is_white, K));
	U64 hash = 0;

	for (int i = 0; i < 64; i++) {
		U64 bit = (p & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, P) * 64 + i];
		}
		bit = (rk & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, R) * 64 + i];
		}
		bit = (bshp & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, B) * 64 + i];
		}
		bit = (kn & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, K) * 64 + i];
		}
		bit = (q & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, Q) * 64 + i];
		}
		bit = (k & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(is_white, K) * 64 + i];
		}
	}

	p = *(bb_lookup(!is_white, P));
	rk = *(bb_lookup(!is_white, R));
	bshp = *(bb_lookup(!is_white, B));
	kn = *(bb_lookup(!is_white, Kn));
	q = *(bb_lookup(!is_white, Q));
	k = *(bb_lookup(!is_white, K));

	for (int i = 0; i < 64; i++) {
		U64 bit = (p & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, P) * 64 + i];
		}
		bit = (rk & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, R) * 64 + i];
		}
		bit = (bshp & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, B) * 64 + i];
		}
		bit = (kn & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, K) * 64 + i];
		}
		bit = (q & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, Q) * 64 + i];
		}
		bit = (k & (mask << i));
		if (bit) {
			hash ^= hashes[INDEX(!is_white, K) * 64 + i];
		}
	}
	hash ^= is_white ? hashes[768] : 0;
	hash ^= wk_castle ? hashes[769] : 0;
	hash ^= wq_castle ? hashes[770] : 0;
	hash ^= bk_castle ? hashes[771] : 0;
	hash ^= bq_castle ? hashes[772] : 0;
	int index = __builtin_ffsl(p_enpassant) - 1;
	int file = index % 8;
	hash ^= hashes[773 + file]; 
	return hash;
}