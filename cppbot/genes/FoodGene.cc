#include "FoodGene.h"

#include "../utils/LocalData.h"
#include "../utils/State.h"
#include "../utils/Location.h"

void FoodGene::disperse_once(State& state, LocalData& local_data)
{
	// Swap the buffer 
	swap_disp_buffer();
	for(int i = 0; i < rows; ++i)
	{
		for(int j = 0; j < cols; ++j)
		{
      if (state.grid[i][j].isWater) { // TODO replace all iswaters with function from localdata
        disp_field_curr[i][j] = 0;
        continue; // Don't compute dispersion for this square
      }
			Location cell_loc(i, j);
			// Default center energy from previous iter
      float center_energy = disp_field_prev[i][j];
			// Compute the energy for the center square
      // Square is more attractive the longer it hasn't been seen
      // For food, adjacent cells are equally attractive
			if (local_data.is_food[i][j]) {
      	center_energy = food_attract;
        for(int direction = 0; direction < TDIRECTIONS; ++direction) {
          Location adj = state.getLocation(cell_loc, direction);
          if(!local_data.is_water[adj.row][adj.col]) // If is passable
          {
            disp_field_curr[adj.row][adj.col] = food_attract;
          }
        }
        disp_field_curr[i][j] = food_attract;
        continue;
			} 

			// Compute the sum of the adjacent squares and how many were used
			float adj_sum = 0;
			int adj_count = 0;

			for(int direction = 0; direction < TDIRECTIONS; ++direction)
			{
				Location adj = state.getLocation(cell_loc, direction);
				if(!local_data.is_water[i][j]) // If is passable
				{
					adj_sum += disp_field_prev[adj.row][adj.col] - center_energy;
					++adj_count;
				}
			}
			float adj_energy = 0.0f;

			if(adj_count > 0) {
				adj_energy = adj_sum / adj_count;
			}
      float collab_coeff = 1;
      if (state.grid[i][j].ant == 0) { // TODO define in constants (0 = us)
        collab_coeff = FOODGENE_COLLAB_COEFF; // moderate competition for food
      }
			disp_field_curr[i][j] = collab_coeff * disp_coeff * (center_energy + adj_energy);
		}
	}
	dump_current_df(state.turn); 
}

void FoodGene::express(State& s, LocalData& ld, AntManager& am) {
	for (int iter = 0; iter < FOODGENE_DISPERSION_ITERATIONS; ++iter) {
		disperse_once(s, ld);
	}
	am.apply_field(s, disp_field_curr); // Apply current dispersion field to all ants
}


