CC=g++
CFLAGS=-o program -Wall -std=c++17 -pedantic -fconstexpr-depth=100
BIN=program

all: 
	$(CC) main.cpp -o $(BIN) $(CFLAGS)

clean: 
	rm -f *.o
	rm $(BIN)
