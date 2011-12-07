#include "HillGene.h"

#include "../utils/State.h"
#include "../utils/Location.h"

void HillGene::disperse_once(State& state, LocalData& local_data)
{
	// Swap the buffer 
	swap_disp_buffer();
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < cols; ++j)
		{
      if (local_data.is_water[i][j]) { // TODO replace all iswaters with function from localdata
        disp_field_curr[i][j] = 0;
        continue; // Don't compute dispersion for this square
      }
			Location cell_loc(i, j);
      float center_energy = disp_field_prev[i][j]; // Default - not energy source
			
      // Hill interaction
      // Is strongly (locally) collaboratively diffused by ants belonging to that hill
      // TODO add collaborative diffusion as an additional layer with a different dispersion coefficient
      if (state.grid[i][j].isHill == 1 && state.grid[i][j].hillPlayer != 0) { // enemy hill
        disp_field_curr[i][j] = hill_attract;
        continue;
      }

      // Enemy interaction
      // Squares which enemy cannot guarantee kills on next turn do not carry dispersion
      // Squares which link us to vulnerable enemies (enemies attacking < constant) carry attractive force
      // TODO vary behavior as surround/hold ground depending on terrain and support behind
      // For now, make our ants try to surround if we have more (with hard-coded advancing)
      // Retreat if we have less
      // Hold (no suicides) if we are about equal
		  //if (state.grid[i][j].ant > 0) { // enemy ant TODO constantize
			//	disp_field_curr[i][j] = enemy_attract;
			//	continue;
			//} 

			// Compute the sum of the adjacent squares and how many were used
			float adj_sum = 0;
			int adj_count = 0;

			for(int direction = 0; direction < TDIRECTIONS; ++direction)
			{
				Location adj = state.getLocation(cell_loc, direction);
				if(!state.grid[adj.row][adj.col].isWater) // If is passable
				{
					adj_sum += disp_field_prev[adj.row][adj.col] - center_energy;
					++adj_count;
				}
			}
			float adj_energy = 0.0f;

			if(adj_count > 0) {
				adj_energy = adj_sum / adj_count;
			}
			disp_field_curr[i][j] = disp_coeff * (center_energy + adj_energy);
		}
	}
}

void HillGene::express(State& s, LocalData& ld, AntManager& am) {
	for (int iter = 0; iter < dispersion_iterations; ++iter) {
		disperse_once(s, ld);
	}
  #ifdef DEBUG_DF
  dump_current_df(s.turn);
  #endif
	am.apply_field(s, disp_field_curr); // Apply current dispersion field to all ants
  clean_current_df();
}
