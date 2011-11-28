#ifndef ANT_MANAGER_H
#define ANT_MANAGER_H

#include <utility> 
#include <vector>

#include "State.h"
#include "LocalData.h"
#include "../Bug.h"

struct AntManager {
	std::vector<std::vector<std::pair<int, float> > > ant_forces; // TODO this could be done faster	
	
	// Map info copied at start
	int rows;
	int cols;

	void newTurn(State& s); // Empty and prepare vectors for next turn
	void apply_field(State& s, float** df); // accumulate forces from disp fields
	void apply_single(std::pair<int, int> ant, int dir);
	void resolve_forces(State &s, LocalData &ld); // resolve max forces to issue orders

	struct compareAF {
		bool operator() (const std::pair<int, float>& left,
		  const std::pair<int, float>& right) {
			return left.second > right.second;
		}
	};
};

#endif
