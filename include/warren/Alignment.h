#ifndef WARREN_ALIGNMENT_H
#define WARREN_ALIGNMENT_H

#include <sstream>
#include <string>

#include "bamtools/api/BamAlignment.h"

#include "warren/Feature.h"

using std::string;
using std::stringstream;
using std::vector;

using BamTools::BamAlignment;
using BamTools::CigarOp;

class Alignment : public BamAlignment
{
    public:
        std::string RefName;

        Alignment(void);
        Alignment(BamAlignment& other);

        int position (void) const
        {
            return Position + 1;
        }

        void position (int p)
        {
            Position = p - 1;
        }

        bool getJunction (Feature& junction);

    private:
        // Prevent public access to Position, because it's zero-based,
        // which can cause confusion and difficult debugging.
        // Use position() instead.
        BamAlignment::Position;
};

string toString(const vector<CigarOp>& cd);

int sumCigar(vector<CigarOp>& cigar);

int pairedGapLength(Alignment& a, Alignment& b);

#endif
