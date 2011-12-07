#include "BruteCombatGene.h"

#include "../utils/State.h"
#include "../utils/Location.h"

void BruteCombatGene::disperse_once(State& state, LocalData& local_data)
{
  // Identify ants which can begin a fight
  // Any square adjacent to an ant which is within attack radius of a square adjacent to an enemy ant

  // First identify the cells adjacent to my ants that are valid moves (not water)
  // TODO code neatness and efficiency
  vector<Location> my_moves;
  vector<Location>::iterator my_ant;
  for (my_ant = state.myAnts.begin(); my_ant != state.myAnts.end(); ++my_ant) {
    for (int d = 0; d < TDIRECTIONS; ++d) {
      Location adj = state.getLocation(*my_ant, d);
      if (!local_data.is_water[adj.row][adj.col]) {
        my_moves.push_back(adj);
      }
    }
    // can also keep it still
    my_moves.push_back(*my_ant); // TODO efficiency
  }
  vector<Location> enemy_moves;
  vector<Location>::iterator enemy_ant;
  for (enemy_ant = state.enemyAnts.begin(); enemy_ant != state.enemyAnts.end(); ++enemy_ant) {
    for (int d = 0; d < TDIRECTIONS; ++d) {
      Location adj = state.getLocation(*enemy_ant, d);
      if (!local_data.is_water[adj.row][adj.col]) {
        enemy_moves.push_back(adj);
      }
    }
    enemy_moves.push_back(*enemy_ant); // TODO efficiency
  }
  
  // Filter the two arrays into moves which are potentially in range of each other
   
 
 
  /*
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
      if (state.grid[i][j].isHill == 1 && state.grid[i][j].hillPlayer != 0) { // enemy hill
        disp_field_curr[i][j] = hill_attract;
        continue;
      }
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
  */
	///dump_current_df(state.turn); 
}

void BruteCombatGene::express(State& s, LocalData& ld, AntManager& am) {
/*
	for (int iter = 0; iter < dispersion_iterations; ++iter) {
		disperse_once(s, ld);
	}
	am.apply_field(s, disp_field_curr); // Apply current dispersion field to all ants
  */
}
