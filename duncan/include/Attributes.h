#ifndef _GFF_ATTRIBUTES_H
#define _GFF_ATTRIBUTES_H

#include <map>
#include <set>
#include <string>
#include <vector>

using std::string;

namespace GFF
{
    class Attributes
    {
        public:
            bool has(const string) const;
            bool raw(const string, string&) const;
            bool get(const string, string&) const;
            bool all(const string, std::vector<string>&); // TODO const?
            void keys(std::set<string>&) const;

            bool addFromGFF (const char *);
            bool addFromGFF (string&);

        private:
            std::multimap<string, string> attributes;
            std::map<string, string> raw_values;
            std::set<string> key_set;
    };
}
#endif
