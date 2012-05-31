#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "warren/Feature.h"
#include "warren/Index.h"

using testing::ElementsAre;
using testing::WhenSorted;

TEST(UniquePositionIndexTest, overlappingFeature)
{
    UniquePositionIndex index;

    // a and b are valid features and will be returned from the query
    Feature a;
    a.seqid = "one";
    a.source = "A";
    a.start = 20;
    a.end = 40;
    index.add(a);

    Feature b;
    b.start = 60;
    b.end = 80;
    b.seqid = "one";
    b.source = "B";
    index.add(b);

    // The query range will fall in the middle of this feature,
    // so it won't be returned.
    // i.e. valid features must fall entirely within the query range
    Feature c;
    c.seqid = "one";
    c.source = "C";
    c.start = 100;
    c.end = 150;
    index.add(c);

    // duplicate of Feature a.
    // UniquePositionIndex only returns unique features,
    // so this won't be returned as a duplicate
    Feature d;
    d.seqid = "one";
    d.start = 20;
    d.end = 40;
    index.add(d);

    // will fall in a valid range, but has a different seqid than the query
    // so won't be returned as overlapping query feature
    Feature e;
    e.seqid = "two";
    e.start = 20;
    e.end = 40;
    index.add(e);

    Feature query;
    query.seqid = "one";
    query.start = 10;
    query.end = 110;

    std::vector<Feature> ret;
    index.overlappingFeature(query, ret);
    
    std::vector<std::string> sources;
    for (std::vector<Feature>::iterator it = ret.begin(); it != ret.end(); ++it)
    {
        sources.push_back((*it).source);
    }

    EXPECT_THAT(sources, WhenSorted(ElementsAre("A", "B")));
}

TEST(UniquePositionIndexTest, all)
{
    // UniquePositionIndex::all() allows getting all unique features in this index
    Feature f;
    UniquePositionIndex index;

    f.seqid = "Chr1";
    f.source = "A";
    f.start = 10;
    f.end = 20;

    index.add(f);
    // add a duplicate
    f.source = "B";
    index.add(f);

    f.source = "C";
    f.start = 15;
    index.add(f);

    f.source = "D";
    f.seqid = "Chr2";
    index.add(f);

    std::vector<std::string> sources;
    std::vector<Feature> ret;
    index.all(ret);

    for (std::vector<Feature>::iterator it = ret.begin(); it != ret.end(); ++it)
    {
        sources.push_back((*it).source);
    }
    EXPECT_THAT(sources, WhenSorted(ElementsAre("A", "C", "D")));

    // test count()
    EXPECT_EQ(3, index.count());
}
