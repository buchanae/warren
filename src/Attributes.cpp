#include <algorithm>
#include <cstring>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "uriparser/Uri.h"

#include "Attributes.h"
#include "tokenizer.h"

using std::string;
using std::vector;

namespace GFF
{
    typedef std::multimap<string, string>::iterator attribs_iter_t;
    typedef std::pair<attribs_iter_t, attribs_iter_t> attribs_range;

    bool Attributes::addFromGFF(const char* raw)
    {
        string s(raw);
        return addFromGFF(s);
    }

    bool Attributes::addFromGFF(string& raw)
    {
        // split attributes
        // e.g. foo=bar;baz=bat; => foo=bar, baz=bat
        tokenizer tokens(raw, separator(";"));

        for (token_iter it = tokens.begin(); it != tokens.end(); ++it)
        {
            // split attribute key-value at "=" char
            // e.g. foo=bar => foo, bar
            // TODO better way to do this than creating a vector?
            vector<string> key_value;
            tokenizer kv_tokens(*it, separator("="));
            std::copy(kv_tokens.begin(), kv_tokens.end(), std::back_inserter(key_value));

            // expect exactly one key and one value
            if (key_value.size() == 2)
            {
                raw_values.insert(make_pair(key_value.at(0), key_value.at(1)));
                key_set.insert(key_value.at(0));

                // split attribute value at "," char
                // e.g. val1,val2 => val1, val2
                tokenizer vals(key_value.at(1), separator(","));

                for (token_iter val_it = vals.begin(); val_it != vals.end(); ++val_it)
                {
                    // URL decode key and value
                    char* key;
                    key = new char [key_value.at(0).size() + 1];
                    strcpy(key, key_value.at(0).c_str());
                    uriUnescapeInPlaceA(key);

                    char* value;
                    value = new char [(*val_it).size() + 1];
                    strcpy(value, (*val_it).c_str());
                    uriUnescapeInPlaceA(value);

                    attributes.insert(make_pair(string(key), string(value)));

                    delete[] key;
                    delete[] value;
                }
            }
        }

        return true;
    }

    bool Attributes::has(const string key) const
    {
        return key_set.find(key) != key_set.end();
    }

    bool Attributes::raw(const string key, string& value) const
    {
        if (!has(key)) return false;

        value = raw_values.find(key)->second;
        return true;
    }

    bool Attributes::get(const string key, string& value) const
    {
        if (!has(key)) return false;

        value = attributes.find(key)->second;
        return true;
    }

    bool Attributes::all(const string key, vector<string>& values)
    {
        if (!has(key)) return false;

        attribs_range range = attributes.equal_range(key);

        for (attribs_iter_t it = range.first; it != range.second; ++it)
        {
            values.push_back(it->second);
        }

        return true;
    }

    void Attributes::keys(std::set<string>& keys) const
    {
        return keys.insert(key_set.begin(), key_set.end());
    }
}
