CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)


chess: main.o Board.o Chess.o CreatePiece.o Pawn.o Knight.o Bishop.o Queen.o King.o Rook.o mystery.h
	$(CC) -o chess main.o Board.o Chess.o CreatePiece.o Pawn.o Knight.o Bishop.o Queen.o King.o Rook.o $(CFLAGS)

Board.o: Board.cpp Bishop.h Board.h King.h Knight.h Mystery.h Pawn.h Piece.h CreatePiece.h Queen.h Rook.h Terminal.h
	$(CC) -c Board.cpp $(CFLAGS)

Chess.o: Chess.cpp Board.h Chess.h Piece.h Mystery.h
	$(CC) -c Chess.cpp $(CFLAGS)

Pawn.o: Pawn.cpp Pawn.h
	$(CC) -c Pawn.cpp $(CFLAGS)

Knight.o: Knight.cpp Knight.h
	$(CC) -c Knight.cpp $(CFLAGS)

Bishop.o: Bishop.cpp Bishop.h
	$(CC) -c Bishop.cpp $(CFLAGS)

Rook.o: Rook.cpp Rook.h
	$(CC) -c Rook.cpp $(CFLAGS)

Queen.o: Queen.cpp Queen.h
	$(CC) -c Queen.cpp $(CFLAGS)

King.o: King.cpp King.h
	$(CC) -c King.cpp $(CFLAGS)

CreatePiece.o: CreatePiece.cpp Bishop.h Board.h Chess.h King.h Knight.h Mystery.h Pawn.h Piece.h CreatePiece.h Queen.h Rook.h
	$(CC) -c CreatePiece.cpp $(CFLAGS)

main.o: main.cpp Bishop.h Board.h Chess.h King.h Knight.h Mystery.h Pawn.h Piece.h CreatePiece.h Queen.h Rook.h Mystery.h
	$(CC) -c main.cpp $(CFLAGS)

clean:
	rm -f *.o chess
