#include <vector>
#include "ClusterCells.h"
#include "LocalData.h"
#include "Location.h"

using namespace std;

int CELL_DIST_THRESHOLD = 10; // Max cell diameter

// Modify changed to be which clusters were split this turn
void ClusterCells::split_clusters(State& s) {
  // For each of the clusters in changed, compute the maximum distance for any two cells
  for (int cnum = 0; cnum < clusters.size(); ++cnum) {
    vector<Location>::iterator first_cell = clusters[cnum].begin();
    vector<Location>::iterator second_cell;
    bool not_split = true;
    while (first_cell != clusters[cnum].end() && not_split) {
      second_cell = first_cell + 1; 
      while (second_cell != clusters[cnum].end() && not_split) {
        if (s.distance(*first_cell, *second_cell) > CELL_DIST_THRESHOLD) {
          not_split = false;
          // Do the split
          vector<Location> cluster_1, cluster_2;
          cluster_1.push_back(*first_cell);
          cluster_2.push_back(*second_cell);
          clusters.push_back(cluster_1);
          clusters.push_back(cluster_2);
        }
        ++second_cell;
      }
      ++first_cell;
    }
  }
}

void ClusterCells::refine_clusters(State& s, LocalData& ld) {
  // Find the centers of each cluster, build a dispersion map, apply and recompute fields
  // TODO fix this shit
  float*** cur_disp_field = new float**[clusters.size()];
  float*** prev_disp_field = new float**[clusters.size()];
  for (int cnum = 0; cnum < clusters.size(); ++cnum) {
    cur_disp_field[cnum] = new float*[s.rows];
    prev_disp_field[cnum] = new float*[s.rows];
    for (int r = 0; r < s.rows; ++r) {
      cur_disp_field[cnum][r] = new float[s.cols];
      prev_disp_field[cnum][r] = new float[s.cols];
      for (int c = 0; c < s.cols; ++c) {
        cur_disp_field[cnum][r][c] = 0;
        prev_disp_field[cnum][r][c] = 0;
      }
    }
  }
  
  // Initialize dispersion field stuff
  int r, c, cnum, iter;
  for (cnum = 0; cnum < clusters.size(); ++cnum) {
    for (r = 0; r < s.rows; ++r) {
      for (c = 0; c < s.cols; ++c) {
        prev_disp_field[cnum][r][c] = -1;
      }
    }
  }
  vector<Location>::iterator cluster; // Iterate over clusters and place sources
  for (cnum = 0; cnum < clusters.size(); ++cnum) {
    Location center = find_center(s, ld, cnum); // center of this cluster
    // TODO center
    // Disperse from cluster centers and hills
    int iterations = 15;
    for (iter = 0; iter < iterations; ++iter) {
      for (r = 0; r < s.rows; ++r) {
        for (c = 0; c < s.cols; ++c) {
          // Ignore water TODO use water to dampen fields
          if (ld.is_water[r][c]) {
            cur_disp_field[cnum][r][c] = 0;
            continue;
          }
          // Define the sources as the centers
          if (center.row == r && center.col == c) {
            cur_disp_field[cnum][r][c] = 1;
            continue;
          }
          float adj_sum = 0;
          int adj_count = 0;
          Location cell(r,c);
          for (int d = 0; d < TDIRECTIONS; d++) {
            Location adj = s.getLocation(cell, d);
            if (!ld.is_water[adj.row][adj.col]) {
              adj_sum += prev_disp_field[cnum][adj.row][adj.col] - prev_disp_field[cnum][r][c];
              ++adj_count;
            }
          }
          float adj_energy = 0.0f;
          if (adj_count > 0) {
            adj_energy = adj_sum / adj_count;
          }
          float disp_coeff = 0.9;
          // Contains a hill - use collaboration coefficient
          if (s.grid[r][c].hillPlayer != -1) { // 0 is us TODO
            disp_coeff = 1.5;
          }
          cur_disp_field[cnum][r][c] = disp_coeff * (prev_disp_field[cnum][r][c] + adj_energy);
          // Swap the pointers over
          float*** temp = prev_disp_field;
          prev_disp_field = cur_disp_field;
          cur_disp_field = temp;
        }
      }
    }
  }
  // Now, allocate each cell to the cluster with highest value
  int num_clusters = clusters.size();
  for (cnum = 0; cnum < num_clusters; ++cnum) {
    clusters[cnum].clear();
  }
  for (r = 0; r < s.rows; ++r) {
    for (c = 0; c < s.cols; ++c) {
      int best_cluster = 0;
      float best_force = 0; 
      for (cnum = 0; cnum < num_clusters; ++cnum) {
        if (prev_disp_field[cnum][r][c] > best_force) {
          best_force = prev_disp_field[cnum][r][c];
          best_cluster = cnum;
        }
      }
      // TODO check if a cell has been observed
      clusters[cnum].push_back(Location(r,c));
    }
  }
  delete cur_disp_field;
  delete prev_disp_field;
}

