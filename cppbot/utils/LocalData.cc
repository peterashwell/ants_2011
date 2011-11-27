#include "LocalData.h"
#include "State.h"
#include "Location.h"

void LocalData::setup(State& s) {
	num_rows = s.rows;
	num_cols = s.cols;
	last_seen = new float*[s.rows];
	for (int rownum = 0; rownum < s.rows; rownum++) {
		last_seen[rownum] = new float[s.cols];
		for (int colnum = 0; colnum < s.cols; colnum++) {
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
	moves.clear();
	updateVisibleData(s);
}

void LocalData::updateVisibleData(State& s) {
	for (int r = 0; r < num_rows; r++) {
		for (int c = 0; c < num_cols; c++) {
			if (s.grid[r][c].isVisible) {
				last_seen[r][c] = s.turn; // set to last seen on current turn
			}
		}
	}			
}

int LocalData::turnsSinceSeen(State& s, int row, int col) {
	return s.turn - last_seen[row][col];
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
		bool have_moved = true;
		std::vector<std::pair<Location, Location> >::iterator move;
		for (move = moves.begin(); move != moves.end(); move++) {
			// The cell has been cleared (but possibly reoccupied)
			if (loc.row == (move->first).row && loc.col == (move->first).col) {
				have_moved = true;
			}
			// The cell has already been occupied by one of our ants
			if (loc.row == (move->second).row && loc.col == (move->second).col) {
				return false;
			}
		}
		// The cell has been cleared and not reoccupied
		if (have_moved) {
			return true;
		}
	}
	// There are no impediments to moving to the cell
	return true;
}
