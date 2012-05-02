#include "Feature.h"
#include "FeatureIndex.h"

namespace GFF
{
    bool FeatureList::getNextFeature(Feature&)
    {
        return false;
    }

    VectorFeatureList::VectorFeatureList(vector_iter begin, vector_iter e)
    {
        iter = begin;
        end = e;
    }

    bool VectorFeatureList::getNextFeature(Feature& f)
    {
        if (iter == end) return false;
        f = **iter;
        iter++;
        return true;
    }

    MapFeatureList::MapFeatureList(map_iter begin, map_iter e)
    {
        iter = begin;
        end = e;
    }

    bool MapFeatureList::getNextFeature(Feature& f)
    {
        if (iter == end) return false;
        f = *(iter->second);
        iter++;
        return true;
    }
}
