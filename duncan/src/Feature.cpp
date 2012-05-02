#include <sstream>
#include <stdlib.h>
#include <vector>

#include "Attributes.h"
#include "Feature.h"
#include "tokenizer.h"

using std::string;

namespace GFF
{
    Feature::Feature (void)
    {
        seqid = ".";
        source = ".";
        type = ".";
        start = 0;
        end = 0;
        score = ".";
        strand = '.';
        phase = '.';
        raw_attributes = ".";
    }

    Feature::Feature (const char * raw)
    {
        string s(raw);
        if (!initFromGFF(s)) throw "Invalid GFF string";
    }

    Feature::Feature (string& raw)
    {
        if (!initFromGFF(raw)) throw "Invalid GFF string";
    }

    bool Feature::initFromGFF(string& raw)
    {
        // split the tab-delimited columns
        std::vector<string> cols;
        tokenizer tokens(raw, separator("\t"));
        std::copy(tokens.begin(), tokens.end(), std::back_inserter(cols));

        if( cols.size() != 9 ) return false;

        seqid = cols.at(0);
        source = cols.at(1);
        type = cols.at(2);
        start = atoi(cols.at(3).c_str());
        end = atoi(cols.at(4).c_str());
        score = cols.at(5);
        strand = *(cols.at(6).c_str());
        phase = *(cols.at(7).c_str());

        raw_attributes = cols.at(8);
        attributes = Attributes();
        attributes.addFromGFF(raw_attributes);

        return true;
    }

    bool Feature::hasStrand( void ){
        return strand == '+' || strand == '-';
    }

    bool Feature::isRevStrand( void ){
        return strand == '-';
    }

    int Feature::getLength( void ){
        return end - start + 1;
    }
    
    /* TODO
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
    */
}
