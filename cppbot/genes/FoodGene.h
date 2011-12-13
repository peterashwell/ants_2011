#ifndef FOOD_GENE_H
#define FOOD_GENE_H

#include <algorithm>
#include <cmath>
using namespace std;

#include "Gene.h"
#include "../utils/State.h"
#include "../utils/LocalData.h"
#include "../utils/AntManager.h"
#include "../GENE_PARAMS.h"

class FoodGene : public Gene
{
public:
	FoodGene(int rows, int cols,
      float food_attract = FOODGENE_FOOD_ATTRACT,
      float food_collab_coeff = FOODGENE_COLLAB_COEFF,
      float disp_coeff = FOODGENE_DISP_COEFF
    ) : Gene("FoodGene", rows, cols)
	{
		this->food_attract = food_attract;
		this->food_collab_coeff = food_collab_coeff;
                this->disp_coeff = disp_coeff;
	}
	void express(State& state, LocalData& ld, AntManager& am);
	void disperse_once(State& state, LocalData& ld);

private:
	float food_attract, food_collab_coeff, disp_coeff;
	const static int DISPERSION_ITERATIONS = 20;
};

#endif
