#include <iostream>
#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "warren/Feature.h"
#include "warren/StackReader.h"

using std::string;

TEST (StackReaderTest, read)
{
    std::stringstream data;
    data << "@Chr\tAT-CG\t10\t20\t11" << std::endl;
    data << "ATCG\t1" << std::endl;
    data << "ATCG\t1" << std::endl;
    data << "@Chr2\tAT-CG\t30\t40\t11" << std::endl;

    StackReader reader(data);
    Feature f;

    EXPECT_TRUE(reader.read(f));
    EXPECT_EQ("Chr", f.seqid);
    EXPECT_EQ("splice_junction", f.type);
    EXPECT_EQ(10, f.start);
    EXPECT_EQ(20, f.end);

    EXPECT_TRUE(reader.read(f));
    EXPECT_EQ("Chr2", f.seqid);
    EXPECT_EQ("splice_junction", f.type);
    EXPECT_EQ(30, f.start);
    EXPECT_EQ(40, f.end);

    EXPECT_FALSE(reader.read(f));
}
