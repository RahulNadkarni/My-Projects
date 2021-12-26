CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = -lm

all: encode decode

encode: encode.o pq.o code.o stack.o huffman.o stats.o node.o io.o 
	$(CC) $(CFLAGS) -o encode code.o encode.o stats.o pq.o stack.o huffman.o node.o io.o $(LFLAGS)
decode: decode.o pq.o stack.o huffman.o stats.o node.o io.o code.o
	$(CC) $(LFLAGS) -o decode decode.o code.o pq.o stats.o stack.o huffman.o node.o io.o 
encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c
decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c
pq.o: pq.c pq.h 
	$(CC) $(CFLAGS) -c pq.c stats.c
stack.o: stack.c stack.h 
	$(CC) $(CFLAGS) -c stack.c 
node.o: node.c node.h 
	$(CC) $(CFLAGS) -c node.c 
stats.o: stats.c stats.h
	$(CC) $(CFLAGS) -c stats.c
io.o: io.c io.h 
	$(CC) $(CFLAGS) -c io.c 
huffman.o: huffman.c huffman.h 
	$(CC) $(CFLAGS) -c huffman.c	
code.o: code.c code.h
	$(CC) $(CFLAGS) -c code.c
clean:
	rm -f enode *.o
	rm decode
format:
	clang-format -i -style=file *.[ch]


