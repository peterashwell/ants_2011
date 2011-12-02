#include "ExploreGene.h"

#include "../utils/State.h"
#include "../utils/Location.h"

void ExploreGene::disperse_once(State& state, LocalData& local_data)
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
			// Default center energy from previous iter
      float center_energy = disp_field_prev[i][j];;
			
			// Compute the energy for the center square
      // Square is more attractive the longer it hasn't been seen
			int turns_since_seen = local_data.turnsSinceSeen(state, i, j);
			if(turns_since_seen > unseen_limit) // check to see if it is a source of energy
			{
				center_energy = pow(unex_attract * (1.0f - 1.0f / (float)turns_since_seen), unex_decay);
				center_energy = min(unex_attract, center_energy);
				disp_field_curr[i][j] = center_energy;
				continue;
			} 

			// Compute the sum of the adjacent squares and how many were used
			float adj_sum = 0;
			int adj_count = 0;

			for(int direction = 0; direction < TDIRECTIONS; ++direction)
			{
				Location adj = state.getLocation(cell_loc, direction);
				//cerr << adj << " is adjacent to " << cell_loc << endl;
				int adj_row = adj.row;
				int adj_col = adj.col;

				if(!state.grid[adj_row][adj_col].isWater) // If is passable
				{

					adj_sum += disp_field_prev[adj_row][adj_col] - center_energy;
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

void ExploreGene::express(State& s, LocalData& ld, AntManager& am) {
	for (int iter = 0; iter < EXPGENE_DISPERSION_ITERATIONS; ++iter) {
		disperse_once(s, ld);
	}
	dump_current_df(state.turn); 
	am.apply_field(s, disp_field_curr); // Apply current dispersion field to all ants
}


