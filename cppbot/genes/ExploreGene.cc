#include "../utils/utils.cc"
#include "../utils/State.h"
#include "../utils/Location.h"
#include "ExploreGene.h"

void ExploreGene::disperse_once(State& state, LocalData& local_data)
{
	// Swap the buffer 
	swap_disp_buffer();

	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < cols; ++j)
		{
			Location cell_loc(i, j);
			// Compute the energy for the center square
			// Square is more attractive the longer it hasn't been seen
			int last_seen = local_data.turnsSinceSeen(state, i, j);
			float center_energy = 0.0f;

			if(last_seen != 0)
			{
				center_energy = pow(unex_attract * (1.0f - 1.0f / (float)last_seen), unex_decay);
				center_energy = min(1.0f, center_energy);
				disp_field_curr[i][j] = center_energy;
				continue;
			} 

			// Compute the sum of the adjacent squares and how many were used
			float adjacent_sum = 0;
			int adjacent_count = 0;

			for(int direction = 0; direction < TDIRECTIONS; ++direction)
			{
				Location adj = state.getLocation(cell_loc, direction);
				int adjacent_row = adj.row;
				int adjacent_col = adj.col;

				if(!state.grid[adjacent_row][adjacent_col].isWater) // If is passable
				{
					adjacent_sum += disp_field_prev[i][j];
					++adjacent_count;
				}
			}
			float adjacent_energy = 0.0f;

			if(adjacent_count > 0)
				adjacent_energy = adjacent_sum / adjacent_count;

			disp_field_curr[i][j] = disp_coeff * (center_energy + adjacent_energy);
		}
	}
}

void ExploreGene::express(State& s, LocalData& ld, AntManager& am) {
	for (int iter = 0; iter < DISPERSION_ITERATIONS; ++iter) {
		disperse_once(s, ld);
	}
	am.apply_field(s, disp_field_curr); // Apply current dispersion field to all ants
}


