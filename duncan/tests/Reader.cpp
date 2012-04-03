#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "Feature.h"
#include "Reader.h"


TEST( ReaderTest, getNextFeature ){

    std::stringstream data;
    data << "Chr\ttest\ttestgene\t20\t30\t0\t+\t0\tName=foo" << std::endl;

    GFF::Reader r;

    GFF::Feature f;

    r.getNextFeature( data, f );

    EXPECT_EQ( "Chr", f.seqid );
}
