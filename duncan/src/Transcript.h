#ifndef GFF_TRANSCRIPT_H
#define GFF_TRANSCRIPT_H

#include <vector>

#include "Feature.h"


namespace GFF {

    class SpliceJunc {
    };

    class Transcript : public Feature {
        public:
            std::vector<SpliceJunc> getSpliceJunctions( void );
    };
}

#endif
