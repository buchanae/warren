#ifndef _GFF_READER_H
#define _GFF_READER_H

#include <iostream>
#include <fstream>
#include <vector>

#include "Feature.h"

namespace GFF
{
    namespace Reader
    {
        bool getNextFeature(std::istream&, Feature&);
        void readAllAndLinkChildren(std::istream& input, std::vector<Feature>&);
    }
}

#endif
