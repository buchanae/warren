#include <iostream>
#include <fstream>

#include <tclap/CmdLine.h>

#include "warren/Alignment.h"
#include "warren/BamMultiReader.h"
#include "warren/BamPool.h"
#include "warren/Splat.h"

#define VERSION "0.1"

using std::endl;
using std::cerr;
using std::cout;

struct CompareSplats
{
    bool operator() (const Alignment& a, const Alignment& b) const
    {
        return (a.RefName < b.RefName)
            || (a.RefName == b.RefName && a.position() < b.position())
            || (a.RefName == b.RefName && a.position() < b.position()
                && (toString(a.CigarData) < toString(b.CigarData)));
    }
};

int main (int argc, char * argv[])
{
    string output_file_path, TEMP_DIR;
    vector<string> bam_file_paths;
    int MAX_RECORDS;

    try
    {
        TCLAP::CmdLine cmd("Program description", ' ', VERSION);

        TCLAP::MultiArg<string> bamFilesArg("b", "bam", "Input BAM file", 
                                         true, "input.bam", cmd);

        TCLAP::ValueArg<int> maxRecordsArg("r", "max-records", 
            "Maximum records per sorted file.", false, 500000, "max", cmd);

        TCLAP::ValueArg<string> tempDirArg("T", "temp-dir", 
            "Temp directory for sorting.", false, "/tmp", "temp directory", cmd);

        TCLAP::ValueArg<string> outputFileArg("o", "output", "Output file", 
                                              true, "", "output.splat", cmd);

        cmd.parse(argc, argv);

        bam_file_paths = bamFilesArg.getValue();
        MAX_RECORDS = maxRecordsArg.getValue();
        TEMP_DIR = tempDirArg.getValue();
        output_file_path = outputFileArg.getValue();

    } catch (TCLAP::ArgException &e) {
        cerr << "Error: " << e.error() << " " << e.argId() << endl;
    }

    std::ostream* output_stream;
    std::ofstream output_file_stream;

    if (output_file_path == "-")
    {
        cerr << "Outputting to standard out." << endl;
        output_stream = &cout;
    }
    else
    {
        output_file_stream.open(output_file_path.c_str(),
                                std::ios::out | std::ios::trunc);

        if (!output_file_stream.is_open())
        {
            cerr << "Error opening output file. Exiting." << endl;
            return 0;
        }
        output_stream = &output_file_stream;
    }

    BamMultiReader reader;
    reader.Open(bam_file_paths);

    BamTools::RefVector refs = reader.GetReferenceData();
    BamPool<CompareSplats> pool(refs, MAX_RECORDS, TEMP_DIR);

    Alignment alignment;
    while (reader.GetNextAlignment(alignment))
    {
        if (isSplat(alignment)) pool.add(alignment);
    }

    BamPoolReader<CompareSplats> pool_reader = pool.getReader();
    Splat* prev = NULL;

    while (pool_reader.GetNextAlignment(alignment))
    {
        Splat* splat = new Splat(alignment);

        if (prev == NULL)
        {
            prev = splat;
        }
        else
        {
            if (splat->position == prev->position)
            {
                splat->merge( *prev );
            }
            else 
            {
                string out;
                prev->toString(out);
                *output_stream << out << endl;
            }

            delete prev;
            prev = splat;
        }
    }
    string out;
    prev->toString(out);
    *output_stream << out << endl;
}
