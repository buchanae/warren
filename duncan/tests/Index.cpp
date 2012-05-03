#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Feature.h"
#include "Index.h"

using testing::ElementsAre;
using testing::WhenSorted;
using namespace GFF;

TEST(IndexTest, size)
{
    Feature a;
    Feature b;
    Feature c;
    Index index;

    index.add(&a);
    index.add(&b);
    index.add(&c);

    EXPECT_EQ(3, (int)index.size());
}

TEST(IndexTest, iteration)
{
    Feature a;
    Feature b;
    Feature c;
    Index index;

    index.add(&a);
    index.add(&b);
    index.add(&c);

    std::vector<Feature*> features;
    Index::iterator it = index.begin();

    for (Index::iterator it = index.begin(); it != index.end(); ++it)
    {
        features.push_back(*it);
    }
    
    EXPECT_THAT(features, ElementsAre(&a, &b, &c));
}

TEST(IndexTest, filterType)
{
    Feature a;
    Feature b;
    Feature c;
    Index index;

    a.seqid = "one";
    b.seqid = "two";
    c.seqid = "three";

    a.type = "type1";
    b.type = "type2";
    c.type = "type1";

    index.add(&a);
    index.add(&b);
    index.add(&c);

    Index* type1_index = index.filterType("type1");
    vector<string> seqids;

    for (Index::iterator it = type1_index->begin(); it != type1_index->end(); ++it)
    {
        seqids.push_back((*it)->seqid);
    }
    EXPECT_THAT(seqids, WhenSorted(ElementsAre("one", "three")));

    seqids.clear();
    Index* type2_index = index.filterType("type2");

    for (Index::iterator it = type2_index->begin(); it != type2_index->end(); ++it)
    {
        seqids.push_back((*it)->seqid);
    }
    EXPECT_THAT(seqids, WhenSorted(ElementsAre("two")));
}

/*
    a.attributes.addFromGFF("ID=one;");
    b.attributes.addFromGFF("ID=two;Parent=one");
    c.attributes.addFromGFF("ID=three;Parent=one");
*/

// TODO test children() when feature doesn't have ID
