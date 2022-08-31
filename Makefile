CC = clang
CFLAGS  = -Wall -g -Ofast
OBJ = msh.o

all: $(OBJ)
	mkdir ./bin
	$(CC) $(CFLAGS) -o bin/msh  $(OBJ) 

msh.o: src/msh.c
	$(CC) $(CFLAGS) -c $<

