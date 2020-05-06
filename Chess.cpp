#include "Chess.h"
#include <utility>
#include <vector>

using std::map;
using std::pair;
using std::vector;
using std::cout;
using std::endl;
using std::make_pair;


/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
Chess::Chess() : is_white_turn(true) {
	// Add the pawns
	for (int i = 0; i < 8; i++) {
		board.add_piece(std::pair<char, char>('A' + i, '1' + 1), 'P');
		board.add_piece(std::pair<char, char>('A' + i, '1' + 6), 'p');
	}

	// Add the rooks
	board.add_piece(std::pair<char, char>( 'A'+0 , '1'+0 ) , 'R' );
	board.add_piece(std::pair<char, char>( 'A'+7 , '1'+0 ) , 'R' );
	board.add_piece(std::pair<char, char>( 'A'+0 , '1'+7 ) , 'r' );
	board.add_piece(std::pair<char, char>( 'A'+7 , '1'+7 ) , 'r' );

	// Add the knights
	board.add_piece(std::pair<char, char>( 'A'+1 , '1'+0 ) , 'N' );
	board.add_piece(std::pair<char, char>( 'A'+6 , '1'+0 ) , 'N' );
	board.add_piece(std::pair<char, char>( 'A'+1 , '1'+7 ) , 'n' );
	board.add_piece(std::pair<char, char>( 'A'+6 , '1'+7 ) , 'n' );

	// Add the bishops
	board.add_piece(std::pair<char, char>( 'A'+2 , '1'+0 ) , 'B' );
	board.add_piece(std::pair<char, char>( 'A'+5 , '1'+0 ) , 'B' );
	board.add_piece(std::pair<char, char>( 'A'+2 , '1'+7 ) , 'b' );
	board.add_piece(std::pair<char, char>( 'A'+5 , '1'+7 ) , 'b' );

	// Add the kings and queens
	board.add_piece(std::pair<char, char>( 'A'+3 , '1'+0 ) , 'Q' );
	board.add_piece(std::pair<char, char>( 'A'+4 , '1'+0 ) , 'K' );
	board.add_piece(std::pair<char, char>( 'A'+3 , '1'+7 ) , 'q' );
	board.add_piece(std::pair<char, char>( 'A'+4 , '1'+7 ) , 'k' );
}

// copy constructor 
Chess::Chess(const Chess& c) {
  is_white_turn = c.is_white_turn;

  map<pair<char,char>, Piece*> map_to_copy = c.board.get_map();

  for (map<pair<char,char>, Piece*>::iterator it = map_to_copy.begin(); it != map_to_copy.end(); it++) {
    pair<char,char> position = it->first;
    Piece* piece = it->second;
    board.delete_piece(position);
    board.add_piece(position, piece->to_ascii());
  }
  
}

