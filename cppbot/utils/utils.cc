#include <fstream>
#include <sstream>
#include "Location.h"

bool operator==(Location& a, Location& b) {
	return (a.row == b.row && a.col == b.col);
}
