#ifndef WARREN_FEATURE_H
#define WARREN_FEATURE_H

#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>

#include "warren/Attributes.h"
#include "warren/tokenizer.h"

using std::string;
using std::vector;

using boost::assign::list_of;
using boost::algorithm::to_lower;

struct Feature
{
    std::set<string> transcript_types;    
    std::set<string> exon_types;    

    Feature(void)
    {
        seqid = ".";
        source = ".";
        type = ".";
        start = 0;
        end = 0;
        score = ".";
        strand = '.';
        phase = '.';
        raw_attributes = ".";

        transcript_types = list_of("mrna")
                                  ("mrna_te_gene")
                                  ("ncrna")
                                  ("mirna")
                                  ("snorna")
                                  ("snrna")
                                  ("rrna")
                                  ("trna")
                                  ("pseudogenic_transcript");

        exon_types = list_of("exon")
                            ("pseudogenic_exon");
    }

    bool initFromGFF(string& raw)
    {
        // split the tab-delimited columns
        std::vector<string> cols;
        tokenizer tokens(raw, separator("\t"));
        std::copy(tokens.begin(), tokens.end(), std::back_inserter(cols));

        if (cols.size() != 9) return false;

        seqid = cols.at(0);
        source = cols.at(1);
        type = cols.at(2);
        start = atoi(cols.at(3).c_str());
        end = atoi(cols.at(4).c_str());
        score = cols.at(5);
        strand = *(cols.at(6).c_str());
        phase = *(cols.at(7).c_str());

        raw_attributes = cols.at(8);
        attributes = Attributes();
        attributes.addFromGFF(raw_attributes);

        return true;
    }

    string seqid;
    string source;
    string type;
    unsigned int start;
    unsigned int end;
    string score;
    char strand;
    char phase;
    string raw_attributes;
    Attributes attributes;

    vector<Feature> children;

    bool hasStrand(void) const
    {
        return strand == '+' || strand == '-';
    }

    bool isRevStrand(void) const
    {
        return strand == '-';
    }

    int getLength(void) const
    {
        return end - start + 1;
    }

    bool isTranscriptType()
    {
        string s(type);
        to_lower(s);
        return transcript_types.find(s) != transcript_types.end();
    }

    bool isExonType()
    {
        string s(type);
        to_lower(s);
        return exon_types.find(s) != exon_types.end();
    }
};

struct FeaturePositionComparator
{
    bool operator() (const Feature& a, const Feature& b) const
    {
        return (a.seqid < b.seqid)
            || (a.seqid == b.seqid && a.start < b.start)
            || (a.seqid == b.seqid && a.start == b.start && a.end < b.end);
    }
};

#endif
