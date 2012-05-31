#include <iostream>
#include <fstream>

#include <tclap/CmdLine.h>

#include "AlignmentUtils/Alignment.h"
#include "AlignmentUtils/BamMultiReader.h"
#include "AlignmentUtils/BamPool.h"
#include "AlignmentUtils/Splat.h"

#define VERSION "0.1"

using std::endl;
using std::cerr;
using std::cout;

using AlignmentUtils::Alignment;
using AlignmentUtils::BamMultiReader;
using AlignmentUtils::BamPool;
using AlignmentUtils::BamPoolReader;
using AlignmentUtils::Splat;

struct CompareSplats
{
    bool operator() (const Alignment& a, const Alignment& b) const
    {
        return (a.RefName < b.RefName)
               || (a.RefName == b.RefName && a.position() < b.position())
               || (a.RefName == b.RefName && a.position() < b.position()
                   && (AlignmentUtils::toString(a.CigarData) 
                       < AlignmentUtils::toString(b.CigarData)));
    }
};

int main (int argc, char * argv[])
{
    string output_file_path;
    vector<string> bam_file_paths;

    try
    {
        TCLAP::CmdLine cmd("Program description", ' ', VERSION);

        TCLAP::MultiArg<string> bamFilesArg("b", "bam", "Input BAM file", 
                                         true, "input.bam", cmd);
        TCLAP::ValueArg<string> outputFileArg("o", "output", "Output file", 
                                              true, "", "output.splat", cmd);

        cmd.parse(argc, argv);

        bam_file_paths = bamFilesArg.getValue();
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
    BamPool<CompareSplats> pool(refs);

    Alignment alignment;
    while (reader.GetNextAlignment(alignment))
    {
        if (isSplat(alignment)) pool.add(alignment);
    }

    BamPoolReader<CompareSplats> pool_reader = pool.getReader();
    Splat prev;

    while (pool_reader.GetNextAlignment(alignment))
    {
        Splat splat = toSplat(alignment);

        if (prev.ref.empty()) prev = splat;
        else
        {
            if (splat.position == prev.position) splat.merge( prev );
            else *output_stream << prev.str() << endl;

            prev = splat;
        }
    }

    if (output_file_stream.is_open())
    {
        output_file_stream.close();
    }
}
