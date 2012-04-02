#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "Attribute.h"


TEST( AttributeTest, fromString ){

    GFF::Attribute a;
    bool ret;

    ret = a.fromString("foo=bar");
    EXPECT_TRUE(ret);

    a.fromString("foo=hello%20world%2Fplanet");
    EXPECT_TRUE(ret);
}

TEST( AttributeTest, fromString_invalid ){

    GFF::Attribute a;
    bool ret;

    ret = a.fromString("foo");
    EXPECT_FALSE(ret);

    ret = a.fromString("foo=");
    EXPECT_FALSE(ret);
}

TEST( AttributeTest, toString ){

    GFF::Attribute a;
    a.name = "foo";
    a.value = "bar";

    EXPECT_EQ("foo=bar");

    // TODO test encoding characters
}

TEST( AttributeTest, hasMultiple ){

    GFF::Attribute a;
    a.value = "foo";
    EXPECT_FALSE(a.hasMultiple());

    a.value = "foo,bar";
    EXPECT_TRUE(a.hasMultiple());
}

TEST( AttributeTest, getAll_one ){

    GFF::Attribute a;
    std::vector<std::string> expect;

    a.value = "foo";
    expect.push_back("foo");

    EXPECT_EQ(expect, a.getAll());
}

TEST( AttributeTest, getAll_many ){

    GFF::Attribute a;
    std::vector<std::string> expect;

    a.value = "foo,bar";
    expect.push_back("foo");
    expect.push_back("bar");

    EXPECT_EQ(expect, a.getAll());
    expect.clear();
}

TEST( AttributeTest, decoded_chars_are_unescaped ){
}
