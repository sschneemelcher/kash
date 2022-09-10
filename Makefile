CC = clang
CFLAGS  = -Wall -g -Ofast
OBJ = main.o keys.o prompt.o run.o parse.o

all: kash

kash: $(OBJ)
	test -e build || mkdir build
	$(CC) $(CFLAGS) $(OBJ) -o build/kash 
	rm *.o

%.o: src/%.c
	$(CC) $(CFLAGS) -c $^
