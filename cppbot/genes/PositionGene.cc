#include "PositionGene.h"

#include "../utils/State.h"
#include "../utils/Location.h"

void PositionGene::disperse_once(State& state, LocalData& local_data) {};

// If we outnumber enemy nearby (attackrad + const radius ours and theirs), advance
// Else retreat (apply repulsive force)
// TODO distance from our ant or enemy ant does not propagate over water
void PositionGene::disperse_localstrat(State& state, LocalData& local_data)
{
  // Overlay additive constants up to D distance from our ants and for enemy ants
  // The sum our attacking - their attacking forms repulsive or attractive sources
  // Do this only for ants with C distance of an enemy (and all ants with R of them)
   
  // Identify cells containing our ants within R/2 of an enemy (these will have additive fields computed)
  // Now go through all the ants, ours and otherwise and accumulate the sum of their coverage - overlaps are sources
  vector<Location>::iterator my_ant;
  vector<Location>::iterator enemy_ant;
  vector<pair<int, int> > overlaps;
  int our_coverage[rows][cols];
  int enemy_coverage[rows][cols];
  bool overlap[rows][cols];
  for (int r = 0; r < rows; ++r) { // TODO write efficient method to return cells in range of a target cell
    for (int c = 0; c < cols; ++c) {
      overlap[r][c] = 0;
      our_coverage[r][c] = 0;
      enemy_coverage[r][c] = 0;
      Location cur_cell(r, c);
      for (my_ant = state.myAnts.begin(); my_ant != state.myAnts.end(); ++my_ant) {
        if (state.distance(*my_ant, cur_cell) < _strat_overlap_radius) {
          our_coverage[r][c] += 1;
        }
      }
      for (enemy_ant = state.enemyAnts.begin(); enemy_ant != state.enemyAnts.end(); ++enemy_ant) {
        if (state.distance(*enemy_ant, cur_cell) < _strat_overlap_radius) {
          enemy_coverage[r][c] += 1;
        }
      }
      if (our_coverage[r][c] > 0 && enemy_coverage[r][c] > 0) {
        overlap[r][c] = 1;
      }
    }
  }

  // Now use standard dispersion on the sources in coverage
  for (int iters = 0; iters < _strat_disp_iters; iters++) {
    swap_disp_buffer();
    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) {
        if (local_data.is_water[r][c]) { // do not interact with water
          disp_field_curr[r][c] = 0;
          continue;
        }
        float center_energy = disp_field_prev[r][c]; // Default - not energy source

        if (overlap[r][c]) {
          int coverage = (our_coverage[r][c] - enemy_coverage[r][c]);
          if (fabs(coverage) > _strat_coverage_bound) {
            disp_field_curr[r][c] = coverage;
            continue; 
          }
        }

        // Compute the sum of the adjacent squares and how many were used
        float adj_sum = 0;
        int adj_count = 0;
        Location loc(r, c);
        for(int direction = 0; direction < TDIRECTIONS; ++direction)
        {
          Location adj = state.getLocation(loc, direction);
          if(!local_data.is_water[adj.row][adj.col]) // If is passable
          {
            adj_sum += disp_field_prev[adj.row][adj.col] - center_energy;
            ++adj_count;
          }
        }
        float adj_energy = 0.0f;

        if(adj_count > 0) {
          adj_energy = adj_sum / adj_count;
        }
        disp_field_curr[r][c] = _strat_disp_coeff * (center_energy + adj_energy);
      }
    }
  }
}

// TODO zero current buffer at end of turn, it's doing weird shit atm
void PositionGene::disperse_clustering(State& state, LocalData& local_data)
{
  // TODO the clustering impact should not go through water, or should, but only a bit?
  // This will allow us to compute our ants near enemies once
  bool near_enemy[rows][cols];
  vector<Location>::iterator enemy_ant;
  // ZERO THE BASTARD
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      near_enemy[r][c] = 0; 
      Location our_ant(r, c);
      if (state.grid[r][c].ant != 0) { // must be our ant
        continue;
      }
      for (enemy_ant = state.enemyAnts.begin(); enemy_ant != state.enemyAnts.end(); ++enemy_ant) {
        double distance = state.distance(our_ant, *enemy_ant);
        if (distance < _cluster_threshold) {
          near_enemy[r][c] = 1; // reuse in loop 
        }
        continue;
      }
    }
  }
  
  for (int iter = 0; iter < _cluster_disp_iters; iter++) {
    // Swap the buffer 
    swap_disp_buffer();
    for(int i = 0; i < rows; ++i)
    {
      for(int j = 0; j < cols; ++j)
      {
        Location loc(i, j);
        if (local_data.is_water[i][j]) { // TODO replace all iswaters with function from localdata
          disp_field_curr[i][j] = 0;
          continue; // Don't compute dispersion for this square
        }
        float center_energy = disp_field_prev[i][j]; // Default - not energy source

        if (near_enemy[i][j]) {
          disp_field_curr[i][j] = _cluster_attractiveness;
          continue; 
        }

        // Compute the sum of the adjacent squares and how many were used
        float adj_sum = 0;
        int adj_count = 0;

        for(int direction = 0; direction < TDIRECTIONS; ++direction)
        {
          Location adj = state.getLocation(loc, direction);
          if(!local_data.is_water[adj.row][adj.col]) // If is passable
          {
            adj_sum += disp_field_prev[adj.row][adj.col] - center_energy;
            ++adj_count;
          }
        }
        float adj_energy = 0.0f;

        if(adj_count > 0) {
          adj_energy = adj_sum / adj_count;
        }
        disp_field_curr[i][j] = _cluster_disp_coeff * (center_energy + adj_energy);
      }
    }
  }
  // TODO clear dispersion field (curr) here
}

void PositionGene::express(State& s, LocalData& ld, AntManager& am) {
  //disperse_clustering(s, ld);
  //am.apply_field(s, disp_field_curr); // Apply current dispersion field to all ant
  clean_current_df();
  disperse_localstrat(s, ld);
  am.apply_field(s, disp_field_curr);
  #ifdef DEBUG_DF
    dump_current_df(s.turn);
  #endif
}
