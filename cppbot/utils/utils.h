#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include "Location.h"
bool operator==(Location&, Location&);
std::ostream& operator<<(std::ostream&, Location&);
std::string tostring(Location& loc);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
template <class T> bool from_string(T &t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  bool success = !(iss >> f >> t).fail();
  return success;
}
