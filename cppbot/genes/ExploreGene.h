#ifndef EXPLORE_GENE_H
#define EXPLORE_GENE_H

#include <algorithm>
#include <cmath>
using namespace std;

#include "Gene.h"
#include "../utils/State.h"
#include "../utils/LocalData.h"
#include "../utils/AntManager.h"
class ExploreGene : public Gene
{
public:
	ExploreGene(int rows, int cols, float unex_attract = 1.0f, float unex_decay = 1.0f, float disp_coeff = 0.9f) : Gene("ExploreGene", rows, cols)
	{
		this->unex_attract = unex_attract;
		this->unex_decay = unex_decay;
		this->disp_coeff = disp_coeff;
	}
	void express(State& state, LocalData& ld, AntManager& am);
	void disperse_once(State& state, LocalData& ld);

private:
	float unex_attract, unex_decay, disp_coeff;
	const static int DISPERSION_ITERATIONS = 100;
};

#endif
