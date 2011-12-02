#ifndef HILL_GENE_H
#define HILL_GENE_H

using namespace std;

#include "Gene.h"
#include "../utils/State.h"
#include "../utils/LocalData.h"
#include "../utils/AntManager.h"
#include "../GENE_PARAMS.h"

class HillGene : public Gene
{
public:
	HillGene(int rows, int cols,
      float hill_attract = HILLGENE_HILL_ATTRACT,
      float disp_coeff = HILLGENE_DISP_COEFF,
      int disp_iters = HILLGENE_DISP_ITERS
    ) : Gene("HillGene", rows, cols)
	{
		this->hill_attract = hill_attract;
    this->disp_coeff = disp_coeff;
    this->dispersion_iterations = disp_iters;
	}
	void express(State& state, LocalData& ld, AntManager& am);
	void disperse_once(State& state, LocalData& ld);

private:
	float hill_attract, disp_coeff;
  int dispersion_iterations;
};

#endif
