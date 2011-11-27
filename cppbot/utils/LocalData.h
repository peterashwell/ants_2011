#ifndef LOCALDATA_H
#define LOCALDATA_H

#include <vector>

#include "Location.h"
#include "State.h"
#include "utils.cc"


/**
 * Used for storing game data and history not given in State.h
 */
class LocalData {
	// Keeps track of moves we have made
	private:
		int num_rows;
		int num_cols;
		float** last_seen;
	public:
		~LocalData();
		std::vector<std::pair<Location, Location> > moves;
		bool passable(Location& loc, State& s); // Tests is a square is okay to move to
		int turnsSinceSeen(State& s, int row, int col);
		void setup(State& s); // Set up the LocalData for the remainder of the game
		void newTurn(State& s); // update stuff for new turn
	private:
		void updateVisibleData(State& s);	

	// TODO food history
};

#endif
