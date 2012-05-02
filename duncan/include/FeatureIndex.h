#ifndef _GFF_FEATUREINDEX_H
#define _GFF_FEATUREINDEX_H

#include <map>
#include <string>
#include <vector>

#include "Feature.h"
#include "FeatureList.h"

using std::multimap;
using std::string;
using std::vector;

namespace GFF
{
    class FeatureIndex
    {
        public:
            typedef map<string, Feature*>::iterator map_iter;
            typedef vector<Feature*>::iterator vector_iter;

            void add(Feature&);

            FeatureList* children(Feature&);
            //TODO FeatureList* parents(Feature&);
            FeatureList* type(string);
            //TODO FeatureList* types(vector<string>);

        private:
            // TODO need to store all?
            vector<Feature*> all;
            map<string, Feature*> by_ID;
            multimap<string, Feature*> by_parent_ID;
            multimap<string, Feature*> by_type;
    };
}

#endif
