#include <fstream>
#include <string>
#include <stringstream>
#include "Location.h"
#ifndef UTILS_H
#define UTILS_H

/* CONSTANTS */
static const int PLAYER = 0; // defines us as player
// last turn squares were seen at start of game
static const int DEFAULT_TURNS_SINCE_VISIBLE = -100;

void dump_df(float** df, int rows, int cols, string id, int turn) {
	ofstream out;
	stringstream fname;
	fname << "dflogs/" << id << "/" << turn << ".df";		
	out.open(fname, 'w');
	for (int r = 0; r < rows; ++r) {
		
	}
}

/*
bool operator==(Location& a, Location& b) {
	if (a.row == b.row && a.col == b.col) {
		return true;
	}
	return false;
}*/

#endif
