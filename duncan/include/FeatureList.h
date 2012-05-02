#ifndef _GFF_FEATURELIST_H
#define _GFF_FEATURELIST_H

#include <map>
#include <string>
#include <vector>

#include "Feature.h"

using std::map;
using std::string;
using std::vector;

namespace GFF
{
    class FeatureList
    {
        public:
            typedef map<string, Feature*>::iterator map_iter;
            typedef vector<Feature*>::iterator vector_iter;

            virtual bool getNextFeature(Feature&);
    };

    class VectorFeatureList : public FeatureList
    {
        public:
            VectorFeatureList(vector_iter, vector_iter);
            virtual bool getNextFeature(Feature&);

        private:
            vector_iter iter;
            vector_iter end;
    };

    class MapFeatureList : public FeatureList
    {
        public:
            MapFeatureList(map_iter, map_iter);
            virtual bool getNextFeature(Feature&);

        private:
            map_iter iter;
            map_iter end;
    };
}
#endif
