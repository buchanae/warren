#ifndef _GFF_ATTRIBUTE_H
#define _GFF_ATTRIBUTE_H

#include <string>;
#include <vector>;

using std::string;


namespace GFF {

    class Attribute {
        public:
            string name;
            string value;

            bool hasMultiple( void );
            std::vector<string> getAll( void );
            string toString( void );
            bool fromString( string );
    };
}

#endif
