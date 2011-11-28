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
		bool have_moved = false;
		vector<pair<Location, Location> >::iterator move;
		cerr << "checking passable at : " << tostring(loc) << endl;
		for (move = moves.begin(); move != moves.end(); move++) {
			cerr << "checking: " << tostring(move->first) << " " << (loc == move->first) << endl;
			// The cell has been cleared (but possibly reoccupied)
			if (loc == (move->first)) {
				have_moved = true;
			}
			// The cell has already been occupied by one of our ants
			cerr << "checking: " << tostring(move->second) << " " << (loc == move->second) << endl;
			if (loc == (move->second)) {
				return false;
			}
		}
		// The cell has been cleared and not reoccupied
		if (have_moved) {
			return true;
		}
		return false; // Can't move there, we are occupying it!
	}
	// There are no impediments to moving to the cell
	return true;
}

string LocalData::dumpMoves() {
	stringstream out;
	if (moves.size() == 0) {
		return "";
	}
	out << "moves are currently:" << endl;
	for (int m = 0; m < moves.size(); m++) {
		out << "(" << moves.at(m).first.row << "," << moves.at(m).first.col << ")" << "=>";
		out << "(" << moves.at(m).second.row << "," << moves.at(m).second.col << ")" << endl;
	}
	return out.str();
}
