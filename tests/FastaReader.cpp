#include <iostream>
#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "warren/Fasta.h"
#include "warren/FastaReader.h"

using std::string;
using std::endl;

TEST(FastaReaderTest, read)
{
    std::stringstream data;
    data << ">header one" << endl;
    data << "ATCG"        << endl;
    data << "ATCG"        << endl;
    data << ">header two" << endl;
    data << "GGTC"        << endl;

    Fasta f;
    FastaReader reader(data);

    EXPECT_TRUE(reader.read(f));
    EXPECT_EQ("header one", f.header);
    EXPECT_EQ("ATCGATCG", f.sequence);

    EXPECT_TRUE(reader.read(f));
    EXPECT_EQ("header two", f.header);
    EXPECT_EQ("GGTC", f.sequence);

    EXPECT_FALSE(reader.read(f));
}

TEST(FastaReaderTest, read_empty_sequence)
{
    std::stringstream data;
    data << ">header one" << endl;
    data << "ATCG"        << endl;
    data << ">header two" << endl;

    Fasta f;
    FastaReader reader(data);

    EXPECT_TRUE(reader.read(f));
    EXPECT_EQ("header one", f.header);
    EXPECT_EQ("ATCG", f.sequence);

    EXPECT_TRUE(reader.read(f));
    EXPECT_EQ("header two", f.header);
    EXPECT_EQ("", f.sequence);
}

TEST(FastaReaderTest, read_skips_bad_first_record)
{
    std::stringstream data;
    data << "junk"        << endl;
    data << ">header one" << endl;
    data << "ATCG"        << endl;

    Fasta f;
    FastaReader reader(data);

    EXPECT_TRUE(reader.read(f));
    EXPECT_EQ("header one", f.header);
    EXPECT_EQ("ATCG", f.sequence);

    EXPECT_FALSE(reader.read(f));
}

TEST(FastaReaderTest, empty_input)
{
    std::stringstream data;

    Fasta f;
    FastaReader reader(data);

    EXPECT_FALSE(reader.read(f));
}
