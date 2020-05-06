#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include "Piece.h"
#include "Board.h"

using std::map;
using std::pair;
using std::vector;

class Chess {

public:
	// This default constructor initializes a board with the standard
	// piece positions, and sets the state to white's turn
	Chess();

        // copy constructor
        Chess(const Chess& c);
  
	// Returns a constant reference to the board 
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	const Board& get_board() const { return board; }

	// Returns true if it is white's turn
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	bool turn_white() const { return is_white_turn; }

	// Attempts to make a move. If successful, the move is made and
	// the turn is switched white <-> black
	bool make_move(std::pair<char, char> start, std::pair<char, char> end);

	// Returns true if the designated player is in check
	bool in_check(bool white) const;

        // Returns true if king can make at least one legal move without moving into check
        bool can_king_move_all(bool white) const;

        // Returns true if king can move from start to end without moving into check
        bool can_king_move_one(std::pair<char,char> start, std::pair<char,char> end) const;	  

        // Returns a vector of all pieces (position and Piece*) that can capture the piece at the given position in one move
        std::vector<std::pair<std::pair<char,char>, const Piece*>> get_attackers(bool white, std::pair<char,char> position) const;
  
        // Returns true if the designated player is in mate
	bool in_mate(bool white) const;

        // Returns true if the designated player is in stalemate
	bool in_stalemate(bool white) const;

        // Sets the turn given the character turn indicator 
        void set_white_turn(char c);

        // Reads the board in from a stream
        friend std::istream& operator>> (std::istream& is, Chess& chess);
  
private:
	// The board
	Board board;

	// Is it white's turn?
	bool is_white_turn;

};

// Writes the board out to a stream
std::ostream& operator<< (std::ostream& os, const Chess& chess);

#endif // CHESS_H
