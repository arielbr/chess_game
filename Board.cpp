#include <iostream>
#include <utility>
#include <string>
#include <map>
#include "Board.h"
#include "Terminal.h"
#include "CreatePiece.h"
#include <vector> //added
#include "Piece.h"
#include "CreatePiece.h"

using std::cerr;
using std::endl;
using std::cout;
using std::string;
using std::pair;
using std::map;
using std::vector;
using std::make_pair;

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Board::Board(){}

//Board destructor, deletes piece pointers from board
Board::~Board() {
   for(map<pair<char,char>, Piece*>::const_iterator it = occ.cbegin();
      it != occ.cend();
      ++it) {
    if(it->second) {
      delete it->second;
    }
  }
}
 
// Returns a const pointer to the piece at a prescribed location if it exists,
// or nullptr if there is nothing there. 
const Piece* Board::operator()(std::pair<char, char> position) const {
  try {
    Piece* piece = occ.at(position);
    return piece;
  } catch (const std::out_of_range& oor) {
    
    return NULL;
  }
}

// Returns false if:
// -- the designator is invalid,
// -- the specified location is not on the board, or
// -- if the specified location is occupied
bool Board::add_piece(std::pair<char, char> position, char piece_designator) {
  //test if the designator is valid
  if(piece_designator == '-') {
    return false;
  }
  
  std::vector<char> piece_type = {'K','k','Q','q','B','b','N','n','R','r','P','p','M','m'};
  bool found = false;
  for (std::vector<char>::const_iterator it = piece_type.cbegin();
       it != piece_type.cend();
       it++) {
    if (*it == piece_designator) {
      found = true;
      break;
    }
  }
  if (found == false) {
    cerr << "Not a valid piece." << endl;
    return false;
  }
  
  //test if the specified location is on board
  position.first = toupper(position.first);
  if (position.first > 'H' || position.first < 'A' || position.second > '8' || position.second < '1') {
    cerr << "Coordinates not on board." << endl;
    return false;
  }

  //check if the specified is occupied
  for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.cbegin();
       it != occ.cend();
       it++) {
    if (it->first == position) {
      cerr << "There is a piece already at this position." << endl;
      return false;
    }
  }
  
  occ[position] = create_piece(piece_designator);
  return true;
}

// Deletes the piece at the specified position  
bool Board::delete_piece(std::pair<char, char> position) {
  map<pair<char, char>, Piece*>::iterator piece = occ.find(position);

  if (piece != occ.end() and piece->second != nullptr) {
    delete piece->second;
    occ.erase(piece);
    return true;
  } 
  return false;
}

// Records squares a piece moves through in a move given the start/end locations
// Returns the squares as a vector of pairs       
std::vector<std::pair<char, char>> Board::get_path(std::pair<char,char> start, std::pair< char , char > end) const {

  vector<pair<char,char>> path;
  
  //horizontal move
  if (start.second == end.second) {
    if(start.first > end.first) { //moving left
      for(char i = end.first + 1; i < start.first; i++) {
	pair<char,char> pos = std::make_pair(i,start.second);
	path.push_back(pos);
      }
    } else if (start.first < end.first) { //moving right
      for (char i = start.first + 1; i < end.first; i++) {
	pair<char,char> pos = std::make_pair(i,start.second);
	path.push_back(pos);
      }
    }
  }

  //vertical move
  if (start.first == end.first) {
    if (start.second > end.second) { //moving down
      for(char i = end.second + 1; i < start.second; i++) {
	pair<char,char> pos = std::make_pair(start.first,i);
	path.push_back(pos);
      }
    } else if (start.second < end.second) { //moving up
      for (char i = start.second + 1; i < end.second; i++) {
	pair<char,char> pos = std::make_pair(start.first,i);
	path.push_back(pos);
      }
    }
  }

  //diagonal move
  int rowChange = abs(start.second - end.second);
  int colChange = abs(start.first - end.first);
  int distance = 1;
  
  int rowIncrement = 1;
  int colIncrement = 1;
  char loopStart = end.first;
  char loopEnd = start.first;
  
  if (start.second < end.second) {
    rowIncrement = -1;
  }
  if (start.first < end.first) {
    colIncrement = -1;
    loopStart = start.first;
    loopEnd = end.first;
  }
  
  if (rowChange == colChange) {
    for (char i = loopStart + 1; i < loopEnd; i++) {
	pair<char,char> pos = std::make_pair(end.first + distance*colIncrement, end.second + distance*rowIncrement);
	path.push_back(pos);
	++distance;
      }
  }
  return path;
}

