#include "utils/AntManager.h"
#include "utils/State.h"
#include "utils/LocalData.h"

#include "genes/Gene.h"
#include "genes/ExploreGene.h"

#ifndef GENOME_H
#define GENOME_H

struct Genome {
	static const int NUM_GENES = 1;

	public:
		void setup(State& s);
		~Genome();
		std::vector<Gene*> genes;
		void express(State& s, LocalData& ld, AntManager& am); // Express all genes
		int size();
};

#endif
