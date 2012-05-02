#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "Feature.h"
#include "Reader.h"

// TODO skip GFF comments

TEST( ReaderTest, getNextFeature ){

    std::stringstream data;
    data << "Chr\ttest\ttestgene\t20\t30\t0\t+\t0\tName=foo" << std::endl;
    data << "Chr2\ttest\ttestgene\t20\t30\t0\t+\t0\tName=foo" << std::endl;

    GFF::Feature f;

    GFF::Reader::getNextFeature( data, f );
    EXPECT_EQ( "Chr", f.seqid );

    GFF::Reader::getNextFeature( data, f );
    EXPECT_EQ( "Chr2", f.seqid );
}
