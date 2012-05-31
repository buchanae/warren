#include <iostream>
#include <vector>
#include <string>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "warren/Feature.h"
#include "warren/StackReader.h"
#include "warren/tokenizer.h"

using std::string;

bool StackReader::getNextFeature(std::istream& input, Feature& f)
{
    string line;

    while (std::getline(input, line).good())
    {
        if (boost::starts_with(line, "@"))
        {
            std::vector<string> cols;
            tokenizer tokens(line, separator("\t"));
            cols.insert(cols.end(), tokens.begin(), tokens.end());

            f.seqid = cols.at(0).substr(1);            
            f.type = "splice_junction";
            f.start = boost::lexical_cast<int>(cols.at(2));
            f.end = boost::lexical_cast<int>(cols.at(3));

            return true;
        }
    }
    return false;
}
