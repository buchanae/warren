#include <vector>

#include "warren/Feature.h"
#include "warren/junctions.h"

using std::vector;

bool getJunctions(vector<Feature>& features, vector<Feature>& juncs)
{
    if (features.size() < 2) return false;

    FeaturePositionComparator compare_by_position;
    std::sort(features.begin(), features.end(), compare_by_position);

    Feature f;
    f.type = "junction";

    vector<Feature>::iterator feature = features.begin();
    while (feature != features.end())
    {
        f.start = feature->end;
        ++feature;
        if (feature != features.end())
        {
            f.seqid = feature->seqid;
            f.source = feature->source;
            f.strand = feature->strand;
            f.end = feature->start;
            juncs.push_back(f);
        }
    }
    return true;
}
