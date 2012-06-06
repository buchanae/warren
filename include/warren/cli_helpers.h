#include <iostream>
#include <fstream>
#include <string>

#include "tclap/CmdLine.h"

#include "warren/BamReader.h"

using std::ifstream;
using std::ios;
using std::string;

void initOutput(std::ostream* output, string file_path)
{
    std::ofstream of;

    if (file_path != "-")
    {
        of.open(file_path.c_str(), ios::out | ios::trunc);
        if (!of.is_open())
        {
            throw TCLAP::ArgException("Could not open the output file: " + file_path);
        }
        output = &of;
    }
    else
    {
        output = &std::cout;
    }
}

void initBamReader(BamReader& reader, string file_path)
{
    if(!reader.Open(file_path))
    {
        throw TCLAP::ArgException("Could not open the BAM file: " + file_path);
    }
}

void initGff(ifstream& gff_stream, string file_path)
{
    gff_stream.open(file_path.c_str());
    if (!gff_stream.is_open())
    {
        throw TCLAP::ArgException("Could not open the GFF file: " + file_path);
    }
}

void initMultipleStackStreams(vector<ifstream*> stack_streams,
                              vector<string> file_paths)
{
    for (vector<string>::iterator file_path = file_paths.begin();
         file_path != file_paths.end(); ++file_path)
    {
        ifstream* stack_stream = new ifstream(file_path->c_str());
        if (!stack_stream->is_open())
        {
            throw TCLAP::ArgException("Could not open the Stack file: " + *file_path);
        }
        stack_streams.push_back(stack_stream);
    }
}
