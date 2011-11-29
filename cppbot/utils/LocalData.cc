#include "LocalData.h"
#include "State.h"
#include "Location.h"
#include "utils.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void LocalData::setup(State& s) {
	num_rows = s.rows;
	num_cols = s.cols;
	last_seen = new float*[s.rows];
  moved_to = new bool*[s.rows];
	freed = new bool*[s.rows];
  for (int rownum = 0; rownum < s.rows; rownum++) {
		last_seen[rownum] = new float[s.cols];
		moved_to[rownum] = new bool[s.cols];
    freed[rownum] = new bool[s.cols];
    for (int colnum = 0; colnum < s.cols; colnum++) {
      moved_to[rownum][colnum] = 0;
      freed[rownum][colnum] = 0;
			last_seen[rownum][colnum] = DEFAULT_TURNS_SINCE_VISIBLE; // set default value
		}
	}
}

LocalData::~LocalData() {
	for (int rownum = 0; rownum < num_rows; rownum++) {
		delete last_seen[rownum];
	}
	delete last_seen;
}

void LocalData::newTurn(State& s) {
  for (int r = 0; r < num_rows; r++) {
		for (int c = 0; c < num_cols; c++) {
			if (s.grid[r][c].isVisible) {
        moved_to[r][c] = 0; // TODO performance boost by moving this code into 'endturn' section
        freed[r][c] = 0;
				last_seen[r][c] = s.turn; // set to last seen on current turn
			}
		}
	}			
}

int LocalData::turnsSinceSeen(State& s, int row, int col) {
	return s.turn - last_seen[row][col];
}

void LocalData::recordMove(Location& source, Location& dest) {
  moved_to[dest.row][dest.col] = 1;
  freed[dest.row][dest.col] = 1;
}
/* Return true if
 *  1) Not water
 *  2) Not an enemy ant
 *  3) We have not already sent an ant there that turn
 *  4) We have an ant there but it has been moved
 * TODO what happens if two enemy ants move onto same square?
 */
bool LocalData::passable(Location& loc, State& s) {
	int row = loc.row;
	int col = loc.col;
	// Check if the cell is water
	if (s.grid[row][col].isWater) {
		return false;
	}
	// Check if the cell is an ant
	if (s.grid[row][col].ant != -1) { // -1 denotes no ant (utils/Square.h)
		if (s.grid[row][col].ant != 0) { // 0 == US
			return false; // Enemy ant, can't occupy
		}
		// Check if we cleared the cell by moving our ants during the turn
		if (moved_to[row][col]) {
      return false; // definitely can't move to
    }
    else if (freed[row][col]) {
      return true; // if it's our ant but we freed it, ok
    }
    else {
      return false; // not freed, our ant, can't move
    }
	}
	// There are no impediments to moving to the cell
	return true;
}
