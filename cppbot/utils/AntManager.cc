#include <vector>

#include "State.h"
#include "Location.h"
#include "AntManager.h"
#include "LocalData.h"

using namespace std;

void AntManager::newTurn(State& s) {
	float default_forces[4] = {0.0, 0.0, 0.0, 0.0};
	for (int antnum = 0; antnum < s.myAnts.size(); antnum++) {
		vector<float> new_vec;
		new_vec.assign(default_forces, default_forces + 4);
		ant_forces.push_back(new_vec);
	}
}

// Apply a whole dispersion field to each of our ants
void AntManager::apply_field(State& s, float** df) {
	// Apply dispersion field to each ant in turn
	for (int antnum = 0; antnum < s.myAnts.size(); antnum++) {
		Location ant_loc = s.myAnts.at(antnum);
		// For each direction around the ant add the forces
		for (int d = 0; d < TDIRECTIONS; d++) {  
			Location dest = s.getLocation(ant_loc, d);
			ant_forces.at(antnum)[d] += df[dest.row][dest.col];
		}
	}
}

// Modify a single force in a direction around the ant
void AntManager::apply_single(std::pair<int, int> ant, int dir) {
	// TODO this (not needed until later)
}

// Resolve the forces around each ant to issue an order
void AntManager::resolve_forces(State& s, LocalData& ld) {
	for (int antnum = 0; antnum < ant_forces.size(); antnum++) {
		Location ant_loc = s.myAnts.at(antnum); // parallel arrays
		int best_dir = 0;
		float best_force = ant_forces.at(antnum)[0];
		for (int d = 1; d < TDIRECTIONS; d++) {
			// TODO is a threshold needed here
			if (ant_forces.at(antnum)[d] > best_force) {
				best_dir = d;
			}
		}
		// TODO this needs to be rewritten to recursively call resolve for ants blocking the way
		if (ld.passable(ant_loc, s)) { // See LocalData.h
			s.makeMove(ant_loc, best_dir);
			ld.moves.push_back(pair<Location, Location>(ant_loc, s.getLocation(ant_loc, best_dir)));
		}
	}
}
