#ifndef WARREN_FASTASORTER_H
#define WARREN_FASTASORTER_H

#include <string>
#include <vector>

#include "api/BamAux.h"

#include "warren/Alignment.h"
#include "warren/BamPool.h"

using std::string;
using std::vector;

using BamTools::RefVector;
using BamTools::RefData;

const RefVector DUMMY_BAMTOOLS_REF_VECTOR(vector<RefData>(1, RefData("*", 1)));

struct CompareSequences
{
    bool operator() (const Alignment& a, const Alignment& b) const
    {
        return a.QueryBases < b.QueryBases;
    }
};

struct CompareSequencePairs
{
    bool operator() (const Alignment& a, const Alignment& b) const
    {
        string a_second_seq;
        string b_second_seq;
        a.GetTag("R2", a_second_seq);
        b.GetTag("R2", b_second_seq);

        return a.QueryBases < b.QueryBases ||
               (a.QueryBases == b.QueryBases && a_second_seq < b_second_seq);
    }
};

typedef BamPool<CompareSequences> Pool;
typedef BamPoolReader<CompareSequences> PoolReader;

typedef BamPool<CompareSequencePairs> PairedPool;
typedef BamPoolReader<CompareSequencePairs> PairedPoolReader;

class FastaSorterReader
{
    PoolReader pool_reader;

    public:
        FastaSorterReader (PoolReader pr) : pool_reader(pr) {};

        bool read (Fasta& rec)
        {
            Alignment al;
            if (pool_reader.GetNextAlignment(al))
            {
                rec.header = al.Name;
                rec.sequence = al.QueryBases;
                return true;
            }
            return false;
        }
};

class FastaSorter
{
    // initialized after class definition, see below
    Pool pool;

    public:
        typedef FastaSorterReader Reader;

        FastaSorter (void) : pool(DUMMY_BAMTOOLS_REF_VECTOR) {}

        // initialized after class definition, see below
        static const int DEFAULT_BUFFER_SIZE;
        static const string DEFAULT_TMP_DIR;

        void add (Fasta& rec)
        {
            Alignment al;
            al.Name = rec.header;
            al.QueryBases = rec.sequence;
            pool.add(al);
        }

        FastaSorterReader getReader (void)
        {
            return FastaSorterReader(pool.getReader());
        }

        void setMaxBufferSize (int max)
        {
            pool.setMaxBufferSize(max);
        }

        void setTmpDir (string& tmp)
        {
            pool.setTmpDir(tmp);
        }
};

const int FastaSorter::DEFAULT_BUFFER_SIZE = Pool::DEFAULT_BUFFER_SIZE;
const string FastaSorter::DEFAULT_TMP_DIR = Pool::DEFAULT_TMP_DIR;

class FastaPairSorterReader
{
    PairedPoolReader pool_reader;

    public:
        FastaPairSorterReader (PairedPoolReader pr) : pool_reader(pr) {};

        bool read (FastaPair& rec)
        {
            Alignment al;
            if (pool_reader.GetNextAlignment(al))
            {
                rec.a.header = al.Name;
                rec.a.sequence = al.QueryBases;
                al.GetTag("XM", rec.b.header);
                al.GetTag("R2", rec.b.sequence);
                return true;
            }
            return false;
        }
};

class FastaPairSorter : public FastaSorter
{
    // initialized after class definition, see below
    PairedPool pool;

    public:
        typedef FastaPairSorterReader Reader;

        FastaPairSorter (void) : pool(DUMMY_BAMTOOLS_REF_VECTOR) {}

        // initialized after class definition, see below
        static const int DEFAULT_BUFFER_SIZE;
        static const string DEFAULT_TMP_DIR;

        void add (FastaPair& rec)
        {
            Alignment al;
            al.Name = rec.a.header;
            al.QueryBases = rec.a.sequence;

            al.AddTag("XM", "Z", rec.b.header);
            al.AddTag("R2", "Z", rec.b.sequence);

            pool.add(al);
        }

        FastaPairSorterReader getReader (void)
        {
            return FastaPairSorterReader(pool.getReader());
        }
};

const int FastaPairSorter::DEFAULT_BUFFER_SIZE = PairedPool::DEFAULT_BUFFER_SIZE;
const string FastaPairSorter::DEFAULT_TMP_DIR = PairedPool::DEFAULT_TMP_DIR;

#endif
