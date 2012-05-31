#ifndef WARREN_STACKREADER_H
#define WARREN_STACKREADER_H

#include <iostream>

#include "warren/Feature.h"

class StackReader
{
    bool getNextFeature(std::istream&, Feature&);
};

#endif
