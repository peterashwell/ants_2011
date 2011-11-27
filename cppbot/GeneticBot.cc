#include <iostream>
#include <string>

#include "utils/State.h"
#include "utils/AntManager.h"
#include "GeneticBot.h"

using namespace std;

//constructor
GeneticBot::GeneticBot() {
	// Redirect stderr and open log file for debugging
	freopen("debug/errors.txt","w",stderr);
	debug.open("debug/botlog.txt");
}


//plays a single game of Ants.
void GeneticBot::playGame()
{
	//reads the game parameters and sets up
	cin >> state;
	Timer timer;	
	state.setup();
	genome.setup(state); // GENES DEFINED HERE
	local_data.setup(state);	
	endTurn();

	//continues making moves while the game is not over
	while(cin >> state)
	{
		timer.start();
		debug << "turn " << state.turn << ":" << endl;
		debug << state << endl;
		debug << "TIME (no actions): " << timer.getTime() << endl;
		// Update state data with new pieces of food and vision etc.
		state.updateVisionInformation();
		debug << "msg1" << endl;
		// Prepare local data for the coming turn
		local_data.newTurn(state);
		debug << "msg2" << endl;
		antmgr.newTurn(state);
		debug << "TIME (updating data): " << timer.getTime() << endl;
		debug << "msg3" << endl;
		// Use the AntManager to gather move data from genes, battles etc.
		genome.express(state, local_data, antmgr);
		debug << "TIME (expressed genes): " << timer.getTime() << endl;
		debug << "msg4" << endl;
		//handleBattles(state, antboss); // TODO 
		antmgr.resolve_forces(state, local_data); // Issue orders for ants
		debug << "TIME (resolved forces): " << timer.getTime() << endl;
		debug << "msg4" << endl;
		endTurn();
		debug << "msg5" << endl;
	}
};

//finishes the turn
void GeneticBot::endTurn()
{
	debug << "msg6" << endl;
	debug.flush();
	// Reset the state data for next turn
	if(state.turn > 0)
		state.reset(debug);
	state.turn++;
	debug << "msg7" << endl;
	debug.flush();

	cout << "go" << endl;
};

