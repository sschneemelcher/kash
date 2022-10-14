CC = clang
CFLAGS  = -Wall -pedantic -g -Ofast -Wno-gnu-statement-expression -Wno-gnu-empty-initializer
OBJ = main.o keys.o prompt.o run.o parse.o utils.o
VERBOSITY = 0

all: kash test

kash: $(OBJ)
	test -e build || mkdir build
	$(CC) $(CFLAGS) $(OBJ) -o build/kash 

%.o: src/%.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o

test: 
	./tests/test $(VERBOSITY)

