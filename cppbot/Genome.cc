#include "Genome.h"
#include "utils/State.h"
#include "utils/AntManager.h"
#include "genes/FoodGene.h"
#include "genes/CombatGene.h"
#include "genes/ExploreGene.h"

int Genome::size() {
	return genes.size();
}

void Genome::setup(State& s) {
	genes.push_back(new ExploreGene(s.rows, s.cols));
  genes.push_back(new FoodGene(s.rows, s.cols));
  genes.push_back(new CombatGene(s.rows, s.cols));
}

Genome::~Genome() {
  for (int g = 0; g < genes.size(); g++) {
    delete genes[g];
  }
}

void Genome::express(State& s, LocalData& ld, AntManager& am) {
	for (int g = 0; g < genes.size(); g++) {
		genes[g]->express(s, ld, am);
	}
}
