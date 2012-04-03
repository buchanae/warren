#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Attribute.h"


using ::testing::ElementsAre;

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

    EXPECT_EQ("foo=bar", a.toString());

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
    a.value = "foo";
    EXPECT_THAT(a.getAll(), ElementsAre("foo"));
}

TEST( AttributeTest, getAll_many ){

    GFF::Attribute a;
    a.value = "foo,bar";
    EXPECT_THAT(a.getAll(), ElementsAre("foo", "bar"));
}

TEST( AttributeTest, decoded_chars_are_unescaped ){
}
