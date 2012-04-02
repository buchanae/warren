#ifndef _GFF_READER_H
#define _GFF_READER_H

#include <ifstream>
#include <string>

using std::string;

namespace GFF {
    class Reader {
        public:
            bool Open( string );
            bool getNextFeature( Feature& );

        private:
            ifstream fh;

            bool isGFFComment( string );
    };
}

#endif
