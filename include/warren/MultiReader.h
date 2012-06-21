#ifndef WARREN_MULTIREADER_H
#define WARREN_MULTIREADER_H

#include <iostream>
#include <vector>

using std::istream;
using std::vector;

struct Record {};

template<class Reader, class Comparator>
class MultiReader
{
    typedef typename Reader::record Record;

    Comparator comparator;

    vector<Reader*> readers;

    bool compareReaderHeads (const Reader* a, const Reader* b)
    {
        Record c, d;
        a->peek(c);
        b->peek(d);
        return comparator(d, c);
    }

    bool pushReader (Reader* reader)
    {
        if (!reader->good()) return false;

        readers.push_back(reader);
        push_heap(readers.begin(), readers.end(), compareReaderHeads);

        return true;
    }

    bool popReader (Reader* reader)
    {
        if (readers.empty()) return false;

        // front record is the one on the top of the heap
        reader = readers.front();
        pop_heap(readers.begin(), readers.end(), compareReaderHeads);
        readers.pop_back();

        return true;
    }

    public:
        ~MultiReader (void)
        {
            typedef typename vector<Reader*>::iterator ReaderIterator;
            for (ReaderIterator reader = readers.begin();
                 reader != readers.end(); ++reader)
            {
                delete *reader;
            }
        }

        void addInputStream (istream& input)
        {
            pushReader(new Reader(input));
        }

        bool read (Record& record)
        {
            Reader* max;
            if (popReader(max))
            {
                max->read(record);
                if (!pushReader(max)) delete max;

                return true;
            }
            return false;
        }
};

#endif
