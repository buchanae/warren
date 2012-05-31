#ifndef WARREN_GFFREADER_H
#define WARREN_GFFREADER_H

#include <istream>
#include <vector>

#include "warren/Feature.h"

using std::istream;
using std::vector;

class GFFReader
{
    bool getNextFeature (istream& input, Feature& feature);
    void readAllAndLinkChildren (istream& input, vector<Feature>& features);
};

#endif
