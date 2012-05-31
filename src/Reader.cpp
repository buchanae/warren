#include <iostream>
#include <string>
#include <vector>

#include "warren/Feature.h"
#include "warren/Index.h"
#include "warren/Reader.h"

using std::istream;
using std::vector;
using std::string;

bool GFFReader::getNextFeature(istream& input, Feature& f)
{
    string temp;
    Feature a;

    while (std::getline(input, temp).good())
    {
        if (a.initFromGFF(temp))
        {
            f = a;
            return true;
        }
    }
    return false;
}

void GFFReader::readAllAndLinkChildren(istream& input, vector<Feature>& features)
{
    Feature f;
    ChildrenIndex index;

    while (getNextFeature(input, f))
    {
        features.push_back(f);
        index.add(f);
    }

    vector<Feature>::iterator it = features.begin();
    for (; it != features.end(); ++it)
    {
        index.childrenOf(*it, it->children);
    }
}
