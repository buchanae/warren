#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "warren/Feature.h"
#include "warren/Index.h"

using testing::ElementsAre;
using testing::WhenSorted;

TEST(TypeIndexTest, filter_type)
{
    typedef vector<Feature>::iterator Feature_iter;

    Feature a;
    Feature b;
    Feature c;

    a.seqid = "one";
    b.seqid = "two";
    c.seqid = "three";

    a.type = "type1";
    b.type = "type2";
    c.type = "type1";

    TypeIndex index;
    index.add(a);
    index.add(b);
    index.add(c);

    vector<Feature> ret;
    index.type("type1", ret);

    vector<string> seqids;

    for (Feature_iter it = ret.begin(); it != ret.end(); ++it)
    {
        seqids.push_back(it->seqid);
    }
    EXPECT_THAT(seqids, WhenSorted(ElementsAre("one", "three")));

    ret.clear();
    seqids.clear();

    index.type("type2", ret);

    for (Feature_iter it = ret.begin(); it != ret.end(); ++it)
    {
        seqids.push_back(it->seqid);
    }
    EXPECT_THAT(seqids, WhenSorted(ElementsAre("two")));
}

TEST(ChildrenIndexTest, children)
{
    typedef vector<Feature>::iterator Feature_iter;

    Feature a;
    Feature b;
    Feature c;

    a.attributes.addFromGFF("ID=one;");
    b.attributes.addFromGFF("ID=two;Parent=one");
    c.attributes.addFromGFF("ID=three;Parent=one");

    ChildrenIndex index;
    index.add(a);
    index.add(b);
    index.add(c);

    vector<Feature> ret;
    index.childrenOf(a, ret);

    vector<string> IDs;

    for (Feature_iter it = ret.begin(); it != ret.end(); ++it)
    {
        string ID = "";
        it->attributes.get("ID", ID);
        IDs.push_back(ID);
    }
    EXPECT_THAT(IDs, WhenSorted(ElementsAre("three", "two")));

    ret.clear();
    index.childrenOf(b, ret);
    EXPECT_EQ(0, ret.size());
}

// TODO test children() when feature doesn't have ID
