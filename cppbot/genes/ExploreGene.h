#ifndef EXPLORE_GENE_H
#define EXPLORE_GENE_H

#include <algorithm>
#include <cmath>
using namespace std;

#include "Gene.h"
#include "../utils/State.h"
#include "../utils/LocalData.h"
#include "../utils/AntManager.h"
#include "../GENE_PARAMS.h"
class ExploreGene : public Gene
{
public:
	ExploreGene(int rows, int cols,
      float unex_attract = EXPGENE_UNEX_ATTRACT,
      float unex_decay = EXPGENE_UNEX_DECAY,
      float disp_coeff = EXPGENE_DISP_COEFF,
      int unseen_limit = EXPGENE_UNSEEN_LIMIT
    ) : Gene("ExploreGene", rows, cols)
	{
		this->unex_attract = unex_attract;
		this->unex_decay = unex_decay;
		this->disp_coeff = disp_coeff;
    this->unseen_limit = unseen_limit;
	}
	void express(State& state, LocalData& ld, AntManager& am);
	void disperse_once(State& state, LocalData& ld);

private:
	float unex_attract, unex_decay, disp_coeff, unseen_limit;
	const static int DISPERSION_ITERATIONS = 20;
};

#endif
