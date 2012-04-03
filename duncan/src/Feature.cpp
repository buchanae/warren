#include <sstream>
#include <stdlib.h>
#include <vector>

#include "Attribute.h"
#include "Feature.h"
#include "utils.h"


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
        std::stringstream out;
        out << seqid << "\t";
        out << source << "\t";
        out << type   << "\t";
        out << start  << "\t";
        out << end    << "\t";
        out << score  << "\t";
        out << strand << "\t";
        out << phase  << "\t";
        // TODO attributes here
        out << "";

        return out.str();
    }

    bool Feature::fromString( string in ){

        std::vector<string> cols = split( in, '\t' );

        if( cols.size() != 9 ) return false;

        seqid = cols.at(0);
        source = cols.at(1);
        type = cols.at(2);
        start = atoi(cols.at(3).c_str());
        end = atoi(cols.at(4).c_str());
        score = cols.at(5);
        strand = *(cols.at(6).c_str());
        phase = *(cols.at(7).c_str());
        std::vector<string> raw_attrs = split( cols.at(8), ';' );

        std::vector<string>::iterator it = raw_attrs.begin();
        for( ; it != raw_attrs.end(); ++it ){
            Attribute a;
            a.fromString( *it );
            attributes.push_back( a );
        }

        return true;
    }
}