// Returns true if the path of a piece is clear 
bool Board::check_clear_path( std::pair< char , char > start , std::pair< char , char > end ) const {

  //a knight will always have a clear path because it can jump over pieces
 
  vector<pair<char,char>> path = get_path(start,end);
  
  if (start.first == end.first || start.second == end.second || abs(start.first - end.first) == abs(start.second - end.second)) {
    for(vector<pair<char,char>>::iterator it = path.begin(); it !=path.end(); it++) {
      map<pair<char,char>, Piece*>::const_iterator itt = occ.find(*it);
      if (itt != occ.end()) {
	return false;
      }
    }
  }
  return true;
  
}

// Returns true if the board has the right number of kings on it  
bool Board::has_valid_kings() const {
  //should have two kings before game ends
  int count_king = 0;
  for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.cbegin();
       it != occ.cend();
       ++it) {
    if ((it->second)->to_ascii() == 'K' || (it->second)->to_ascii() == 'k') {
      ++count_king;
    }
  }
  if (count_king != 2) {return false;}
	return true;
}    

// Gets the location of the king, given the team
std::pair< char , char > Board::get_king_position( bool white ) const { 
  char c = 'k';
  if (white){
    c = 'K';
  }
   
  std::pair<char, char> king;
  for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.cbegin();
       it != occ.cend();
       it++) {
    Piece* piece = it->second;
    if (piece->to_ascii() == c) {
      king = it->first;
    }
  }
  return king;
}

// Displays the board by printing it to stdout    
void Board::display() const {
  cout << "\n A  B  C  D  E  F  G  H\n";
  bool white = false;
  std::pair<char,char> piece_position;
  
  for(char row = '8'; row >= '1'; row--) {
    white = !white;
    for(char col = 'A'; col <= 'H'; col++) {
      piece_position.first = col;
      piece_position.second = row;
      map<pair<char, char>, Piece*>::const_iterator piece = occ.find(piece_position);
        if (white) {
          Terminal::color_bg(Terminal::WHITE);
        } else {
          Terminal::color_bg(Terminal::GREEN);
        }
        if (piece != occ.end()) {	
	  cout << " " << unicode(piece->second->to_ascii()) << " ";
	} else {
	  cout << "   ";
	}
        white = !white;
    }
    Terminal::set_default();
    cout <<  " " << row << endl;
  }
}

// Gets the unicode character for the chess piece from the ascii piece designator
std::string Board::unicode(char ascii) const {
  switch(ascii)
  {
    case 'K':
    return "\u2654";
    break;
    case 'k':
    return "\u265A";
    break;

    case 'Q':
    return "\u2655";
    break;
    case 'q':
    return "\u265B";
    break;

    case 'R':
    return "\u2656";
    break;
    case 'r':
    return "\u265C";
    break;

    case 'B':
    return "\u2657";
    break;
    case 'b':
    return "\u265D";
    break;

    case 'N':
    return "\u2658";
    break;
    case 'n':
    return "\u265E";
    break;

    case 'P':
    return "\u2659";
    break;
    case 'p':
    return "\u265F";
    break;

  case 'M':
    return "\u2606";
      break;
  case 'm':
    return "\u2605";
      break;
    
    default:
    return "-";
    break;
  }
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
// Write the board state to an output stream 
std::ostream& operator<<(std::ostream& os, const Board& board) {
	for(char r = '8'; r >= '1'; r--) {
		for(char c = 'A'; c <= 'H'; c++) {
			const Piece* piece = board(std::pair<char, char>(c, r));
			if (piece) {
				os << piece->to_ascii();
			} else {
			  os << '-';
			}
		}
		os << std::endl;
	}
	return os;
}
