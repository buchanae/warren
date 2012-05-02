#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Attributes.h"

using namespace GFF;

using testing::ElementsAre;
using std::string;
using std::vector;

TEST (AttributesTest, has)
{
    Attributes a;
    a.addFromGFF("foo=bar;baz=bat");
    EXPECT_TRUE(a.has("foo"));
    EXPECT_TRUE(a.has("baz"));
    EXPECT_FALSE(a.has("blah"));
}

TEST (AttributesTest, raw)
{
    string s;
    Attributes a;
    a.addFromGFF("foo=hello%20world%2Fplanet");
    a.raw("foo", s);
    EXPECT_EQ("hello%20world%2Fplanet", s);
}

TEST (AttributesTest, get)
{
    string s;
    Attributes a;
    a.addFromGFF("foo=bar");
    a.get("foo", s);
    EXPECT_EQ("bar", s);
}

TEST (AttributesTest, get_first_of_many)
{
    string s;
    Attributes a;
    a.addFromGFF("foo=bar,baz");
    a.get("foo", s);
    EXPECT_EQ("bar", s);
}

TEST (AttributesTest, get_empty_value)
{
    string s;
    Attributes a;
    a.addFromGFF("foo=");
    a.get("foo", s);
    EXPECT_EQ("", s);
}

TEST (AttributesTest, get_decoded_value)
{
    string s;
    Attributes a;
    a.addFromGFF("foo=hello%20world%2Cplanet");
    a.get("foo", s);
    EXPECT_EQ("hello world,planet", s);
}

TEST (AttributesTest, invalid_attrib_is_ignored)
{
    string s = "default";
    Attributes a;
    a.addFromGFF("foo");
    EXPECT_FALSE(a.get("foo", s));
    EXPECT_EQ("default", s);
}

TEST (AttributesTest, all)
{
    vector<string> v;
    Attributes a;
    a.addFromGFF("foo=bar%2Cbaz,bat");
    EXPECT_TRUE(a.all("foo", v));
    EXPECT_FALSE(a.all("non-existent", v));
    EXPECT_THAT(v, ElementsAre("bar,baz", "bat"));
}

TEST (AttributesTest, funky_attribute_def)
{
    vector<string> v;
    Attributes a;
    a.addFromGFF("foo=1;foo=2");
    EXPECT_TRUE(a.all("foo", v));
    EXPECT_THAT(v, ElementsAre("1", "2"));
}
