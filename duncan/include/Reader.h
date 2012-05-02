#ifndef _GFF_READER_H
#define _GFF_READER_H

#include <iostream>
#include <fstream>
#include <string>

#include "Feature.h"

using std::string;

namespace GFF {
    namespace Reader {

        bool getNextFeature( std::istream&, Feature& );
    }
}

#endif
