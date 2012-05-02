#include <iostream>
#include <string>

#include "Feature.h"
#include "Reader.h"


bool GFF::Reader::getNextFeature( std::istream& input, Feature& f ){

    string temp;
    Feature a;
    
    while( std::getline( input, temp ).good() ){
        if (a.initFromGFF(temp))
        {
            f = a;
            return true;
        }
    }

    return false;
}
