#include <iostream>
#include <fstream>
#include <tr1/functional>
#include <string>
#include <vector>

#include <boost/algorithm/string/join.hpp>
#include <tclap/CmdLine.h>

#include "warren/cli_helpers.h"
#include "warren/Fasta.h"
#include "warren/FastaReader.h"
#include "warren/MultiReader.h"

#define VERSION "0.1"

using boost::algorithm::join;

using TCLAP::CmdLine;
using TCLAP::MultiArg;
using TCLAP::ValueArg;
using TCLAP::ArgException;

using std::cerr;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::vector;

template<typename Record>
class Grouper
{
    int group_ID;
    vector<Record> group;

    virtual bool compare (const Record&, const Record&) = 0;

    virtual void handleGroup (int, const vector<Record>&) = 0;

    public:
        Grouper (void) : group_ID(1) {}

        void add (Record& record)
        {
            if (group.empty())
            {
                group.push_back(record);
            }
            else
            {
                Record current = group.front();
                if (compare(current, record))
                {
                    group.push_back(record);
                }
                else
                {
                    flush();
                    group.clear();
                    group_ID++;
                    group.push_back(record);
                }
            }
        }

        void flush (void)
        {
            if (!group.empty())
            {
                handleGroup(group_ID, group);
            }
        }
};

class FastaGroupWriter : public Grouper<Fasta>
{
    ofstream fasta_out;
    ofstream group_out;

    virtual bool compare (const Fasta& a, const Fasta& b)
    {
        return a.sequence == b.sequence;
    }

    virtual void handleGroup (int ID, const vector<Fasta>& group)
    {
        Fasta f = group.front();
        fasta_out << ">group-" << ID << endl;
        fasta_out << f.sequence << endl;

        group_out << "group-" << ID << "\t";
        vector<string> headers;
        for (vector<Fasta>::const_iterator iter = group.begin();
             iter != group.end(); ++iter)
        {
            headers.push_back(iter->header);
        }
        group_out << join(headers, "\t") << endl;
    }

    public:
        FastaGroupWriter (const string& fasta_path, const string& group_path)
        : Grouper<Fasta>()
        {
            fasta_out.open(fasta_path.c_str());
            group_out.open(group_path.c_str());
        }
};

class FastaPairGroupWriter : public Grouper<FastaPair>
{
    ofstream fasta_out;
    ofstream group_out;

    virtual bool compare (const FastaPair& one, const FastaPair& two)
    {
        return one.a.sequence == two.a.sequence && one.b.sequence == two.b.sequence;
    }

    virtual void handleGroup (int ID, const vector<FastaPair>& group)
    {
        FastaPair f = group.front();
        fasta_out << ">group-" << ID << endl;
        fasta_out << f.a.sequence << endl;
        fasta_out << ">group-" << ID << endl;
        fasta_out << f.b.sequence << endl;

        group_out << "group-" << ID << "\t";
        vector<string> headers;
        for (vector<FastaPair>::const_iterator iter = group.begin();
             iter != group.end(); ++iter)
        {
            headers.push_back(iter->a.header);
            headers.push_back(iter->b.header);
        }
        group_out << join(headers, "\t") << endl;
    }

    public:
        FastaPairGroupWriter (const string& fasta_path, const string& group_path)
        : Grouper<FastaPair>()
        {
            fasta_out.open(fasta_path.c_str());
            group_out.open(group_path.c_str());
        }
};

template<typename Reader, typename GroupWriter>
void run (vector<ifstream*>& streams, string output_file_path, string groups_file_path)
{
    Reader reader;

    for (vector<ifstream*>::iterator stream = streams.begin();
         stream != streams.end(); ++stream)
    {
        reader.addInput(**stream);
    }

    GroupWriter group_writer(output_file_path, groups_file_path);

    typename Reader::Record next;

    while (reader.read(next))
    {
        group_writer.add(next);
    }
    group_writer.flush();
}


int main (int argc, char* argv[])
{
    string output_file_path, TEMP_DIR;
    vector<ifstream*> streams;
    int MAX_RECORDS;
    bool paired;

    try
    {
        CmdLine cmd("Program description", ' ', VERSION);

        MultiArg<string> fastaFilesArg("f", "fasta", "Input FASTA file", 
                                       true, "input.fasta", cmd);

        ValueArg<string> outputFileArg("o", "output", "Output file", 
                                       true, "", "output.fasta", cmd);

        TCLAP::SwitchArg pairedArg("p", "paired",
            "Records should be sorted in pairs", cmd);

        cmd.parse(argc, argv);

        initInputFileStreams(streams, fastaFilesArg.getValue());
        output_file_path = outputFileArg.getValue();
        paired = pairedArg.getValue();
    }
    catch (ArgException &e)
    {
        cerr << "Error: " << e.error() << " " << e.argId() << endl;
        return 1;
    }

    string groups_file_path = output_file_path + ".groups.txt";

    if (paired)
    {
        typedef MultiReader<FastaPairReader> Reader;
        typedef FastaPairGroupWriter GroupWriter;

        run<Reader, GroupWriter>(streams, output_file_path, groups_file_path);
    }
    else
    {
        typedef MultiReader<FastaReader> Reader;
        typedef FastaGroupWriter GroupWriter;

        run<Reader, GroupWriter>(streams, output_file_path, groups_file_path);
    }
}
