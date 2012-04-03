#ifndef _GFF_UTILS_H
#define _GFF_UTILS_H

#include <string>
#include <vector>


using std::vector;
using std::string;

vector<string>& split(const string& s, char delim, vector<string>& elems);

vector<string> split(const string &s, char delim);

#endif
