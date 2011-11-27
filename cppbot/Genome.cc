#include "Genome.h"
#include "utils/State.h"
#include "utils/AntManager.h"

int Genome::size() {
	return genes.size();
}

void Genome::setup(State& s) {
	genes.push_back(new ExploreGene(s.rows, s.cols));
}

Genome::~Genome() {
	for (int g = 0; g < NUM_GENES; g++) {
		delete genes[g];
	}
}

void Genome::express(State& s, LocalData& ld, AntManager& am) {
	for (int g = 0; g < NUM_GENES; g++) {
		genes[g]->express(s, ld, am);
	}
}
