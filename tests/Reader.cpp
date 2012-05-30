#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Feature.h"
#include "Reader.h"

using namespace GFF;
using std::vector;
using std::string;
using testing::ElementsAre;
using testing::WhenSorted;
// TODO skip GFF comments

TEST(ReaderTest, getNextFeature)
{
    std::stringstream data;
    data << "Chr\ttest\ttestgene\t20\t30\t0\t+\t0\tName=foo" << std::endl;
    data << "Chr2\ttest\ttestgene\t20\t30\t0\t+\t0\tName=foo" << std::endl;

    Feature f;

    Reader::getNextFeature(data, f);
    EXPECT_EQ("Chr", f.seqid);

    Reader::getNextFeature(data, f);
    EXPECT_EQ("Chr2", f.seqid);
}

TEST(ReaderTest, readAllAndLinkChildren)
{
    typedef vector<Feature>::iterator Feature_iter;

    std::stringstream data;
    data << "Chr\t.\tgene\t20\t30\t0\t+\t0\tID=one" << std::endl;
    data << "Chr\t.\tgene\t20\t30\t0\t+\t0\tID=two;Parent=one;" << std::endl;
    data << "Chr\t.\tgene\t20\t30\t0\t+\t0\tID=three;Parent=one;" << std::endl;

    vector<Feature> ret;
    Reader::readAllAndLinkChildren(data, ret);

    EXPECT_EQ(3, ret.size());

    Feature d = ret.at(0);
    vector<string> IDs;

    for (Feature_iter it = d.children.begin(); it != d.children.end(); ++it)
    {
        string ID = "";
        it->attributes.get("ID", ID);
        IDs.push_back(ID);
    }
    EXPECT_THAT(IDs, WhenSorted(ElementsAre("three", "two")));

    EXPECT_EQ(0, ret.at(1).children.size());
    EXPECT_EQ(0, ret.at(2).children.size());
}

// TODO test children() when feature doesn't have ID
