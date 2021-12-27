CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = -lm
all: banhammer

banhammer:  banhammer.o speck.o ht.o bst.o node.o bf.o bv.o parser.o  
	$(CC) $(CFLAGS) -o banhammer banhammer.o speck.o ht.o bst.o node.o bf.o bv.o parser.o $(LFLAGS) 
banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c
speck.o: speck.c speck.h
	$(CC) $(CFLAGS) -c speck.c
ht.o: ht.c ht.h
	$(CC) $(CFLAGS) -c ht.c
bst.o: bst.c bst.h 
	$(CC) $(CFLAGS) -c bst.c 
bf.o: bf.c bf.h 
	$(CC) $(CFLAGS) -c bf.c  
parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c
bv.o:	bv.c bv.h
	$(CC) $(CFLAGS) -c bv.c 
clean:
	rm -f banhammer *.o
	
format:
	clang-format -i -style=file *.[ch]



