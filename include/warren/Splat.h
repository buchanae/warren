#ifndef WARREN_SPLAT_H
#define WARREN_SPLAT_H

#include <string>
#include <sstream>
#include <vector>

#include <boost/algorithm/string/join.hpp>

#include "warren/Alignment.h"

using std::string;
using std::stringstream;
using std::vector;

bool isSplat (Alignment& al)
{
    return al.HasTag("XD");
}

struct SplatPosition
{
    string ref;
    int a_start;
    int a_end;
    int b_start;
    int b_end;

    bool operator== ( const SplatPosition& other ) const {
        return ref == other.ref
            && a_start == other.a_start
            && a_end == other.a_end 
            && b_start == other.b_start
            && b_end == other.b_end;
    }
};

struct Splat
{
    string flanks;
    SplatPosition position;
    string seq;
    vector<string> read_IDs;

    Splat (Alignment& al)
    {
        position.ref = al.RefName;
        al.GetTag("XD", flanks);

        position.a_start = al.position();
        position.a_end = position.a_start + al.CigarData.at(0).Length - 1;
        position.b_start = position.a_end + al.CigarData.at(1).Length + 1;
        position.b_end = position.b_start + al.CigarData.at(2).Length - 1;

        seq = al.QueryBases;

        string strand = al.IsReverseStrand() ? "-" : "+";

        stringstream read_ID (stringstream::in | stringstream::out);
        read_ID << strand;
        read_ID << al.Name;
        read_IDs.push_back(read_ID.str());
    }

    void toString (string& output)
    {
        stringstream buffer;

        buffer << position.ref << "\t";
        buffer << flanks << "\t";
        buffer << position.a_end - position.a_start + 1 << "\t";
        buffer << position.b_end - position.b_start + 1 << "\t";
        buffer << position.b_start - position.a_end - 1 << "\t";
        buffer << position.a_start << "\t" << position.a_end << "\t";
        buffer << position.b_start << "\t" << position.b_end << "\t";
        buffer << seq << "\t";
        buffer << read_IDs.size() << "\t";
        buffer << boost::algorithm::join(read_IDs, ",");

        output = buffer.str();
    }

    void merge (Splat& other)
    {
        read_IDs.insert(read_IDs.end(), other.read_IDs.begin(), other.read_IDs.end());
    }
};

#endif
