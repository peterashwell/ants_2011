#ifndef COMBAT_GENE_H
#define COMBAT_GENE_H

using namespace std;

#include "Gene.h"
#include "../utils/State.h"
#include "../utils/LocalData.h"
#include "../utils/AntManager.h"
#include "../GENE_PARAMS.h"

class BruteCombatGene : public Gene
{
public:
	BruteCombatGene(int rows, int cols) : Gene("BruteCombatGene", rows, cols){};
	void express(State& state, LocalData& ld, AntManager& am);
	void disperse_once(State& state, LocalData& ld);

private:
  // nothing 
};

#endif
