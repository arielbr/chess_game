#include "King.h"
#include <cmath>
#include <ctype.h>

using std::abs;
using std::pair;

//function that returns true if the king's move falls within its capabilities
//king can move up down or diagonal one square 
bool King::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {
  //move must be vertical or horizontal or diagonal, one square

  end.first = toupper(end.first);
  start.first = toupper(start.first);
  
  //check if the end position is on the board
  if((end.first < 'A') or (end.first > 'H') or (end.second < '1') or (end.second > '8')){
    return false;
  }
  
  //vertical move
  if (start.first == end.first && (abs(start.second - end.second) == 1)) {
    return true;
  //horizontal move    
  } else if (start.second == end.second && (abs(start.first - end.first) == 1)) {
      return true;
  //diagonal move  
  } else if (abs(start.first - end.first) == 1 && abs(start.second - end.second) == 1) {
      return true;
  //move was invalid
  } else {
      return false;
  }
}