void ClusterCells::dump_cluster_data(State& s) {
  int cluster_map[s.rows][s.cols];
  for (int r = 0; r < s.rows; r++) {
    for (int c = 0; c < s.cols; c++) {
      cluster_map[r][c] = 0;
    }
  }
  cerr << clusters.size() << " clusters total" << endl;
  for (int cnum = 0; cnum < clusters.size(); ++cnum) {
    cerr << "cluster " << cnum << " size: " << clusters[cnum].size() << endl;
    for (int cellnum = 0; cellnum < clusters[cnum].size(); cellnum++) {
      Location& cell = clusters[cnum][cellnum];
      cluster_map[cell.row][cell.col] = cnum + 1;
    }
  }
  cerr << "cluster map:" << endl;
  for (int r = 0; r < s.rows; r++) {
    for (int c = 0; c < s.cols; c++) {
      cerr << cluster_map[r][c];
    }
    cerr << endl;
  }
}

Location ClusterCells::find_center(State& s, LocalData& ld, int cnum) {
  // Find the center of the cluster
  if (clusters[cnum].size() == 0) {
    cerr << "warning, zero cluster" << endl;
    return Location(0, 0); // not sure if this is a good idea
  }
  
  Location rel_cell(clusters[cnum][0]);
  vector<Location>::iterator cell;
  
  float avg_x_offset;
  float avg_y_offset;

  for (cell = clusters[cnum].begin(); cell != clusters[cnum].end(); ++cell) {
    // Choose closest distance on taurus for offsets
    int y_offset_up = (rel_cell.row - cell->row) % s.rows;
    int y_offset_down = (cell->row - rel_cell.row) % s.rows;
    int x_offset_up = (rel_cell.col - cell->col) % s.cols;
    int x_offset_down = (cell->col - rel_cell.col) % s.cols;
    int x_offset, y_offset;
    if (fabs(y_offset_up) > fabs(y_offset_down)) {
      y_offset = y_offset_down;
    }
    else {
      y_offset = y_offset_up;
    }
    if (fabs(x_offset_up) > fabs(x_offset_down)) {
      x_offset = x_offset_down;
    }
    else {
      x_offset = x_offset_up;
    }
    avg_x_offset += x_offset;
    avg_y_offset += y_offset;
  }
  // Wrap offsets if necessary
  Location true_center((rel_cell.row + (int) avg_x_offset) % s.rows, (rel_cell.col + (int) avg_y_offset) % s.cols);
  
  // Now call a function to find nearest non-water square to our center and we are done (phew)
  return ld.find_nearest_nw_cell(true_center); 
}

void ClusterCells::add_cells(State& s, LocalData& ld, vector<Location>& new_cells) {
  // First, go through the new cells and allocate them to cluster NUM_CLUSTERS + 1  
  
  clusters.push_back(new_cells);
  int cur_iter = 0;
  bool converged = false;
  // TODO stop at convergence
  while (cur_iter < REFINE_ITERATIONS && !converged) {
    split_clusters(s); // splits all clusters
    // Refine the split result (either unchanged or two new clusters)
    refine_clusters(s, ld); // refines all clusters
    // And repeat until REFINE_ITERATIONS or changed.size == 0
    ++cur_iter;
  }
}
