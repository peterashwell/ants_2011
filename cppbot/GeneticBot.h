#ifndef GENETICBOT_H
#define GENETICBOT_H


#include <string>
#include <fstream>

#include "Genome.h"
#include "utils/State.h"
#include "utils/AntManager.h"
#include "utils/LocalData.h"
#include "genes/Gene.h"
#include "Bug.h"

/*
	This struct represents your bot in the game of Ants
*/
class GeneticBot
{
	private:
		State state;
		AntManager antmgr;	// interprets genes and makes moves
		LocalData local_data;
		Genome genome;
		Bug debug; // use this stupid thing
		float time_marker;	// marks times for logTime

	public: 
		GeneticBot();
				
		void playGame();	//plays a single game of Ants
		
		void endTurn();		//indicates to the engine that it has made its moves

		void logTime(string message); // logs time elapsed with a message
};

#endif
