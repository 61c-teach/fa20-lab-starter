CC=gcc
LD=gcc
CFLAGS=-ggdb -Wall -pedantic -std=gnu99 -O3
LDFLAGS=

EX2_PROG=matrixMultiply

EX3_OBJS=transpose.o test_transpose.o
EX3_PROG=transpose

ex2:
		$(CC) -o $(EX2_PROG) $(CFLAGS) $(EX2_PROG).c
			./$(EX2_PROG)

ex3: $(EX3_OBJS)
		$(CC) -o $(EX3_PROG) $(CFLAGS) $(EX3_OBJS)

clean:
		-rm -rf core *.o *~ "#"*"#" Makefile.bak $(PROGS) *.dSYM

