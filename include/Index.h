#ifndef _GFF_FEATUREINDEX_H
#define _GFF_FEATUREINDEX_H

#include <map>
#include <string>
#include <vector>

#include "Feature.h"

using std::string;
using std::vector;

namespace GFF
{
    class IndexBase
    {
        typedef vector<Feature>::iterator Feature_iter;

        public:
            void add(Feature&);
            void add(Feature_iter, Feature_iter);

        private:
            virtual void index(Feature&) = 0;
    };

    class TypeIndex : public IndexBase
    {
        typedef std::map<string, Feature>::iterator map_iter;
        std::multimap<string, Feature> by_type;

        public:
            void type(const string&, vector<Feature>&);

        private:
            virtual void index(Feature&);
    };

    class ParentChildIndex : public IndexBase
    {
        typedef std::map<string, Feature>::iterator map_iter;
        std::multimap<string, Feature> by_parent_ID;

        public:
            void childrenOf(Feature&, vector<Feature>&);

        private:
            virtual void index(Feature&);
    };
}
#endif
