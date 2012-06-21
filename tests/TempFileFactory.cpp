#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "warren/TempFileFactory.h"

using std::string;

TEST(TempFileFactoryTest, make)
{
    TempFileFactory factory;
    string path;

    EXPECT_TRUE(factory.make(path));
    EXPECT_EQ("/tmp/Warren-", path.substr(0, 12));
}

TEST(TempFileFactoryTest, make_bad_location)
{
    TempFileFactory factory;
    factory.TMP_DIR = "/something/that/does/not/exist";
    string path;

    EXPECT_FALSE(factory.make(path));
}
