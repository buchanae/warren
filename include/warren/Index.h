#ifndef WARREN_INDEX_H
#define WARREN_INDEX_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "warren/Feature.h"

using std::map;
using std::multimap;
using std::string;
using std::vector;

class TypeIndex
{
    typedef map<string, Feature>::iterator map_iter;
    multimap<string, Feature> by_type;

    public:
        void type (const string&, vector<Feature>&);
        void add (Feature&);
};

class ChildrenIndex
{
    typedef map<string, Feature>::iterator map_iter;
    multimap<string, Feature> by_parent_ID;

    public:
        void add (Feature&);

        void childrenOf (Feature&, vector<Feature>&);
};

class UniquePositionIndex
{
    std::set<Feature, FeaturePositionComparator> features;

    public:
        void add(Feature&);

        void overlappingFeature(Feature&, std::vector<Feature>&);
        bool contains(Feature&);
        void all(std::vector<Feature>&);
        int count(void);
};

#endif
