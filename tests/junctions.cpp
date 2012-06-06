#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "warren/Feature.h"
#include "warren/junctions.h"

using std::vector;

using testing::ElementsAre;
using testing::Field;

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

    vector<Feature> juncs;
    juncs.push_back(b);
    juncs.push_back(c);
    juncs.push_back(d);

    vector<Feature> ret;
    EXPECT_TRUE(getJunctions(juncs, ret));

    EXPECT_EQ("junction", ret.at(0).type);
    EXPECT_EQ("junction", ret.at(1).type);
    EXPECT_EQ(20, ret.at(0).start);
    EXPECT_EQ(30, ret.at(0).end);
    EXPECT_EQ(40, ret.at(1).start);
    EXPECT_EQ(60, ret.at(1).end);

    EXPECT_EQ(2, ret.size());
}

TEST(junctionsTest, getJunctions_reverse_strand)
{
    // Feature objects representing splice junctions will always have
    // a start position less than end position.

    // since GFF features on the reverse strand may come in opposite order,
    // we need to flip these positions around, and that's what we're testing here.

    Feature b;
    Feature c;
    Feature d;

    b.start = 30;
    b.end = 40;

    c.start = 10;
    c.end = 20;

    d.start = 60;
    d.end = 65;

    vector<Feature> juncs;
    juncs.push_back(b);
    juncs.push_back(c);
    juncs.push_back(d);

    vector<Feature> ret;
    EXPECT_TRUE(getJunctions(juncs, ret));

    EXPECT_EQ("junction", ret.at(0).type);
    EXPECT_EQ("junction", ret.at(1).type);
    EXPECT_EQ(20, ret.at(0).start);
    EXPECT_EQ(30, ret.at(0).end);
    EXPECT_EQ(40, ret.at(1).start);
    EXPECT_EQ(60, ret.at(1).end);

    EXPECT_EQ(2, ret.size());
}

TEST(junctionsTest, nonOverlappingJunctionCombos)
{
    Feature a;
    Feature b;
    Feature c;
    Feature d;

    a.source = "A";
    a.start = 10;
    a.end = 20;

    b.source = "B";
    b.start = 15;
    b.end = 25;

    c.source = "C";
    c.start = 30;
    c.end = 40;

    d.source = "D";
    d.start = 10;
    d.end = 65;

    vector<Feature> juncs;
    juncs.push_back(a);
    juncs.push_back(b);
    juncs.push_back(c);
    juncs.push_back(d);

    vector<vector<Feature> > combos;
    nonOverlappingJunctionCombos(juncs, combos);

    EXPECT_THAT(combos, ElementsAre(
        ElementsAre(
            Field(&Feature::source, "A")
        ),
        ElementsAre(
            Field(&Feature::source, "A"),
            Field(&Feature::source, "C")
        ),
        ElementsAre(
            Field(&Feature::source, "B")
        ),
        ElementsAre(
            Field(&Feature::source, "B"),
            Field(&Feature::source, "C")
        ),
        ElementsAre(
            Field(&Feature::source, "C")
        ),
        ElementsAre(
            Field(&Feature::source, "D")
        )
    ));
}
