CC=gcc
CFLAGS=-Wall -g

OBJS=main.o lexer.o parser.o executor.o

myshell: $(OBJS)
	$(CC) -o myshell $(OBJS)

clean:
	rm -f *.o myshell
