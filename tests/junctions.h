#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "warren/Feature.h"

using std::vector;

using testing::ElementsAre;

TEST(junctionsTest, getJunctions)
{
    Feature b;
    Feature c;
    Feature d;

    b.start = 10;
    b.end = 20;

    c.start = 30;
    c.end = 40;

    d.start = 60;
    d.end = 65;

    vector<Feature> ret;
    EXPECT_TRUE(getJunctions(ret));

    EXPECT_EQ("junction", ret.at(0).type);
    EXPECT_EQ("junction", ret.at(1).type);
    EXPECT_EQ(20, ret.at(0).start);
    EXPECT_EQ(30, ret.at(0).end);
    EXPECT_EQ(40, ret.at(1).start);
    EXPECT_EQ(60, ret.at(1).end);

    EXPECT_EQ(2, ret.size());
}

TEST(FeatureTest, spliceJunctions_reverse_strand)
{
    // Feature objects representing splice junctions will always have
    // a start position less than end position.

    // since GFF features on the reverse strand may come in opposite order,
    // we need to flip these positions around, and that's what we're testing here.

    Feature a;
    Feature b;
    Feature c;
    Feature d;

    b.type = "exon";
    b.start = 30;
    b.end = 40;

    c.type = "exon";
    c.start = 10;
    c.end = 20;

    d.type = "exon";
    d.start = 60;
    d.end = 65;

    a.children.push_back(b);
    a.children.push_back(c);
    a.children.push_back(d);

    vector<Feature> ret;
    EXPECT_TRUE(a.spliceJunctions(ret));

    EXPECT_EQ("junction", ret.at(0).type);
    EXPECT_EQ("junction", ret.at(1).type);
    EXPECT_EQ(20, ret.at(0).start);
    EXPECT_EQ(30, ret.at(0).end);
    EXPECT_EQ(40, ret.at(1).start);
    EXPECT_EQ(60, ret.at(1).end);

    EXPECT_EQ(2, ret.size());
}

TEST(FeatureTest, spliceJunctions_configurable_exon_types)
{
    // exons are sometimes defined with type names like 'pseudogenic_exon'.
    // allow configuring which types are exons via the Feature::spliceJunctions method

    Feature a;
    Feature b;
    Feature c;
    Feature d;
    Feature e;

    b.type = "exon";
    b.start = 30;
    b.end = 40;

    c.type = "exon_type_B";
    c.start = 10;
    c.end = 20;

    d.type = "exon";
    d.start = 60;
    d.end = 65;

    e.type = "ignored";
    e.start = 200;
    e.end = 250;

    a.children.push_back(b);
    a.children.push_back(c);
    a.children.push_back(d);
    a.children.push_back(e);

    vector<string> allowed_types;
    allowed_types.push_back("exon");
    allowed_types.push_back("exon_type_B");

    vector<Feature> ret;
    EXPECT_TRUE(a.spliceJunctions(ret, allowed_types));

    EXPECT_EQ("junction", ret.at(0).type);
    EXPECT_EQ("junction", ret.at(1).type);
    EXPECT_EQ(20, ret.at(0).start);
    EXPECT_EQ(30, ret.at(0).end);
    EXPECT_EQ(40, ret.at(1).start);
    EXPECT_EQ(60, ret.at(1).end);

    EXPECT_EQ(2, ret.size());
}
