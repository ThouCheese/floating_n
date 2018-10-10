CC=g++
CFLAGS=-o program -Wall -std=c++17 -pedantic
BIN=program

all: 
	$(CC) main.cpp -o $(BIN) $(CFLAGS)

clean: 
	rm -f *.o
	rm $(BIN)
