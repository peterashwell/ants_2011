#include "utils/AntManager.h"
#include "utils/State.h"

#include "genes/Gene.h"
#include "genes/ExploreGene.h"

struct Genome {
	static const int NUM_GENES = 1;

	public:
		void initGenes(State s);
		~Genome();
		std::vector<Gene*> genes;
		void express(State s, AntManager am); // Express all genes
		int size();
};

int Genome::size() {
	return genes.size();
}

/* GENOME IS DEFINED HERE */
void Genome::initGenes(State s) {
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
