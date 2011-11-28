#include <vector>
#include <algorithm>

#include "State.h"
#include "Location.h"
#include "AntManager.h"
#include "LocalData.h"
#include "utils.h"

using namespace std;



void AntManager::newTurn(State& s) {
	ant_forces.clear(); // TODO can this be done more efficiently
	for (int antnum = 0; antnum < s.myAnts.size(); antnum++) {
		vector<pair<int, float> > new_vec; 
		for (int d = 0; d < TDIRECTIONS; d++) {
			new_vec.push_back(pair<int, float>(d, 0.0)); // just cause it's easy fuck off
		}
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
			ant_forces.at(antnum)[d].second += df[dest.row][dest.col];
		}
	}
}

// Modify a single force in a direction around the ant
void AntManager::apply_single(std::pair<int, int> ant, int dir) {
	// TODO this (not needed until later)
}

// Resolve the forces around each ant to issue an order
void AntManager::resolve_forces(State& s, LocalData& ld) {
	//debug << "stuff" << endl;
	for (int antnum = 0; antnum < ant_forces.size(); antnum++) {
		Location ant_loc = s.myAnts.at(antnum); // parallel arrays
		cerr << "at: " << tostring(ant_loc) << endl;
		sort(ant_forces[antnum].begin(), ant_forces[antnum].end(), compareAF());
		// Try to moves from best to worst
		for (int index = 0; index < TDIRECTIONS; ++index) {
			cerr << "index: " << index << " " << "force: " << ant_forces[antnum][index].second << endl;
			int best_direction = ant_forces[antnum][index].first; // direction of greatest force
			Location dest = s.getLocation(s.myAnts.at(antnum), best_direction);	
			cerr << "trying to move to: " << tostring(dest) << endl;
			if (ld.passable(dest, s)) { // See LocalData.h
				cerr << "passable" << endl;
				s.makeMove(ant_loc, best_direction);
				ld.moves.push_back(pair<Location, Location>(ant_loc, dest));
				break; // no more moves for this guy
			}
		}
		// TODO this needs to be rewritten to recursively call resolve for ants blocking the way
	}
	cerr << ld.dumpMoves();
}