// Attempts to make a move. If successful, the move is made and                     
// the turn is switched white <-> black  
bool Chess::make_move(std::pair<char, char> start, std::pair<char, char> end){
  bool is_legal = false;
  char piece_type;
  // overload () operator to see if a piece exists on start and end squares
  const Piece* start_piece = board(start);
  const Piece* end_piece = board(end);

  // there isn't a piece at the start square
  if (start_piece == nullptr) {
    return false;

  // the piece is the wrong color
  } else if (is_white_turn != start_piece->is_white()) {
    return false;

  // start piece is valid
  } else {
    piece_type = start_piece->to_ascii();
  }

  //Check if the king can move
  if(piece_type == 'K' or piece_type == 'k') {
    if(can_king_move_one(start,end)){
	board.add_piece(end,piece_type);
	board.delete_piece(start);
      }
      else {
	return false;
      }
  }

    // there is a piece at the end location
    if(end_piece){
      // the piece is an opponent's piece
      if(end_piece->is_white() != start_piece->is_white()){
	// the move is a legal capture move
	if(start_piece->legal_capture_shape(start,end)){
	  if(board.check_clear_path(start,end)){ 
	  // capture
	  board.delete_piece(end);
	  // piece promotion white pawn to queen
	  if(piece_type == 'P' and end.second == '8') {
	    board.add_piece(end, 'Q');
	  // piece promotion black pawn to queen
	  } else if (piece_type == 'p' and end.second == '1') {
	    board.add_piece(end, 'q');
	  // no piece promotion, just move piece
	  } else {
	    board.add_piece(end, piece_type);
	  }
	  board.delete_piece(start);
	  // the move put the player in check, undo.
	  if (in_check(is_white_turn)){
	    board.delete_piece(end);
	    board.add_piece(start, piece_type);
	    return false;
	  }
          is_legal = true;
	  }
	}
      // the end location piece was not an opponent's piece
      } else {
	return false;
      }
    // there is no piece at the end location
    } else {
     // the move is a legal move shape
      if(start_piece->legal_move_shape(start, end)){
	if(board.check_clear_path(start,end)){
	board.delete_piece(start);
	// piece promotion white pawn to queen
	if(piece_type == 'P' and end.second == '8') {
	  board.add_piece(end, 'Q');
	// piece promotion black pawn to queen
	} else if (piece_type == 'p' and end.second == '1'){
	  board.add_piece(end, 'q');
	// no piece promotion, just move piece
	} else {
	  board.add_piece(end, piece_type);
	}
	// the move put the player in check, undo.
	if(in_check(is_white_turn)){
	  board.delete_piece(end);
	  board.add_piece(start, piece_type);
	  return false;
	}
	is_legal = true;
	}
      }
    }
  
	  
  // switch turns if a legal move was made
  if (is_legal) {
    is_white_turn = !(is_white_turn);
    return true;
  }
  return false;
}

// Returns true if the designated player is in check 
bool Chess::in_check(bool white) const {
  pair<char,char> king = board.get_king_position(white);

  //gets vector of pieces that can attack the king in one move
  vector<pair<pair<char,char>, const Piece*>> attackers = get_attackers(white, king);

  //if there are no attackers (vector is empty), king is not in check
  if (attackers.empty()) {
    return false;
    //if there are attackers, king is not in check
  } else {
    return true;
  }
}

// Returns true if king can move from start to end without moving into check
bool Chess::can_king_move_one(std::pair<char,char> start, std::pair<char,char> end) const {
  const Piece* king = board(start);
  const Piece* end_piece = board(end);
  bool white = king->is_white();
  bool on_board = false;

  // check if end location is on board
  for(char row = '1'; row <= '8'; row++) {
    for(char col = 'A'; col <= 'H'; col++){
      pair<char,char> position = make_pair(col,row);
      if(position == end){
	on_board = true;
      }
    }
  }
  // end location  was not on the board
  if(on_board == false) {
    return false;
  }

  std::vector<pair<pair<char,char>, const Piece*>> attackers;
  
  // there is a piece where the king is moving
  if(end_piece){
    // the piece is the opposing team's
    if(end_piece->is_white() != white) {
      attackers = get_attackers(white, end);
      // there are no pieces attacking the end spot, king can move there
      if(attackers.empty()){
	return true;
      // there are pieces attacking end spot, king can't move there
      } else {
	return false;
      }
    // the end piece is on the king's team 
    } else {
      return false;
    }
  // there is no piece where the king is moving 
  } else {
    attackers = get_attackers(white,end);
    // there are no pieces attacking end spot, king can move there
    if(attackers.empty()){
      return true;
    // there are pieces attacking end spot, king cannot move there 
    } else {
      return false;
    }
  }
}

// Returns true if king can make at least one legal move without moving into check
bool Chess::can_king_move_all(bool white) const {
  pair<char,char> king = board.get_king_position(white);

  // make a vector with all the possible king moves
  // up, down, left, right, 4 diagonals
  vector<pair<char,char>> positions;
  pair<char,char> att1 = make_pair(king.first - 1, king.second + 1);
  positions.push_back(att1);
  pair<char,char> att2 = make_pair(king.first, king.second + 1);
  positions.push_back(att2);
  pair<char,char> att3 = make_pair(king.first + 1, king.second + 1);
  positions.push_back(att3);
  pair<char,char> att4 = make_pair(king.first - 1, king.second);
  positions.push_back(att4);
  pair<char,char> att6 = make_pair(king.first + 1, king.second);
  positions.push_back(att6);
  pair<char,char> att7 = make_pair(king.first - 1, king.second - 1);
  positions.push_back(att7);
  pair<char,char> att8 = make_pair(king.first, king.second - 1);
  positions.push_back(att8);
  pair<char,char> att9 = make_pair(king.first + 1, king.second - 1);
  positions.push_back(att9);

  //for each possible move, check if the king can make the move using can_king_move_onw function
  for(vector<pair<char,char>>::iterator it = positions.begin(); it != positions.end(); ++it) {
    if(can_king_move_one(king, *it)) {
      return true;
    }
  }
  return false;
}

