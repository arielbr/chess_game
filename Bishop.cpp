#include "Bishop.h"
#include <cmath>
#include <ctype.h>

using std::abs;
using std::pair;

//function that returns true if the bishop's move falls within its capabilities
//bishop can move diagonally any number of spaces
bool Bishop::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {

  end.first = toupper(end.first);
  start.first = toupper(start.first);

  //check if the end position is on the board           
  if((end.first < 'A') or (end.first > 'H') or (end.second < '1') or (end.second > '8')){
    return false;
  }

  //diagonal non-zero move
  if((abs(start.first - end.first) == abs(start.second - end.second)) and (abs(start.first - end.first) != 0)) {
    return true;
  }

  //invalid move
  return false;
}

  
