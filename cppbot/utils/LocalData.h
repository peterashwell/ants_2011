#ifndef LOCALDATA_H
#define LOCALDATA_H

#include <vector>

#include "Location.h"
#include "State.h"
#include "utils.h"


/**
 * Used for storing game data and history not given in State.h
 */
struct LocalData {
	// Keeps track of moves we have made
	std::vector<std::pair<Location, Location> > moves;
	bool passable(Location loc, State s); // Tests is a square is okay to move to
	// TODO food history
};

/* Return true if
 *  1) Not water
 *  2) Not an enemy ant
 *  3) We have not already sent an ant there that turn
 *  4) We have an ant there but it has been moved
 * TODO what happens if two enemy ants move onto same square?
 */
bool LocalData::passable(Location loc, State s) {
	int row = loc.row;
	int col = loc.col;
	// Check if the cell is water
	if (s.grid[row][col].isWater) {
		return false;
	}
	// Check if the cell is an ant
	if (s.grid[row][col].ant != -1) { // -1 denotes no ant (utils/Square.h)
		if (s.grid[row][col].ant != PLAYER) {
			return false; // Enemy ant, can't occupy
		}
		// Check if we cleared the cell by moving our ants during the turn
		bool have_moved = true;
		std::vector<std::pair<Location, Location> >::iterator move;
		for (move = moves.begin(); move != moves.end(); move++) {
			// The cell has been cleared (but possibly reoccupied)
			if (loc == (move->first)) {
				have_moved = true;
			}
			// The cell has already been occupied by one of our ants
			if (loc == (move->second)) {
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

#endif
