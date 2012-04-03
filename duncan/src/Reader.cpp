#include <iostream>
#include <string>

#include "Feature.h"
#include "Reader.h"


namespace GFF {

    bool Reader::getNextFeature( std::istream& input, Feature& f ){

        string temp;
        
        while( std::getline( input, temp ).good() ){
            // TODO this will modify f even if the line is invalid
            //      kinda sucks
            if ( f.fromString( temp ) ) return true;
        }

        return false;
    }

}
