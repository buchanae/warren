#include <iostream>
#include <fstream>
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

int main (int argc, char* argv[])
{
    string output_file_path, TEMP_DIR;
    vector<ifstream*> fasta_streams;
    int MAX_RECORDS;

    try
    {
        CmdLine cmd("Program description", ' ', VERSION);

        MultiArg<string> fastaFilesArg("f", "fasta", "Input FASTA file", 
                                       true, "input.fasta", cmd);

        ValueArg<string> outputFileArg("o", "output", "Output file", 
                                       true, "", "output.fasta", cmd);

        cmd.parse(argc, argv);

        initInputFileStreams(fasta_streams, fastaFilesArg.getValue());
        output_file_path = outputFileArg.getValue();
    }
    catch (ArgException &e)
    {
        cerr << "Error: " << e.error() << " " << e.argId() << endl;
        return 1;
    }

    ofstream output_fasta_stream(output_file_path.c_str());

    string groups_file_path = output_file_path + ".groups.txt";
    ofstream output_group_stream(groups_file_path.c_str());

    MultiReader<FastaReader> reader;

    for (vector<ifstream*>::iterator stream = fasta_streams.begin();
         stream != fasta_streams.end(); ++stream)
    {
        reader.addInput(**stream);
    }

    Fasta current;
    reader.read(current);

    vector<string> group;
    group.push_back(current.header);
    int group_i = 1;

    Fasta next;
    while (reader.read(next))
    {
        if (current.sequence == next.sequence)
        {
            group.push_back(next.header);
        }
        else
        {
            output_fasta_stream << ">group-" << group_i << endl;
            output_fasta_stream << current.sequence << endl;

            output_group_stream << "group-" << group_i << "\t";
            output_group_stream << join(group, "\t") << endl;

            group.clear();
            group_i++;
            current = next;
            group.push_back(current.header);
        }
    }

    if (!group.empty())
    {
        output_fasta_stream << ">group-" << group_i << endl;
        output_fasta_stream << current.sequence << endl;

        output_group_stream << "group-" << group_i << "\t";
        output_group_stream << join(group, "\t") << endl;
    }
}
