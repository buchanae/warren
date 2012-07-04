#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include <tclap/CmdLine.h>

#include "warren/Alignment.h"
#include "warren/BamReader.h"
#include "warren/Coverage.h"
#include "warren/Feature.h"
#include "warren/Index.h"
#include "warren/GFFReader.h"
#include "warren/junctions.h"
#include "warren/StackReader.h"

#define VERSION "0.1"

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main (int argc, char* argv[])
{
    string gff_file_path, bam_file_path, output_file_path;
    vector<string> stack_file_paths;

    // TODO allow multiple bam files?
    try
    {
        TCLAP::CmdLine cmd("Program description", ' ', VERSION);

        TCLAP::MultiArg<string> inputSTACKS("s", "stack-file", "Stack file", false, "foo.stacks", cmd);
        TCLAP::ValueArg<string> inputGFF("g", "gff-file", "Input GFF file", true, "", "input_file.gff", cmd);
        TCLAP::ValueArg<string> inputBAM("b", "bam-file", "Input BAM file", true, "", "input_file.bam", cmd);
        TCLAP::ValueArg<string> outputFileArg("o", "output", "Output file", true, "", "output.coverage", cmd);

        cmd.parse(argc, argv);

        gff_file_path = inputGFF.getValue();
        bam_file_path = inputBAM.getValue();
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

    BamReader reader;
    if(!reader.Open(bam_file_path))
    {
        cerr << "Error opening the bam file. Exiting." << endl;
        return 0;
    }

    cerr << "Loading the reference GFF." << endl;

    // open GFF reference file
    std::ifstream gff_stream(gff_file_path.c_str());
    if (!gff_stream.is_open())
    {
        cerr << "Error opening reference GFF file. Exiting." << endl;
        return 0;
    }

    cerr << "Loading splice junctions from reference GFF." << endl;

    UniquePositionIndex junctions;
    ChildrenIndex exon_index;
    GFFReader gff_reader(gff_stream);
    Feature f;

    while (gff_reader.read(f))
    {
        if (f.isExonType())
        {
            exon_index.add(f);
        }
    }

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

        for (vector<Feature>::iterator junc = juncs.begin(); junc != juncs.end(); ++junc)
        {
            junctions.add(*junc);
        }
    }

    cerr << "Loading splice junctions from stack files." << endl;

    // load splice junctions from stack files
    for (vector<string>::iterator it = stack_file_paths.begin();
         it != stack_file_paths.end(); ++it)
    {
        std::ifstream stack_stream(it->c_str());
        if (!stack_stream.is_open())
        {
            cerr << "Error opening stack file: " << *it << endl;
            cerr << "Skipping file." << endl;
        }
        else
        {
            StackReader stack_reader(stack_stream);
            Feature j;
            while (stack_reader.read(j))
            {
                junctions.add(j);
            }
        }
    }

    cerr << "Found " << junctions.count();
    cerr << " unique splice junctions." << endl;

    Coverage coverage;

    cerr << "Reading alignments and building coverage." << endl;

    // initialize references
    BamTools::RefVector ref_vec = reader.GetReferenceData();
    for (int i = 0; i < ref_vec.size(); ++i)
    {
        BamTools::RefData data = ref_vec.at(i);
        coverage.setMinReferenceLength(data.RefName, data.RefLength);
    }

    // read and filter alignments, adding to coverages
    Alignment al, mate;
    Feature junction;
    while (reader.GetNextAlignment(al))
    {
        if (al.IsPaired())
        {
            bool valid = true;
            if (al.getJunction(junction))
                valid = junctions.contains(junction);

            reader.GetNextAlignment(mate);

            if (mate.getJunction(junction))
                valid = valid && junctions.contains(junction);

            if (valid)
            {
                coverage.add(al);
                coverage.add(mate);
            }
        }
        else
        {
            if (al.getJunction(junction))
                coverage.add(al);
            else
                coverage.add(al);
        }
    }

    reader.Close();

    cerr << "Writing coverage file." << endl;

    coverage.toOutputStream(*output_stream);

    return 0;
}
