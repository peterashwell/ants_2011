#ifndef LOCALDATA_H
#define LOCALDATA_H

#include <vector>
#include <fstream>
#include <string>

#include "Location.h"
#include "State.h"

/**
 * Used for storing game data and history not given in State.h
 */
class LocalData {
	// Keeps track of moves we have made
	private:
		int num_rows;
		int num_cols;
	public:
		~LocalData();
		std::vector<std::pair<Location, Location> > moves;
		float** last_seen;
		bool passable(Location& loc, State& s); // Tests is a square is okay to move to
		int turnsSinceSeen(State& s, int row, int col);
		void setup(State& s); // Set up the LocalData for the remainder of the game
		void newTurn(State& s); // update stuff for new turn
		std::string dumpMoves();
	private:
		void updateVisibleData(State& s);	
		static const int DEFAULT_TURNS_SINCE_VISIBLE = -10;
	// TODO food history
};

#endif
