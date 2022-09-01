CC = clang
CFLAGS  = -Wall -g -Ofast
OBJ = shell.o

all: $(OBJ)
	test -e ./bin || mkdir ./bin
	$(CC) $(CFLAGS) -o bin/kash  $(OBJ) 
	rm $(OBJ)

shell.o: src/shell.c
	$(CC) $(CFLAGS) -c $<

