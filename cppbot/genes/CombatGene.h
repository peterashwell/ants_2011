#ifndef COMBAT_GENE_H
#define COMBAT_GENE_H

using namespace std;

#include "Gene.h"
#include "../utils/State.h"
#include "../utils/LocalData.h"
#include "../utils/AntManager.h"
#include "../GENE_PARAMS.h"

class CombatGene : public Gene
{
public:
	CombatGene(int rows, int cols,
      float enemy_attract = COMBATGENE_ENEMY_ATTRACT,
      float hill_attract = COMBATGENE_HILL_ATTRACT,
      float disp_coeff = COMBATGENE_DISP_COEFF,
      int disp_iters = COMBATGENE_DISP_ITERS
    ) : Gene("CombatGene", rows, cols)
	{
		this->enemy_attract = enemy_attract;
		this->hill_attract = hill_attract;
    this->disp_coeff = disp_coeff;
    this->dispersion_iterations = disp_iters;
	}
	void express(State& state, LocalData& ld, AntManager& am);
	void disperse_once(State& state, LocalData& ld);

private:
	float enemy_attract, hill_attract, disp_coeff;
  int dispersion_iterations;
};

#endif
