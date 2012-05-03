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

    void Index::add(Feature* f)
    {
        all_features.push_back(f);
        features_by_type.insert(std::make_pair(f->type, f));
    }

    Index::iterator Index::begin(void)
    {
        return all_features.begin();
    }

    Index::iterator Index::end(void)
    {
        return all_features.end();
    }

    size_t Index::size(void)
    {
        return all_features.size();
    }

    Index* Index::filterType(const string& t)
    {
        typedef std::map<string, Feature*>::iterator map_iter;

        Index* index = new Index;
        std::pair<map_iter, map_iter> range = features_by_type.equal_range(t);

        for (map_iter it = range.first; it != range.second; ++it)
        {
            index->add(it->second);
        }
        return index;
    }
}
