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
        typedef Feature Record;

        GFFReader ();
        GFFReader (istream& in) : input(in) {};

        bool open (const char* file_path)
        {
            std::ifstream in(file_path);
            return in.good();
        }

        bool open (string file_path)
        {
            return open(file_path.c_str());
        }

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
