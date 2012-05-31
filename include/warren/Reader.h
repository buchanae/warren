#ifndef WARREN_GFFREADER_H
#define WARREN_GFFREADER_H

#include <iostream>
#include <vector>

#include "warren/Feature.h"

class GFFReader
{
    bool getNextFeature(std::istream& input, Feature& feature);
    void readAllAndLinkChildren(std::istream& input, std::vector<Feature>& features);
}

#endif
