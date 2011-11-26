#include "utils/AntManager.h"
#include "utils/State.h"

#include "genes/Gene.h"
#include "genes/ExploreGene.h"

struct Genome {
	static const int NUM_GENES = 1;

	public:
		Genome(State s);
		~Genome();
		std::vector<Gene*> genes;
		void express(State s, AntManager am); // Express all genes
};

/* GENOME IS DEFINED HERE */
Genome::Genome(State s) {
	genes.push_back(new ExploreGene(s.rows, s.cols));
}

Genome::~Genome() {
	for (int g = 0; g < NUM_GENES; g++) {
		delete genes[g];
	}
}

void Genome::express(State s, AntManager am) {
	for (int g = 0; g < NUM_GENES; g++) {
		genes[g]->express(s, am);
	}
}
