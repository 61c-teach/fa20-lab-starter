CC=gcc
CFLAGS=--std=c99 -Wall

simd: simd.o common.o
	$(CC) $(CFLAGS) -o simd simd.o common.o

simd.o: simd.c common.h
	$(CC) $(CFLAGS) -c simd.c

common.o: common.c common.h
	$(CC) $(CFLAGS) -c common.c

clean:
	rm -f simd
	rm -f *.o
