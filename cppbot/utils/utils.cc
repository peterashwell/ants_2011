#include "utils.h"
#include "Location.h"

using namespace std;

bool operator==(Location& a, Location& b) {
	return (a.row == b.row && a.col == b.col);
}

std::string tostring(Location& loc) {
	std::stringstream ret;
	ret << "(" << loc.row << "," << loc.col << ")";
	return ret.str();
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}
