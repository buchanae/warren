#include <iostream>

#include "tclap/CmdLine.h"

#include "bamtools/api/BamWriter.h"

#include "warren/cli_helpers.h"
#include "warren/GFFReader.h"
#include "warren/Index.h"
#include "warren/junctions.h"
#include "warren/StackReader.h"

#define VERSION "0.1"

using std::cerr;
using std::endl;
using std::vector;

int main (int argc, char* argv[])
{
    int MIN_SPACE = 10;
    int MAX_SPACE = 1000;

    BamTools::BamWriter writer;
    BamReader bam_reader;
    std::ifstream gff_stream;
    vector<std::ifstream*> stack_streams;

    try
    {
        TCLAP::CmdLine cmd(
            "Filter alignments by spacer size, accounting for splice junctions",
            ' ', VERSION);

        TCLAP::MultiArg<string> stacksArg("s", "stack", "Input Stack file", 
                                          false, "input.stack", cmd);

        TCLAP::ValueArg<string> gffArg("g", "gff", "Input GFF file",
                                       true, "", "input.gff", cmd);

        TCLAP::ValueArg<string> bamArg("b", "bam", "Input BAM file",
                                       true, "", "input.bam", cmd);

        TCLAP::ValueArg<int> minArg("m", "min", "Minimum allowed spacer size",
                                       false, MIN_SPACE, "10", cmd);

        TCLAP::ValueArg<int> maxArg("M", "max", "Input BAM file",
                                       false, MAX_SPACE, "1000", cmd);

        TCLAP::ValueArg<string> outputArg("o", "output", "Output file",
                                          true, "", "output_file", cmd);

        cmd.parse(argc, argv);

        initBamReader(bam_reader, bamArg.getValue());

        if (!writer.Open(outputArg.getValue(), bam_reader.GetHeader(),
            bam_reader.GetReferenceData()))
        {
            throw TCLAP::ArgException("Could not open the output file: " + 
                                      outputArg.getValue());
        }

        initGff(gff_stream, gffArg.getValue());

        initMultipleStackStreams(stack_streams, stacksArg.getValue());

        MIN_SPACE = minArg.getValue();
        MAX_SPACE = maxArg.getValue();
    }
    catch (TCLAP::ArgException &e)
    {
        cerr << "Error: " << e.error() << " " << e.argId() << endl;
        return 1;
    }

    UniquePositionIndex junctions;

    cerr << "Loading splice junctions from reference GFF." << endl;

    ChildrenIndex exon_index;
    GFFReader gff_reader;
    Feature f;

    while (gff_reader.getNextFeature(gff_stream, f))
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

    StackReader stack_reader;
    for (vector<std::ifstream*>::iterator stack_stream = stack_streams.begin();
         stack_stream != stack_streams.end(); ++stack_stream)
    {
        Feature j;
        while (stack_reader.getNextFeature(**stack_stream, j))
        {
            junctions.add(j);
        }
    }

    // TODO require alignments are sorted by ID

    Alignment al;
    while (bam_reader.GetNextAlignment(al))
    {
        Alignment mate;
        bam_reader.GetNextAlignment(mate);

        Feature spacer;
        getSpacer(al, mate, spacer);
        int spacer_len = spacer.getLength();

        if (spacer_len >= MIN_SPACE && spacer_len <= MAX_SPACE)
        {
            writer.SaveAlignment(al);
            writer.SaveAlignment(mate);
        }
        else if (spacer_len > MAX_SPACE)
        {
            vector<Feature> overlaps;
            junctions.overlappingFeature(spacer, overlaps);

            typedef vector<vector<Feature> > combos_t;
            combos_t combos;
            nonOverlappingJunctionCombos(overlaps, combos);

            for (combos_t::iterator combo = combos.begin();
                 combo != combos.end(); ++combo)
            {
                int sum = 0;
                for (vector<Feature>::iterator junction = combo->begin();
                     junction != combo->end(); ++junction)
                {
                    sum += junction->getLength();
                }

                int spacer_with_junctions_len = spacer_len - sum;
                if (spacer_with_junctions_len >= MIN_SPACE &&
                    spacer_with_junctions_len <= MAX_SPACE)
                {
                    writer.SaveAlignment(al);
                    writer.SaveAlignment(mate);
                    break;
                }
            }
        }
    }
}
