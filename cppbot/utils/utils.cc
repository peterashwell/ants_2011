#include <fstream>
#include <sstream>
#include <string>
#include "Location.h"

bool operator==(Location& a, Location& b) {
	return (a.row == b.row && a.col == b.col);
}

std::string tostring(Location& loc) {
	std::stringstream ret;
	ret << "(" << loc.row << "," << loc.col << ")";
	return ret.str();
}
