#include <fstream>
#include "Location.h"

static std::ofstream debug;
bool operator==(Location&, Location&);
std::ofstream& operator<<(std::ofstream&, Location&);
