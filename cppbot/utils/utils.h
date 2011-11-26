#include "Location.h"

/* CONSTANTS */
static const int PLAYER = 0; // defines us as player

/* USEFUL FUNCTIONS */
bool operator==(Location& a, Location& b) {
	if (a.row == b.row && a.col == b.col) {
		return true;
	}
	return false;
}
