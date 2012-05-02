#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "boost/iterator/transform_iterator.hpp"

#include "Feature.h"
#include "FeatureIndex.h"

using std::make_pair;
using std::multimap;
using std::string;
using std::vector;

namespace GFF
{

    void FeatureIndex::add(Feature& f)
    {
        all.push_back(&f);

        // index by ID
        string ID;
        if (f.attributes.get("ID", ID))
        {
            by_ID.insert(make_pair(ID, &f));
        }

        // index by parent IDs
        vector<string> parent_IDs;
        if (f.attributes.all("Parent", parent_IDs))
        {
            vector<string>::iterator IDs_it = parent_IDs.begin();
            for (; IDs_it != parent_IDs.end(); ++IDs_it)
            {
                by_parent_ID.insert(make_pair(*IDs_it, &f));
            }
        }

        // index by type
        by_type.insert(make_pair(f.type, &f));
    }

    FeatureList* FeatureIndex::children(Feature& f)
    {
        string ID;
        f.attributes.get("ID", ID);
        std::pair<map_iter, map_iter> range = by_parent_ID.equal_range(ID);
        return new MapFeatureList(range.first, range.second);
    }

    FeatureList* FeatureIndex::type(string t)
    {
        std::pair<map_iter, map_iter> range = by_type.equal_range(t);
        return new MapFeatureList(range.first, range.second);
    }
}
