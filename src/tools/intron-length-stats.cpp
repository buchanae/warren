#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <tclap/CmdLine.h>

#include "warren/Feature.h"
#include "warren/Index.h"
#include "warren/GFFReader.h"
#include "warren/junctions.h"

#define VERSION "0.1"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main (int argc, char* argv[])
{
    string gff_file_path, output_file_path;

    try
    {
        TCLAP::CmdLine cmd("Program description", ' ', VERSION);

        TCLAP::ValueArg<string> inputGFF("g", "gff-file", "Input GFF file", true, "", "input_file.gff", cmd);
        TCLAP::ValueArg<string> outputFileArg("o", "output", "Output file", true, "", "output.coverage", cmd);

        cmd.parse(argc, argv);

        gff_file_path = inputGFF.getValue();
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

    // open GFF reference file
    std::ifstream gff_stream(gff_file_path.c_str());
    if (!gff_stream.is_open())
    {
        cerr << "Error opening reference GFF file. Exiting." << endl;
        return 0;
    }

    cerr << "Loading splice junctions from reference GFF." << endl;

    vector<Feature> transcripts;
    ChildrenIndex exons_index;
    GFFReader gff_reader(gff_stream);
    Feature f;

    while (gff_reader.read(f))
    {
        if (f.isTranscriptType())
        {
            transcripts.push_back(f);
        }

        if (f.isExonType())
        {
            exons_index.add(f);
        }
    }

    cerr << "Calculating intron length stats." << endl;

    vector<int> percent_counts(101, 0);

    for (vector<Feature>::iterator transcript = transcripts.begin(); 
         transcript != transcripts.end(); ++transcript)
    {
        vector<Feature> junctions;
        vector<Feature> exons;
        exons_index.childrenOf(*transcript, exons);

        getJunctions(exons, junctions);

        if (junctions.size() > 0)
        {
            double total = 0;

            for (vector<Feature>::iterator junction = junctions.begin();
                 junction != junctions.end(); ++junction)
            {
                total += ((double) junction->getLength()) / transcript->getLength();
            }

            int avg_percent = (int)((total / junctions.size()) * 100);

            percent_counts.at(avg_percent) += 1;
        }
    }

    for (int i = 0; i < percent_counts.size(); ++i) 
    {
        *output_stream << i << "\t" << percent_counts.at(i) << endl;
    }

    return 0;
}
