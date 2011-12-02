#ifndef POSGENE_GENE_H
#define POSGENE_GENE_H

using namespace std;

#include "Gene.h"
#include "../utils/State.h"
#include "../utils/LocalData.h"
#include "../utils/AntManager.h"
#include "../GENE_PARAMS.h"

class PositionGene : public Gene
{
public:
	PositionGene(int rows, int cols,
      float cluster_disp_coeff = POSGENE_CLUSTER_DISP_COEFF,
      float cluster_threshold = POSGENE_CLUSTER_THRESHOLD,
      float cluster_attractiveness = POSGENE_CLUSTER_ATTRACTIVENESS,
      int cluster_disp_iters = POSGENE_CLUSTER_DISP_ITERS
    ) : Gene("PositionGene", rows, cols)
	{
    this->cluster_disp_iters = cluster_disp_iters;
    this->cluster_threshold = cluster_threshold;
    this->cluster_disp_coeff = cluster_disp_coeff;
    this->cluster_disp_iters = cluster_disp_iters;
	}
	void express(State& state, LocalData& ld, AntManager& am);
	void disperse_clustering(State& state, LocalData& ld);
  void disperse_once(State& state, LocalData& ld); // NOT USED
private:
  float cluster_disp_coeff, cluster_threshold, cluster_attractiveness;
  int cluster_disp_iters;
};

#endif
