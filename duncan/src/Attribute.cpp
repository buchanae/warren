#include "Attribute.h"


using std::string;

namespace GFF {

    bool Attribute::hasMultiple( void ){
        return false;
    }
    
    std::vector<string> Attribute::getAll( void ){
        std::vector<string> v;
        return v;
    }
}
