#include "PositionGene.h"

#include "../utils/State.h"
#include "../utils/Location.h"

void PositionGene::disperse_once(State& state, LocalData& local_data) {};

// TODO zero current buffer at end of turn, it's doing weird shit atm
void PositionGene::disperse_clustering(State& state, LocalData& local_data)
{
  for (int iter = 0; iter < cluster_disp_iters; iter++) {
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

        vector<Location>::iterator our_ant;
        vector<Location>::iterator enemy_ant;
        for (our_ant = state.myAnts.begin(); our_ant != state.myAnts.end(); ++our_ant) {
          for (enemy_ant = state.enemyAnts.begin(); enemy_ant != state.enemyAnts.end(); ++enemy_ant) {
            // Compute distance
            double distance = state.distance(*our_ant, *enemy_ant);
            if (distance < cluster_threshold) {
              disp_field_curr[our_ant->row][our_ant->col] = cluster_attractiveness;
              continue; 
            }
          }
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
  			disp_field_curr[i][j] = cluster_disp_coeff * (center_energy + adj_energy);
  		}
  	}
  }
  dump_current_df(state.turn); 
  // TODO clear dispersion field (curr) here
}

void PositionGene::express(State& s, LocalData& ld, AntManager& am) {
	disperse_clustering(s, ld);
	am.apply_field(s, disp_field_curr); // Apply current dispersion field to all ants
  
}
