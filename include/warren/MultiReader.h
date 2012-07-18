#ifndef WARREN_MULTIREADER_H
#define WARREN_MULTIREADER_H

#include <istream>
#include <queue>

#include "warren/Fasta.h"

using std::istream;
using std::queue;

template<typename Reader>
class MultiReader
{
    queue<Reader*> readers;

    public:

        typedef typename Reader::Record Record;

        ~MultiReader (void)
        {
            while (!readers.empty())
            {
                readers.pop();
            }
        }

        void addInput(istream& in)
        {
            Reader* r = new Reader(in);
            readers.push(r);
        }

        bool read (Record& rec)
        {
            Record tmp;
            Reader* current = readers.front();

            while (!readers.empty())
            {
                if (current->read(tmp))
                {
                    rec = tmp;
                    return true;
                }
                readers.pop();
                current = readers.front();
            }
            return false;
        }
};

#endif
