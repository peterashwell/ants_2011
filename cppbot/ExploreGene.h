#ifndef EXPLORE_GENE_H
#define EXPLORE_GENE_H

#include <algorithm>
#include <cmath>
using namespace std;

#include "Gene.h"
#include "State.h"

class ExploreGene : public Gene
{
public:
  ExploreGene(int rows, int cols, float unex_attract = 1.0f, float unex_decay = 1.0f, float disp_coeff = 1.0f) : Gene("ExploreGene", rows, cols)
  {
    this->unex_attract = unex_attract;
    this->unex_decay = unex_decay;
    this->disp_coeff = disp_coeff;
  }

  void disperse_once(State state);

private:
  float unex_attract, unex_decay, disp_coeff;
};

#endif