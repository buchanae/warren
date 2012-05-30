#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "Feature.h"
#include "Index.h"

using std::string;
using std::vector;

namespace GFF
{
    void IndexBase::add(Feature& feature)
    {
        index(feature);
    }

    void IndexBase::add(Feature_iter it, Feature_iter end)
    {
        for (; it != end; ++it)
        {
            index(*it);
        }
    }

    void TypeIndex::index(Feature& feature)
    {
        by_type.insert(std::make_pair(feature.type, feature));
    }

    void TypeIndex::type(const string& t, vector<Feature>& features)
    {
        std::pair<map_iter, map_iter> range = by_type.equal_range(t);

        for (map_iter it = range.first; it != range.second; ++it)
        {
            features.push_back(it->second);
        }
    }

    void ParentChildIndex::index(Feature& feature)
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

    void ParentChildIndex::childrenOf(Feature& feature, vector<Feature>& children)
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
}
