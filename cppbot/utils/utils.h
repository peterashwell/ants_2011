#include <fstream>
#include <string>
#include "Location.h"
bool operator==(Location&, Location&);
std::ostream& operator<<(std::ostream&, Location&);
std::string tostring(Location& loc);
