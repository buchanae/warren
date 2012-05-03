#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Feature.h"
#include "FeatureIndex.h"
#include "FeatureList.h"

using testing::ElementsAre;
using testing::WhenSorted;
using namespace GFF;

TEST(FeatureIndexTest, children)
{
    Feature a;
    Feature b;
    Feature c;
    FeatureIndex index;

    a.attributes.addFromGFF("ID=one;");
    b.attributes.addFromGFF("ID=two;Parent=one");
    c.attributes.addFromGFF("ID=three;Parent=one");

    index.add(a);
    index.add(b);
    index.add(c);

    FeatureList* list = index.children(a);
    Feature r;

    string ID;
    EXPECT_TRUE(list->getNextFeature(r));
    r.attributes.get("ID", ID);
    EXPECT_EQ("two", ID);
    EXPECT_TRUE(list->getNextFeature(r));
    r.attributes.get("ID", ID);
    EXPECT_EQ("three", ID);
    EXPECT_FALSE(list->getNextFeature(r));
}

TEST(FeatureIndexTest, type)
{
    Feature a;
    Feature b;
    Feature c;
    FeatureIndex index;

    a.seqid = "one";
    b.seqid = "two";
    c.seqid = "three";

    a.type = "type1";
    b.type = "type2";
    c.type = "type1";

    index.add(a);
    index.add(b);
    index.add(c);

    FeatureList* list = index.type("type1");
    Feature r;
    vector<string> seqids;

    EXPECT_TRUE(list->getNextFeature(r));
    seqids.push_back(r.seqid);
    EXPECT_TRUE(list->getNextFeature(r));
    seqids.push_back(r.seqid);
    EXPECT_FALSE(list->getNextFeature(r));

    EXPECT_THAT(seqids, WhenSorted(ElementsAre("one", "three")));

    list = index.type("type2");
    seqids.clear();
    EXPECT_TRUE(list->getNextFeature(r));
    seqids.push_back(r.seqid);
    EXPECT_FALSE(list->getNextFeature(r));
    EXPECT_THAT(seqids, WhenSorted(ElementsAre("two")));
}