// Returns a vector of all pieces (position and Piece*) that can capture the piece at the given position in one move
std::vector<pair<pair<char,char>, const Piece*>> Chess::get_attackers(bool white, pair<char,char> position) const {
  
  vector< pair<pair<char,char> , const Piece* > > attackers;
  map<pair<char,char>, Piece*> board_map = board.get_map();
  
  //iterates through all pieces in map
  for (map<pair<char,char>, Piece*>::iterator it = board_map.begin(); it != board_map.end(); it++) {   
    const Piece* team_piece = it->second;
    pair<char,char> team_piece_pos = it->first;
    //if piece is a pawn
    if(team_piece->to_ascii() == 'P' || team_piece->to_ascii() == 'p') {
      //check if pawn is on opposing team, can legally move to capture piece, and has a clear path
      if(team_piece->is_white() != white && team_piece->legal_capture_shape(team_piece_pos, position) &&
	 board.check_clear_path(team_piece_pos, position)) {
	//if so, create a pair of its location and object pointer, add to the vector of attackers
	pair<pair<char,char>, const Piece*> piece_obj = make_pair(team_piece_pos, team_piece);
	attackers.push_back(piece_obj);
      }
    } else { //if piece is not a pawn
      //do the same thing (with legal_move_shape instead of legal_capture_shape)
      if(team_piece->is_white() != white && team_piece->legal_move_shape(team_piece_pos, position) &&
         board.check_clear_path(team_piece_pos, position)) {
	pair<pair<char,char>, const Piece*> piece_obj = make_pair(team_piece_pos, team_piece);
        attackers.push_back(piece_obj);
      }
    }
  }
  return attackers;
}

// Returns true if the designated player is in mate
bool Chess::in_mate(bool white) const {
  pair<char,char> king_pos = board.get_king_position(white);

  //if the king is not in check or the king can move, that king can not be in check mate
  if (!in_check(white) or can_king_move_all(white)) {
    return false;
  }

  //get a vector of pieces that can take the king in one move
  vector< pair<pair<char,char> , const Piece* > > attackers = get_attackers(white, king_pos);
  map<pair<char,char>, Piece*> pieces = board.get_map();
  
  //iteration through every attacker that can eat the king in one move
  for(vector< pair<pair<char,char> , const Piece* >>::iterator it = attackers.begin(); it != attackers.end(); it++) {
    pair<char,char> attacker_pos = it->first;
    //const Piece* attack = it->second;
    //iteration through every piece on the map, call them blocks
    for (map<pair<char,char>, Piece*>:: iterator itt = pieces.begin(); itt != pieces.end(); itt++) {
      pair<char,char> block_pos = itt->first;
      const Piece* block = itt->second;

      //check if piece/block can eat attacking piece in one move, if so king is not in check mate
      if(block->to_ascii() == 'P' || block->to_ascii() == 'p') {
	if(block->is_white() == white && block->legal_capture_shape(block_pos, attacker_pos) && board.check_clear_path(block_pos, attacker_pos)) {
	  return false;
	}
      } else {
	if(block->is_white() == white && block->legal_move_shape(block_pos, attacker_pos) && board.check_clear_path(block_pos, attacker_pos)) {
	  if(block->to_ascii() != 'K' && block->to_ascii() != 'k') {
	    return false;
	  }
	}
      }
      
      //check if piece/block can move into the path between attacker and king

      if(attacker_pos.first == king_pos.second or attacker_pos.second == king_pos.second or
	       abs(attacker_pos.first - king_pos.first) == abs(attacker_pos.second - king_pos.second)) {
	
      //get a vector of positions between the attacking piece and the king
      vector<pair<char,char>> path_to_king = board.get_path(attacker_pos, king_pos);
      //iterate through each position in teh path between attacking piece and the king
      for(vector<pair<char,char>>::iterator ittt = path_to_king.begin(); ittt != path_to_king.end(); ittt++){
	//if the piece/block is not a king and can legally move to the position, king is not in check mate
	//(piece/block can block the attacker's path to king)
	if(block->to_ascii() != 'K' && block->to_ascii() != 'k') {
	  if(block->is_white() == white && block->legal_move_shape(block_pos, *ittt) && board.check_clear_path(block_pos, *ittt)) {
	    return false;
	  }
	}
      }
      }
    }
  }
  return true;
}

