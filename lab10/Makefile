CC=gcc
CFLAGS= -std=gnu99 -c -fopenmp
LDFLAGS=-fopenmp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLES=v_add dotp hello server_basic server_process

.PHONY: all clean

all: $(EXECUTABLES)

v_add: v_add.o omp_app.o libbmp.o
hello: hello.o
dotp: dotp.o omp_app.o libbmp.o
server_basic: server_basic.o libhttp.o omp_apps.o server_utils_serial.o libbmp.o
server_process: server_process.o libhttp.o omp_apps.o server_utils_process.o libbmp.o

$(EXECUTABLES):
	$(CC) $(LDFLAGS) $^ -o $@

server_basic.o: server.c libhttp/libhttp.c omp_apps.c server_utils.c libbmp/libbmp.c
server_process.o: server.c libhttp/libhttp.c omp_apps.c server_utils.c libbmp/libbmp.c
omp_app.o: omp_apps.c libbmp/libbmp.c

server_utils_serial.o: server_utils.c
	$(CC) $(CFLAGS) $< -o $@
server_utils_process.o: server_utils.c
	$(CC) -DPROC $(CFLAGS) $< -o $@
server_process.o:
	$(CC) $(CFLAGS) $< -o $@
server_basic.o:
	$(CC) $(CFLAGS) $< -o $@

libbmp.o:
	$(CC) $(CFLAGS) libbmp/libbmp.c -o $@
libhttp.o:
	$(CC) $(CFLAGS) libhttp/libhttp.c -o $@

omp_app.o: omp_apps.c libbmp.o
	$(CC) $(CFLAGS) omp_apps.c -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLES) *.o
	rm -f ./files/*_sobel.bmp
