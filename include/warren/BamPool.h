#ifndef WARREN_BAMPOOL_H
#define WARREN_BAMPOOL_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "bamtools/api/BamAux.h"
#include "bamtools/api/BamWriter.h"

#include "warren/Alignment.h"
#include "warren/BamReader.h"

#define DEFAULT_MAX_SIZE 1000000
#define DEFAULT_TMP_DIR "/tmp"

using std::string;
using std::vector;

using BamTools::BamWriter;
using BamTools::RefVector;

template<class Compare>
class BamPoolReader
{
    struct ReadHead
    {
        BamReader* reader;
        Alignment alignment;

        bool operator< (const ReadHead& other) const
        {
            return comparator(alignment, other.alignment);
        }

        bool readNext (void)
        {
            return reader->GetNextAlignment(alignment);
        }
    };

    bool initialized;
    Compare comparator;
    vector<string> file_names;
    vector<ReadHead*> read_heads;

    void init (void)
    {
        // open files and initilize read heads
        for (vector<string>::iterator file_name = file_names.begin();
             file_name != file_names.end(); ++file_name)
         {
              BamReader* reader = new BamReader;
              if (!reader->Open(*file_name))
              {
                  // TODO how to error here?
              }
              else
              {
                  ReadHead* head = new ReadHead;
                  head->reader = reader;
                  if (head->readNext())
                  {
                      read_heads.push_back(head);
                  }
              }
         }

         // initialize heap of read heads
         make_heap(read_heads.begin(), read_heads.end());

         initialized = true;
    }

    public:
        BamPoolReader (vector<string> fns) : file_names(fns), initialized(false) {};

        bool GetNextAlignment(Alignment& alignment)
        {
            if (!initialized) init();

            if (read_heads.empty()) return false;

            // next alignment is the one on the top of the heap
            ReadHead* max = read_heads.front();
            pop_heap(read_heads.begin(), read_heads.end());
            read_heads.pop_back();
            alignment = max->alignment;

            // read the next alignment from the read head we just popped off the heap
            if (max->readNext())
            {
                read_heads.push_back(max);
                push_heap(read_heads.begin(), read_heads.end());
            }
            else
            {
                max->reader->Close();
            }

            return true;
        }
};

template<class Compare>
class BamPool
{
    string TMP_DIR;
    int MAX_SIZE;

    typedef vector<Alignment> buffer_t;
    buffer_t buffer;

    vector<string> file_names;
    BamTools::RefVector references;

    Compare comparator;

    public:

        BamPool (BamTools::RefVector& refs, 
                 int max = DEFAULT_MAX_SIZE, 
                 string tmp = DEFAULT_TMP_DIR)
            : references(refs), MAX_SIZE(max), TMP_DIR(tmp) {};

        ~BamPool (void)
        {
            // remove temporary files
            for (vector<string>::iterator it = file_names.begin();
                 it != file_names.end(); ++it)
            {
                remove(it->c_str());
            }
        }

        void add (Alignment& alignment)
        {
            // flush buffer if it's full
            if (buffer.size() >= MAX_SIZE) flush();

            buffer.push_back(alignment);
        }

        void flush (void)
        {
            if (buffer.empty()) return;

            std::sort(buffer.begin(), buffer.end(), comparator);

            // open temp. file
            string path = TMP_DIR + "/BamPool-XXXXXX";
            vector<char> dst_path(path.begin(), path.end());
            dst_path.push_back('\0');

            int fd = mkstemp(&dst_path[0]);
            if (fd != -1)
            {
                path.assign(dst_path.begin(), dst_path.end() - 1);
                close(fd);
            }
            // TODO throw error if file could not be created

            // open a BamWriter
            BamWriter writer;
            // FIXME string header = "@HD\tVN:1.0\tSO:" + comparator.name;
            string header = "@HD\tVN:1.0\tSO:unknown";

            if (!writer.Open(path, header, references))
            {
                std::cerr << writer.GetErrorString() << std::endl;
                // TODO should really throw error here
                // TODO bad to assume we can use std::cerr
            }
            file_names.push_back(path);

            // dump buffer to file
            for (buffer_t::iterator it = buffer.begin(); it != buffer.end(); ++it)
            {
                writer.SaveAlignment(*it);
            }

            // TODO does closing the writer allow it to be removed?
            //      need to save reference to file handle?
            writer.Close();
            buffer.clear();
        }

        void setMaxBufferSize(int max)
        {
            MAX_SIZE = max;
        }

        void setTmpDir(string& tmp)
        {
            TMP_DIR = tmp;
        }

        BamPoolReader<Compare> getReader (void)
        {
            flush();
            return BamPoolReader<Compare>(file_names);
        }
};

#endif
