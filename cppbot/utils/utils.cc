#ifndef UTILS_H
#define UTILS_H

#include "Location.h"

/* CONSTANTS */
static const int PLAYER = 0; // defines us as player
// last turn squares were seen at start of game
static const int DEFAULT_TURNS_SINCE_VISIBLE = -100;
/* USEFUL FUNCTIONS */
bool operator==(Location& a, Location& b) {
	if (a.row == b.row && a.col == b.col) {
		return true;
	}
	return false;
}

#endif
