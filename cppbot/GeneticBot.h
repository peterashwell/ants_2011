#ifndef GENETICBOT_H
#define GENETICBOT_H

#include "utils/State.h"
#include "utils/AntManager.h"
#include "utils/LocalData.h"
#include "Genome.h"

/*
	This struct represents your bot in the game of Ants
*/
struct GeneticBot
{
	
	State state;
	AntManager antmgr;	// interprets genes and makes moves
	LocalData local_data;
	Genome genome;

	ofstream debug;		// logfile for debugging
	float time_marker;	// marks times for logTime

	GeneticBot();

	void playGame();	//plays a single game of Ants

	void makeMoves();	//makes moves for a single turn
	void endTurn();		//indicates to the engine that it has made its moves
		
	void logTime(String message); // logs time elapsed with a message
};

#endif GENETICBOT_H
