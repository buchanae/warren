#ifndef _GFF_FEATURE_H
#define _GFF_FEATURE_H

#include <string>;
#include <map>;
#include <vector>;

#include "Attributes.h";

using std::string;

namespace GFF
{
    class Feature
    {
        public:
            Feature ( void );
            Feature (const char *);
            Feature ( string& );

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

            bool hasStrand( void );
            bool isRevStrand( void );
            bool hasScore( void );
            double getScore( void );
            int getLength( void );
            // TODO string toString( void );

            bool initFromGFF(string&);
    };
}
#endif
