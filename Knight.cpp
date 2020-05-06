#include "Knight.h"
#include <cmath>
#include <ctype.h>

using std::abs;
using std::pair;

//function that returns true if knight move is within its capabilities
//knight can move in a 2-1 or 1-2 L shape
//not all shapes have to be covered because some overlap...
bool Knight::legal_move_shape(std::pair<char, char> start, std::pair<char, char> end) const {

  end.first = toupper(end.first);
  start.first = toupper(start.first);

  //check if move is on the board   
  if((end.first < 'A') or (end.first > 'H') or (end.second < '1') or (end.second > '8')){
    return false;
  }
  
  //L shape starting with 2 hops horizontally
  if (abs(start.first - end.first) == 2) {
    if (abs(start.second - end.second) == 1) {
	return true;
    }
  } 

  //L shape starting with 2 hops vertically
  if (abs(start.second - end.second) == 2) {
    if (abs(start.first - end.first) == 1) {
      return true;
    }
  }

  //invalid move
  return false;
}


  
