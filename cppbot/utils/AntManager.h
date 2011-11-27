#ifndef ANT_MANAGER_H
#define ANT_MANAGER_H

#include <utility> 
#include <vector>

#include "State.h"
#include "LocalData.h"


struct AntManager {
	std::vector<std::vector<float> > ant_forces; // TODO this could be done faster	
	
	// Map info copied at start
	int rows;
	int cols;

	void newTurn(State& s); // Empty and prepare vectors for next turn
	void apply_field(State& s, float** df); // accumulate forces from disp fields
	void apply_single(std::pair<int, int> ant, int dir);
	void resolve_forces(State &s, LocalData &ld); // resolve max forces to issue orders
};

#endif
