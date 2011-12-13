#ifndef GENOME_H
#define GENOME_H

#include <string>
#include <map>
using namespace std;

#include "utils/AntManager.h"
#include "utils/State.h"
#include "utils/LocalData.h"

#include "genes/Gene.h"
#include "genes/ExploreGene.h"


struct Genome {
	public:
		void setup(State& s, string filename);
		~Genome();
		std::vector<Gene*> genes;
		void express(State& s, LocalData& ld, AntManager& am); // Express all genes
		int size();
	private:
		void loadFile(string filename);
		map<string, float> gene_vec;
};

#endif
