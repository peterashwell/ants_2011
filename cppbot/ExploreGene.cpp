#include "ExploreGene.h"

void ExploreGene::disperse_once(State state)
{
  // Swap the buffer 
  swap_disp_buffer();

  for(int i = 0; i < rows; ++i)
  {
    for(int j = 0; j < cols; ++j)
    {
			// Compute the energy for the center square
			// Square is more attractive the longer it hasn't been seen
      int last_seen = state.turns_since_visible(i, j);
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
        int adjacent_row = wrap_rows(i + DIRECTIONS[direction][0]);
        int adjacent_col = wrap_cols(j + DIRECTIONS[direction][1]);

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