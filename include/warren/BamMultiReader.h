#ifndef WARREN_BAMMULTIREADER_H
#define WARREN_BAMMULTIREADER_H

#include "bamtools/api/BamMultiReader.h"

#include "Alignment.h"

class BamMultiReader : public BamTools::BamMultiReader
{
    public:
        bool GetNextAlignment(Alignment& a)
        {
            if (!BamTools::BamMultiReader::GetNextAlignment(a)) return false;

            a.RefName = GetReferenceData().at(a.RefID).RefName;
            return true;
        }
};

#endif
