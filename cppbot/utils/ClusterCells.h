#ifndef CLUSTER_CELLS_H
#define CLUSTER_CELLS_H

class LocalData;

#include "Location.h"
#include "State.h"
#include "LocalData.h"

class ClusterCells {
  private:
    int NUM_CLUSTERS; // current number of clusters
    static const int REFINE_ITERATIONS = 5; // Max iterations to refine clusters
    std::vector<std::vector<Location> > clusters; // cluster 0, 1, 2, ...
    void split_clusters(State& s); // break up existing clusters into smaller ones
    void refine_clusters(State& s, LocalData& ld);
    Location find_center(State& s, LocalData& ld, int cnum); // find center cell of cluster "cluster"
  public:
    ClusterCells() {NUM_CLUSTERS = 1;}
    void add_cells(State& s, LocalData& ld, std::vector<Location>& new_cells);
    void dump_cluster_data(State& s);
};

#endif