// Returns true if the designated player is in stalemate
bool Chess::in_stalemate(bool white) const {
  // check if any player is in checked position
  if (in_check(white) or can_king_move_all(white)) {
    return false;
  }
  std::map<std::pair<char, char>, Piece*> occ = board.get_map();
  
  //create a map, key: each piece's position on board, value: vector of position to move to
  std::map<std::pair<char, char>, std::vector<std::pair<char, char>>> all_next_pos; 
  //check if legal movement is available for each existing piece
  for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.cbegin();
       it != occ.cend();
       it++) {
    std::pair<char, char> cur_location = it->first;
    //collect vectors of legal move shape or capture shape for each piece
    std::pair<char, char> end;

    //only collect and check pieces of current player
    if (it->second->is_white() == white) {
      for (char i = 'A'; i <= 'H'; i++) {
	end.first = i;
	for (char j = '1'; j <= '8'; j++) {
	  end.second = j;
	  //there is a piece at the end
      	  if (board(end) != nullptr) {
	    if (board(end)->is_white() != it->second->is_white() && it->second->legal_capture_shape(it->first, end)) {
	      //check the piece at the end is from the opponent
	      all_next_pos[cur_location].push_back(end);
	    }
	  }
	  // if no piece is there
	  else {
	    if (it->second->legal_move_shape(it->first, end)) {
	      all_next_pos[cur_location].push_back(end);
	    }
	  }
	}
      }  
    }
  }

  // For all legal moves, check if there are any that don't put the player in check
  for (std::map<std::pair<char, char>, std::vector<std::pair<char, char>>>::iterator it = all_next_pos.begin();
       it != all_next_pos.end();
       ++it) {
    // iterate through all possible positions for each piece on the board
    if ((int)it->second.size() != 0) {
      for (int i = 0; i < (int)it->second.size(); i++) {
	Chess temp(*this);
      	temp.board.add_piece(it->second[i], temp.board(it->first)->to_ascii());
	temp.board.delete_piece(it->first);

	// There is still a legal move not causing check available, not stalemate
      	if (!temp.in_check(white)) {
	  return false;
       	}
      }
    }
  }
  return true;
}

/////////////////////////////////////
// DO NOT MODIFY THIS FUNCTION!!!! //
/////////////////////////////////////
// Writes the board out to a stream 
std::ostream& operator<< (std::ostream& os, const Chess& chess) {
	// Write the board out and then either the character 'w' or the character 'b',
	// depending on whose turn it is
	return os << chess.get_board() << (chess.turn_white() ? 'w' : 'b');
}

// Reads the board in from a stream
std::istream& operator>> (std::istream& is, Chess& chess) {
  char piece_char;
  for (char r = '8'; r >= '1'; r--) {
    for (char c = 'A'; c <= 'H'; c++) {
      is >> piece_char;
      pair<char,char> position  = make_pair(c,r);
      chess.board.delete_piece(position);
      chess.board.add_piece(position, piece_char);
    }
  }
  
  is >> piece_char;
  chess.set_white_turn(piece_char);
 
  return is;
}

// Sets the turn given the character turn indicator
void Chess::set_white_turn(char c) {
  if (c == 'w') {
    is_white_turn = true;
  }
  if (c == 'b') {
    is_white_turn = false;
  }
}
