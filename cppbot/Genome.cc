#include "Genome.h"
#include "utils/State.h"
#include "utils/AntManager.h"
#include "utils/utils.h"
#include "genes/FoodGene.h"
#include "genes/HillGene.h"
#include "genes/ExploreGene.h"
#include "genes/PositionGene.h"

#include <fstream>
#include <vector>
#include <exception>
using namespace std;

int Genome::size() {
	return genes.size();
}

void Genome::setup(State& s, string filename = "") {
  loadFile(filename);
  genes.push_back(new ExploreGene(s.rows, s.cols,
    gene_vec["expgene_unex_attract"],
    gene_vec["expgene_unex_decay"],
    gene_vec["expgene_unseen_limit"]
  ));
  genes.push_back(new FoodGene(s.rows, s.cols,
    gene_vec["foodgene_food_attract"],
    gene_vec["foodgene_collab_coeff"],
    gene_vec["foodgene_disp_coeff"]
  ));
  genes.push_back(new PositionGene(s.rows, s.cols,
    gene_vec["posgene_strat_disp_iters"],
    gene_vec["posgene_cluster_disp_coeff"],
    gene_vec["posgene_cluster_disp_iters"],
    gene_vec["posgene_cluster_threshold"],
    gene_vec["posgene_cluster_attract"],
    gene_vec["posgene_strat_overlap_radius"],
    gene_vec["posgene_strat_coverage_bound"],
    gene_vec["posgene_strat_disp_coeff"]
  ));
  genes.push_back(new HillGene(s.rows, s.cols,
    gene_vec["hillgene_hill_attract"],
    gene_vec["hillgene_disp_coeff"],
    gene_vec["hillgene_disp_iters"] 
  ));
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

void Genome::loadFile(string filename)
{
  gene_vec["energy_threshold"] = 1e-10;
  
  gene_vec["expgene_disp_iters"] = 20;
  gene_vec["expgene_unex_attract"] = 2;
  gene_vec["expgene_unex_decay"] = 1;
  gene_vec["expgene_disp_coeff"] = 0.95;
  gene_vec["expgene_unseen_limit"] = 10;
  
  gene_vec["foodgene_disp_iters"] = 30;
  gene_vec["foodgene_food_attract"] = 10;
  gene_vec["foodgene_collab_coeff"] = 0.95;
  gene_vec["foodgene_disp_coeff"] = 0.9;
  
  gene_vec["hillgene_enemy_attract"] = -0.9;
  gene_vec["hillgene_hill_attract"] = 1;
  gene_vec["hillgene_disp_coeff"] = 0.95;
  gene_vec["hillgene_disp_iters"] = 30;
  
  gene_vec["posgene_cluster_disp_coeff"] = 0.95;
  gene_vec["posgene_cluster_disp_iters"] = 4;
  gene_vec["posgene_cluster_threshold"] = 4;
  gene_vec["posgene_cluster_attract"] = 2;
  gene_vec["posgene_strat_disp_iters"] = 5;
  gene_vec["posgene_strat_overlap_radius"] = 5;
  gene_vec["posgene_strat_coverage_bound"] = 0.5;
  gene_vec["posgene_strat_disp_coeff"] = 0.95;
  
  string line;
  ifstream file(("gene_vectors/" + filename).c_str());
  if(!file.is_open())
  {
    cerr << "Couldn't load file" << endl;
    return;
  }
  cerr << "Loading: " << filename << endl;
  while(!file.eof())
  {
    getline(file, line);
    vector<string> value = split(line, '=');
    if(value.size() == 2)
    {
      float x;
      from_string<float>(x, value[1], dec);
      gene_vec[value[0]] = x;
      cerr << "  " << value[0] << " = " << gene_vec[value[0]] << endl;
    }
  } 
  file.close();
}
