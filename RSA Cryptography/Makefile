CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp) 

all: encrypt decrypt keygen

encrypt: encrypt.o numtheory.o randstate.o rsa.o
	$(CC) $(CFLAGS) encrypt.c numtheory.c randstate.c rsa.c -o encrypt -lm $(LFLAGS) 
decrypt: decrypt.o numtheory.o randstate.o rsa.o 
	$(CC) $(CFLAGS) decrypt.c numtheory.c randstate.c rsa.c -o decrypt -lm $(LFLAGS) 
keygen: keygen.o numtheory.o randstate.o rsa.o
	$(CC) $(CFLAGS) keygen.c numtheory.c randstate.c rsa.c -o keygen -lm $(LFLAGS)
encrypt.o: encrypt.c 
	$(CC) $(CFLAGS) -c encrypt.c 
decrypt.o: decrypt.c 
	$(CC) $(CFLAGS) -c decrypt.c 

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c 

randstate.o: randstate.c randstate.h 
	$(CC) $(CFLAGS) -c randstate.c 
numtheory.o: numtheory.c numtheory.h 
	$(CC) $(CFLAGS) -c numtheory.c 

rsa.o: rsa.c rsa.h 
	$(CC) $(CFLAGS) -c rsa.c 

clean:
	rm -f encrypt *.o
	rm decrypt 
	rm keygen
format:
	clang-format -i -style=file *.[ch]

