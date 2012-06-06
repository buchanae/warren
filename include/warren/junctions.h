#ifndef WARREN_JUNCTIONS_H
#define WARREN_JUNCTIONS_H

#include <vector>

#include "warren/Feature.h"

using std::vector;

bool getJunctions(vector<Feature>& features, vector<Feature>& juncs);

void nonOverlappingJunctionCombos (vector<Feature>& junctions,
                                   vector<vector<Feature> >& output);

#endif
