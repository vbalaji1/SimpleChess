CC=gcc
CFLAGS=-I$(IDIR)

IDIR=../include
ODIR=obj

_DEPS = utilities.h bitboard.h pawn.h mvgen.h state.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
 
_OBJ = main.o utilities.o bitboard.o pawn.o mvgen.o state.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

viper: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	./viper

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~