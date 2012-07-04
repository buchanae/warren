#ifndef ISOSIGNAL_BAMREADER_H
#define ISOSIGNAL_BAMREADER_H

#include "bamtools/api/BamReader.h"

#include "warren/Alignment.h"

class BamReader : public BamTools::BamReader
{
    public:
        bool GetNextAlignment(Alignment& a)
        {
            if (!BamTools::BamReader::GetNextAlignment(a)) return false;

            if (a.RefID != -1)
            {
                a.RefName = GetReferenceData().at(a.RefID).RefName;
            }
            return true;
        }
};

#endif
