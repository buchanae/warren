#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "warren/Feature.h"
#include "warren/GFFReader.h"

using std::vector;
using std::string;

using testing::ElementsAre;
using testing::WhenSorted;
// TODO skip GFF comments

TEST (GFFReaderTest, read)
{
    std::stringstream data;
    data << "Chr\ttest\ttestgene\t20\t30\t0\t+\t0\tName=foo" << std::endl;
    data << "Chr2\ttest\ttestgene\t20\t30\t0\t+\t0\tName=foo" << std::endl;

    Feature f;

    GFFReader reader(data);
    reader.read(f);
    EXPECT_EQ("Chr", f.seqid);

    reader.read(f);
    EXPECT_EQ("Chr2", f.seqid);
}
