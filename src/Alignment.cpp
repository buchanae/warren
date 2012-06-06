#include <vector>

#include "bamtools/api/BamAux.h"

#include "warren/Alignment.h"
#include "warren/Feature.h"

Alignment::Alignment(void) : BamTools::BamAlignment() {}
Alignment::Alignment(BamTools::BamAlignment& other) : BamTools::BamAlignment(other) {}

bool Alignment::getJunction (Feature& junction)
{
    // this all assumes there is only one gap, i.e. only one 'N' CigarOp

    int len = 0;
    int gap_len = 0;

    for (std::vector<CigarOp>::iterator op = CigarData.begin(); 
         op != CigarData.end(); ++op)
    {
        if (op->Type == 'N')
        {
            gap_len = op->Length;
            break;
        }

        len += op->Length;
    }

    if (gap_len == 0) return false;

    junction.seqid = RefName;
    junction.start = position() + len - 1;
    junction.end = junction.start + gap_len + 1;

    return true;
}

string toString (const vector<CigarOp>& cd)
{
    stringstream ss;
    string out = "";
    vector<CigarOp>::const_iterator iter = cd.begin();
    for (; iter != cd.end() ; ++iter)
    {
        ss << iter->Length << iter->Type;
    }
    ss >> out;
    return out;
}

int sumCigar (vector<CigarOp>& cigar)
{
    int length = 0;
    for (unsigned int i = 0; i < cigar.size(); i++)
    {
        length += cigar.at(i).Length;
    }
    return length;
}

// Calculate paired-end gapped alignments using the CigarOp data
// NOT the length of the query sequence
void getSpacer (Alignment& a, Alignment& b, Feature& spacer)
{
    spacer.seqid = a.RefName;
    spacer.type = "spacer";
    spacer.start = a.position() + sumCigar(a.CigarData) - 1;
    spacer.end = b.position();
}
