CC = gcc
CFLAGS = -Wall -g
OBJ = ts.o asa.o parser.o lexer.o

all:clean exec

exec:arc
	./arc algo_init.algo > algo.ram

arc :  $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -L"/usr/lib" -lfl

*.o: *.c

parser.h: parser.y

parser.c: parser.y asa.o
	bison -o $@ -d $<

lexer.c: lexer.lex parser.h
	flex -o  $@ $^

clean:
	rm -rf *~ *.o
