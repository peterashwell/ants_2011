#ifndef GENETICBOT_H
#define GENETICBOT_H

#include <string>
#include <fstream>

#include "utils/State.h"
#include "utils/AntManager.h"
#include "utils/LocalData.h"
#include "Genome.h"
#include "genes/Gene.h"

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

		float time_marker;	// marks times for logTime

	public: 
		static ofstream debug;		// logfile for debugging
		GeneticBot();
				
		void playGame();	//plays a single game of Ants
		
		void endTurn();		//indicates to the engine that it has made its moves

		void logTime(string message); // logs time elapsed with a message
};

#endif
