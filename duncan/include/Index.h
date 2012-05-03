#ifndef _GFF_FEATUREINDEX_H
#define _GFF_FEATUREINDEX_H

#include <map>
#include <string>
#include <vector>

#include "Feature.h"

using std::string;
using std::vector;

namespace GFF
{
    class Index
    {
        public:
            typedef vector<Feature*>::iterator iterator;

            void add(Feature*);

            iterator begin(void);
            iterator end(void);
            size_t size(void);

            Index* filterType(const string&);
            // TODO Index* filterType(vector<string>);

        private:
            vector<Feature*> all_features;
            std::multimap<string, Feature*> features_by_type;
    };
}
#endif
