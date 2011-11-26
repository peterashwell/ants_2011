#include "GeneticBot.h"
#include <stdio.h>

using namespace std;

//constructor
GeneticBot::GeneticBot() {
	freopen("errors.txt","w",stderr);
	debug.open("botlog.txt");
}


//plays a single game of Ants.
void GeneticBot::playGame()
{
	//reads the game parameters and sets up
	cin >> state;
	// redirect stderr to errors.txt file
	 
	state.setup();
	endTurn();

	//continues making moves while the game is not over
	while(cin >> state)
	{
		debug << "turn " << state.turn << ":" << endl;
		debug << state << endl;
		logTime("no actions yet: ");  
		// Update state data with new pieces of food and vision etc.
		state.updateVisionInformation();
		logTime("updating vision data: ");
		
		// Prepare local data for the coming turn
		local_data.prepare();
		antmgr.prepare()
		// Use the AntManager to gather move data from genes, battles etc.
		genome.express(state, antmgr);
		logTime("expressing all genes");
		//handleBattles(state, antboss); // TODO 
		antmgr.resolve_forces(); // Issue orders for ants
		logTime("resolving forces");
		endTurn();
	}
};

//makes the bots moves for the turn
void GeneticBot::expressGenes()
{
	for (int g = 0; g < num_genes, g++) {
		genome[g].express(state)
	}
};

//finishes the turn
void GeneticBot::endTurn()
{
	// Reset the state data for next turn
	if(state.turn > 0)
		state.reset();
	state.turn++;
	logTime("resetting state data");


	cout << "go" << endl;
};

void GeneticBot::logTime(String message) {
	float new_time = state.timer.getTime();
	debug << message << " " << new_time - time_marker << "ms" << endl;
	time_marker = new_time;
}
