#include <string>
#include <sstream>
#include <vector>


using std::vector;
using std::string;

vector<string>& split(const string& s, char delim, vector<string>& elems) {

    std::stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {

    vector<string> elems;
    return split(s, delim, elems);
}
