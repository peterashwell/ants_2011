#include "utils/AntManager.h"
#include "utils/State.h"

#include "genes/Gene.h"
#include "genes/ExploreGene.h"

struct Genome {
	static const int NUM_GENES = 1;

	public:
		Genome();
		Gene genes[NUM_GENES];
		express(State s, AntManager am); // Express all genes
}

/* GENOME IS DEFINED HERE */
Genome::Genome() {
	genes = {ExploreGene()}
}

Genome::express(State s, AntManager am) {
	for (int g = 0; g < NUM_GENES; g++) {
		genes[g].express(s, am);
	}
}
