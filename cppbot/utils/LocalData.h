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
    bool** moved_to; // ant moved to here
		bool** freed; // ant moved out of here
    float** last_seen;
		
    void recordMove(Location& start, Location& dest);
    bool passable(Location& loc, State& s); // Tests is a square is okay to move to
		int turnsSinceSeen(State& s, int row, int col);
		void setup(State& s); // Set up the LocalData for the remainder of the game
		void newTurn(State& s); // update stuff for new turn
		std::string dumpMoves();
	private:
		void updateVisibleData(State& s);	
		static const int DEFAULT_TURNS_SINCE_VISIBLE = -200;
	// TODO food history
};

#endif
