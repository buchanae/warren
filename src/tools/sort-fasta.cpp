#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <tclap/CmdLine.h>

#include "warren/cli_helpers.h"
#include "warren/Fasta.h"
#include "warren/FastaReader.h"
#include "warren/FastaSorter.h"
#include "warren/MultiReader.h"

#define VERSION "0.1"

using TCLAP::CmdLine;
using TCLAP::MultiArg;
using TCLAP::ValueArg;
using TCLAP::ArgException;

using std::cerr;
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::endl;
using std::vector;


template<typename Reader, typename Sorter>
void run (vector<ifstream*>& streams, int buffer_size, string& tmp_dir, ostream* out);


int main (int argc, char* argv[])
{
    string output_file_path, tmp_dir;
    vector<ifstream*> streams;
    int buffer_size;
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

        TCLAP::ValueArg<int> bufferSizeArg("r", "max-records", 
            "Maximum records per sorted file.", false,
            FastaSorter::DEFAULT_BUFFER_SIZE, "max", cmd);

        TCLAP::ValueArg<string> tmpDirArg("T", "temp-dir", 
            "Temp directory for sorting.", false,
            FastaSorter::DEFAULT_TMP_DIR, "temp directory", cmd);

        cmd.parse(argc, argv);

        initInputFileStreams(streams, fastaFilesArg.getValue());
        buffer_size = bufferSizeArg.getValue();
        tmp_dir = tmpDirArg.getValue();
        paired = pairedArg.getValue();
        output_file_path = outputFileArg.getValue();
    }
    catch (ArgException &e)
    {
        cerr << "Error: " << e.error() << " " << e.argId() << endl;
        return 1;
    }

    ostream* out = new ofstream(output_file_path.c_str());

    if (paired)
    {
        typedef MultiReader<FastaPairReader> Reader;
        typedef FastaPairSorter Sorter;

        run<Reader, Sorter>(streams, buffer_size, tmp_dir, out);
    }
    else
    {
        typedef MultiReader<FastaReader> Reader;
        typedef FastaSorter Sorter;

        run<Reader, Sorter>(streams, buffer_size, tmp_dir, out);
    }
}

template<typename Reader, typename Sorter>
void run (vector<ifstream*>& streams, int buffer_size, string& tmp_dir, ostream* out)
{
    Reader reader;
    for (vector<ifstream*>::iterator stream = streams.begin();
         stream != streams.end(); ++stream)
    {
        reader.addInput(**stream);
    }

    Sorter sorter;
    sorter.setMaxBufferSize(buffer_size);
    sorter.setTmpDir(tmp_dir);

    typename Reader::Record record;

    while (reader.read(record))
    {
        sorter.add(record);
    }

    typename Sorter::Reader sorted = sorter.getReader();
    while (sorted.read(record))
    {
        *out << record.toString() << endl;
    }
}
