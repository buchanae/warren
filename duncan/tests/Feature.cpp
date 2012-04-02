#include "gtest/gtest.h"

#include "Feature.h"


TEST( FeatureTest, isRevStrand ){

    GFF::Feature f;
    f.strand = '+';
    EXPECT_FALSE(f.isRevStrand());

    f.strand = '.';
    EXPECT_FALSE(f.isRevStrand());

    f.strand = '-';
    EXPECT_TRUE(f.isRevStrand());
}

TEST( FeatureTest, hasStrand ){

    GFF::Feature f;
    f.strand = '+';
    EXPECT_TRUE(f.isRevStrand());

    f.strand = '-';
    EXPECT_TRUE(f.isRevStrand());

    f.strand = '.';
    EXPECT_FALSE(f.isRevStrand());

    f.strand = '?';
    EXPECT_FALSE(f.isRevStrand());
}

TEST( FeatureTest, getLength ){

    GFF::Feature f;
    f.start = 20;
    f.end = 30;
    EXPECT_EQ(11, f.getLength());
}

TEST( FeatureTest, toString ){

    GFF::Feature f;
    f.seqid = "Chr";
    f.source = "test";
    f.type = "testgene";
    f.start = 20;
    f.end = 30;
    f.score = 0;
    f.strand = '+';
    f.phase = 0;

    // TODO replace these with a string join
    EXPECT_EQ("Chr\ttest\ttestgene\t20\t30\t0\t+\t0\t", f.toString());

    // TODO these are failing because strand and phase are defined as numbers, not chars
    f.score = '.';
    EXPECT_EQ("Chr\ttest\ttestgene\t20\t30\t.\t+\t0\t", f.toString());

    f.phase = '.';
    EXPECT_EQ("Chr\ttest\ttestgene\t20\t30\t.\t+\t.\t", f.toString());

    f.strand = '.';
    EXPECT_EQ("Chr\ttest\ttestgene\t20\t30\t.\t.\t.\t", f.toString());
}

TEST( FeatureTest, fromString ){

    GFF::Feature f;
    bool ret = f.fromString("Chr\ttest\ttestgene\t20\t30\t0\t+\t0\tName=foo");
    EXPECT_TRUE(ret);
    EXPECT_EQ("Chr", f.seqid);
    EXPECT_EQ("test", f.source);
    EXPECT_EQ("testgene", f.type);
    EXPECT_EQ(20, f.start);
    EXPECT_EQ(30, f.end);
    EXPECT_EQ(0, f.score);
    EXPECT_EQ('+', f.strand);
    EXPECT_EQ(0, f.phase);
    EXPECT_EQ("foo", f.attributes.at(0).value);
}

TEST( FeatureTest, fromString_invalid ){

    GFF::Feature f;
    // invalid number of columns
    // TODO should use expections instead?
    bool ret = f.fromString("Chr\ttest\ttestgene\t20\t30\t0\t+\t");
    EXPECT_FALSE(ret);
}
