#ifndef WARREN_GFFREADER_H
#define WARREN_GFFREADER_H

#include <istream>
#include <fstream>
#include <string>

#include "warren/Feature.h"

using std::istream;
using std::string;

class GFFReader
{
    istream& input;

    public:
        GFFReader (istream& in) : input(in) {};

        bool read (Feature& feature)
        {
            string temp;
            Feature a;

            while (std::getline(input, temp).good())
            {
                if (a.initFromGFF(temp))
                {
                    feature = a;
                    return true;
                }
            }
            return false;
        }
};

#endif
