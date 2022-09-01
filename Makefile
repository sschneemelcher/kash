CC = clang
CFLAGS  = -Wall -g -Ofast
OBJ = msh.o

all: $(OBJ)
	test -e ./bin || mkdir ./bin
	$(CC) $(CFLAGS) -o bin/msh  $(OBJ) 
	rm $(OBJ)

msh.o: src/msh.c
	$(CC) $(CFLAGS) -c $<

