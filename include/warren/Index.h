#ifndef WARREN_INDEX_H
#define WARREN_INDEX_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include "warren/Feature.h"

using std::map;
using std::multimap;
using std::set;
using std::string;
using std::vector;

class TypeIndex
{
    typedef map<string, Feature>::iterator map_iter;
    multimap<string, Feature> by_type;

    public:
        void add (Feature& feature);

        void type (const string& query, vector<Feature>& features);
};

class ChildrenIndex
{
    typedef map<string, Feature>::iterator map_iter;
    multimap<string, Feature> by_parent_ID;
    set<string> parent_IDs;

    public:
        void add (Feature&);

        void childrenOf (Feature& query, vector<Feature>&);
        void childrenOf (string& ID, vector<Feature>&);

        void parentIDs (vector<string>& IDs);
};

class UniquePositionIndex
{
    set<Feature, FeaturePositionComparator> features;

    public:
        void add(Feature&);

        void overlappingFeature (Feature& query, vector<Feature>& overlaps);
        bool contains (Feature& query);
        void all (std::vector<Feature>& features);
        int count (void);
};

#endif
