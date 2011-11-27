#include <iostream>
#include <string>

#include "utils/State.h"
#include "utils/AntManager.h"
#include "GeneticBot.h"

using namespace std;

//constructor
GeneticBot::GeneticBot() {
	// Redirect stderr and open log file for debugging
	freopen("errors.txt","w",stderr);
	debug.open("botlog.txt");
}


//plays a single game of Ants.
void GeneticBot::playGame()
{
	//reads the game parameters and sets up
	cin >> state;
		
	state.setup();
	genome.setup(state); // GENES DEFINED HERE
	local_data.setup(state);	
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
		local_data.newTurn(state);
		antmgr.newTurn(state);
		// Use the AntManager to gather move data from genes, battles etc.
		genome.express(state, local_data, antmgr);
		logTime("expressing all genes");
		//handleBattles(state, antboss); // TODO 
		antmgr.resolve_forces(state, local_data); // Issue orders for ants
		logTime("resolving forces");
		endTurn();
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

void GeneticBot::logTime(string message) {
	float new_time = state.timer.getTime();
	debug << message << " " << new_time - time_marker << "ms" << endl;
	time_marker = new_time;
}
