#include "Feature.h"


using std::string;

namespace GFF {

    bool Feature::hasStrand( void ){
        return strand == '+' || strand == '-';
    }

    bool Feature::isRevStrand( void ){
        return strand == '-';
    }

    int Feature::getLength( void ){
        return end - start + 1;
    }
    
    string Feature::toString( void ){
        return "";
    }

    bool Feature::fromString( string ){
        return false;
    }
}
