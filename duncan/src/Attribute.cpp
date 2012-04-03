#include <vector>

#include "Attribute.h"
#include "utils.h"


using std::string;

namespace GFF {

    bool Attribute::hasMultiple( void ){
        return false;
    }
    
    std::vector<string> Attribute::getAll( void ){
        std::vector<string> v;
        return v;
    }

    bool Attribute::fromString( string in ){

        std::vector<string> sp = split( in, '=' );

        if( sp.size() != 2 ) return false;

        name = sp.at(0);
        value = sp.at(1);

        return true;
    }
}
