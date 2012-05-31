#ifndef _ISOSIGNAL_COVERAGE_H
#define _ISOSIGNAL_COVERAGE_H

#include <map>
#include <string>
#include <vector>

#include "api/BamAux.h"
#include "Feature.h"

#include "Alignment.h"

using GFF::Feature;
using std::vector;
using std::string;

class Coverage
{
    public:
        typedef std::map<std::string, vector<int> > coverages_t;
        coverages_t coverages;

        // be careful with get/set/increment, they will throw out of range errors
        // if you haven't initialized the references
        int get(string ref_name, int pos);
        void set(string ref_name, int pos, int value);
        void increment(string ref_name, int pos, int value = 1);

        void setMinReferenceLength(string name, int length);

        void add(Alignment& alignment);
        void add(string ref_name, int start, int length);

        void load(std::istream& input);

        void toOutputStream(std::ostream& output);
        void toString(std::string& output);
};

#endif
