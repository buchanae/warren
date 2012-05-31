#include <map>
#include <string>
#include <vector>

#include "Feature.h"
#include "Index.h"

using std::string;
using std::vector;

void TypeIndex::add (Feature& feature)
{
    by_type.insert(std::make_pair(feature.type, feature));
}

void TypeIndex::type (const string& t, vector<Feature>& features)
{
    std::pair<map_iter, map_iter> range = by_type.equal_range(t);

    for (map_iter it = range.first; it != range.second; ++it)
    {
        features.push_back(it->second);
    }
}

void ChildrenIndex::add (Feature& feature)
{
    vector<string> parent_IDs;
    if (feature.attributes.all("Parent", parent_IDs))
    {
        vector<string>::iterator IDs_it = parent_IDs.begin();
        for (; IDs_it != parent_IDs.end(); ++IDs_it)
        {
            by_parent_ID.insert(std::make_pair(*IDs_it, feature));
        }
    }
}

void ChildrenIndex::childrenOf (Feature& feature, vector<Feature>& children)
{
    string ID;
    if (feature.attributes.get("ID", ID))
    {
        std::pair<
            std::multimap<string, Feature>::iterator,
            std::multimap<string, Feature>::iterator
        > range = by_parent_ID.equal_range(ID);

        std::multimap<string, Feature>::iterator rit = range.first;
        for (; rit != range.second; ++rit)
        {
            children.push_back(rit->second);
        }
    }
}

// TODO fix GFF lib to make this const Feature& f
void UniquePositionIndex::add (Feature& f)
{
    features.insert(f);
}

void UniquePositionIndex::overlappingFeature (Feature& query, vector<Feature>& overlaps)
{
    Feature lower;
    lower.seqid = query.seqid;
    lower.start = query.start;
    lower.end = query.start;

    Feature upper;
    upper.seqid = query.seqid;
    upper.start = query.end;
    upper.end = query.end;

    typedef std::set<Feature>::iterator iter;
    iter it = features.lower_bound(lower);
    iter end = features.upper_bound(upper);
    for (; it != end; ++it)
    {
        if ((*it).end < target.end) overlaps.push_back(*it);
    }
}

bool UniquePositionIndex::contains (Feature& query)
{
    return features.find(query) != features.end();
}

void UniquePositionIndex::all (vector<Feature>& ret)
{
    ret.insert(ret.end(), features.begin(), features.end());
}

int UniquePositionIndex::count (void)
{
    return features.size();
}
