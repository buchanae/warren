#include <map>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Feature.h"
#include "FeatureList.h"

using std::map;
using std::make_pair;
using std::string;
using std::vector;
using testing::ElementsAre;
using testing::WhenSorted;
using namespace GFF;

TEST( FeatureListTest, vector_list )
{
    Feature a;
    Feature b;
    vector<Feature*> v;

    a.seqid = "one";
    b.seqid = "two";

    v.push_back(&a);
    v.push_back(&b);

    FeatureList* l = new VectorFeatureList(v.begin(), v.end());

    Feature r;
    EXPECT_TRUE(l->getNextFeature(r));
    EXPECT_EQ("one", r.seqid);

    EXPECT_TRUE(l->getNextFeature(r));
    EXPECT_EQ("two", r.seqid);

    EXPECT_FALSE(l->getNextFeature(r));
}

TEST( FeatureListTest, map_list )
{
    Feature a;
    Feature b;
    map<string, Feature*> m;

    a.seqid = "one";
    b.seqid = "two";

    m.insert(make_pair("foo", &a));
    m.insert(make_pair("bar", &b));

    FeatureList* l = new MapFeatureList(m.begin(), m.end());

    vector<string> seqids;
    Feature r;
    EXPECT_TRUE(l->getNextFeature(r));
    seqids.push_back(r.seqid);
    EXPECT_TRUE(l->getNextFeature(r));
    seqids.push_back(r.seqid);
    EXPECT_FALSE(l->getNextFeature(r));

    //EXPECT_THAT(seqids, WhenSorted(ElementsAre("one", "two")));
}
