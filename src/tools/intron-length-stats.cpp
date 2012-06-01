#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <tclap/CmdLine.h>

#include "warren/Coverage.h"
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
    vector<string> stack_file_paths;

    try
    {
        TCLAP::CmdLine cmd("Program description", ' ', VERSION);

        TCLAP::MultiArg<string> inputSTACKS("s", "stack-file", "Stack file", false, "foo.stacks", cmd);
        TCLAP::ValueArg<string> inputGFF("g", "gff-file", "Input GFF file", true, "", "input_file.gff", cmd);
        TCLAP::ValueArg<string> outputFileArg("o", "output", "Output file", true, "", "output.coverage", cmd);

        cmd.parse(argc, argv);

        gff_file_path = inputGFF.getValue();
        stack_file_paths = inputSTACKS.getValue();
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

    std::map<string, int> transcript_lengths;
    ChildrenIndex exon_index;
    GFFReader gff_reader;
    Feature f;

    while (gff_reader.getNextFeature(gff_stream, f))
    {
        if (f.isTranscriptType())
        {
            string ID;
            f.attributes.get("ID", ID);
            transcript_lengths.insert(std::make_pair(ID, f.getLength()));
        }

        if (f.isExonType())
        {
            exon_index.add(f);
        }
    }

    cerr << "Calculating intron length stats." << endl;

    vector<int> percent_counts(101, 0);

    vector<string> IDs;
    exon_index.parentIDs(IDs);

    for (vector<string>::iterator ID = IDs.begin(); ID != IDs.end(); ++ID)
    {
        vector<Feature> exons;
        vector<Feature> juncs;
        exon_index.childrenOf(*ID, exons);

        getJunctions(exons, juncs);
        // TODO it's wasteful to have a juncs vector that just gets moved to the index
        //      it'd be nicer if the index implemented the same interface as the vector
        //      this means giving indexes iterators?
        //      c++ you're so complicated...

        if (juncs.size() > 0)
        {
            double total = 0;
            std::map<string, int>::iterator it = transcript_lengths.find(*ID);
            int transcript_length = it->second;

            for (vector<Feature>::iterator junction = juncs.begin();
                 junction != juncs.end(); ++junction)
            {
                total += ((double) junction->getLength()) / transcript_length;
            }

            int avg_percent = (int)((total / juncs.size()) * 100);

            percent_counts.at(avg_percent) += 1;
        }
    }

    for (int i = 0; i < percent_counts.size(); ++i) 
    {
        *output_stream << i << "\t" << percent_counts.at(i) << endl;
    }

    return 0;
}
