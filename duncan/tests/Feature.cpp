#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Feature.h"

using testing::ElementsAre;
using namespace GFF;

// TODO features can have URL escaped characters

TEST(FeatureTest, isRevStrand)
{

    Feature f;
    f.strand = '+';
    EXPECT_FALSE(f.isRevStrand());

    f.strand = '.';
    EXPECT_FALSE(f.isRevStrand());

    f.strand = '-';
    EXPECT_TRUE(f.isRevStrand());
}

TEST(FeatureTest, hasStrand)
{

    Feature f;
    f.strand = '+';
    EXPECT_FALSE(f.isRevStrand());

    f.strand = '-';
    EXPECT_TRUE(f.isRevStrand());

    f.strand = '.';
    EXPECT_FALSE(f.isRevStrand());

    f.strand = '?';
    EXPECT_FALSE(f.isRevStrand());
}

TEST(FeatureTest, getLength)
{

    Feature f;
    f.start = 20;
    f.end = 30;
    EXPECT_EQ(11, f.getLength());
}

TEST(FeatureTest, from_GFF)
{

    Feature f("Chr\ttest\ttestgene\t20\t30\t0\t+\t0\tName=foo");
    EXPECT_EQ("Chr", f.seqid);
    EXPECT_EQ("test", f.source);
    EXPECT_EQ("testgene", f.type);
    EXPECT_EQ(20, f.start);
    EXPECT_EQ(30, f.end);
    EXPECT_EQ("0", f.score);
    EXPECT_EQ('+', f.strand);
    EXPECT_EQ('0', f.phase);
    EXPECT_EQ("Name=foo", f.raw_attributes);
    string s;
    f.attributes.get("Name", s);
    EXPECT_EQ("foo", s);
}

TEST(FeatureTest, from_GFF_invalid_number_of_columns)
{
    // TODO more specific exception class
    EXPECT_ANY_THROW(Feature f("Chr\ttest\ttestgene\t20\t30\t0\t+\t"));
}

/*
TEST( FeatureTest, toString ){

    Feature f;
    f.seqid = "Chr";
    f.source = "test";
    f.type = "testgene";
    f.start = 20;
    f.end = 30;
    f.score = "0";
    f.strand = '+';
    f.phase = '0';

    EXPECT_EQ("Chr\ttest\ttestgene\t20\t30\t0\t+\t0\t", f.toString());

    f.score = ".";
    EXPECT_EQ("Chr\ttest\ttestgene\t20\t30\t.\t+\t0\t", f.toString());

    f.phase = '.';
    EXPECT_EQ("Chr\ttest\ttestgene\t20\t30\t.\t+\t.\t", f.toString());

    f.strand = '.';
    EXPECT_EQ("Chr\ttest\ttestgene\t20\t30\t.\t.\t.\t", f.toString());
}
*/
