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
	PositionGene(int rows, int cols) : Gene("PositionGene", rows, cols)
	{
    _cluster_disp_iters = POSGENE_CLUSTER_DISP_ITERS;
    _cluster_disp_coeff = POSGENE_CLUSTER_DISP_COEFF;
    _cluster_threshold = POSGENE_CLUSTER_THRESHOLD;
    _cluster_attractiveness = POSGENE_CLUSTER_ATTRACTIVENESS;
    _strat_overlap_radius = POSGENE_STRAT_OVERLAP_RADIUS;
    _strat_coverage_bound = POSGENE_STRAT_COVERAGE_BOUND;
    _strat_disp_iters = POSGENE_STRAT_DISP_ITERS;
    _strat_disp_coeff = POSGENE_STRAT_DISP_COEFF;
	}
	void express(State& state, LocalData& ld, AntManager& am);
	void disperse_clustering(State& state, LocalData& ld);
  void disperse_localstrat(State& state, LocalData& ld);
  void disperse_once(State& state, LocalData& ld); // NOT USED
private:
  float _cluster_disp_coeff, _cluster_threshold, _cluster_attractiveness;
  float _strat_disp_coeff, _strat_overlap_radius, _strat_coverage_bound;
  int _cluster_disp_iters, _strat_disp_iters;
};

#endif
