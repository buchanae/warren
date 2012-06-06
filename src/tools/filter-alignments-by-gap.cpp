#include <iostream>

#include "tclap/CmdLine.h"

#include "warren/cli_helpers.h"
#include "warren/Index.h"
#include "warren/junctions.h"

#define VERSION "0.1"

using std::cerr;
using std::endl;
using std::vector;

int main (int argc, char* argv[])
{
    const int MAX_GAP = 1000;

    std::ostream* output;
    BamReader bam_reader;
    std::ifstream gff_stream;
    vector<std::ifstream*> stack_streams;

    try
    {
        TCLAP::CmdLine cmd("Filter alignments by gap, accounting for splice junctions",
                           ' ', VERSION);

        TCLAP::MultiArg<string> stacksArg("s", "stack", "Input Stack file", 
                                          false, "input.stack", cmd);

        TCLAP::ValueArg<string> gffArg("g", "gff", "Input GFF file",
                                       true, "", "input.gff", cmd);

        TCLAP::ValueArg<string> bamArg("b", "bam", "Input BAM file",
                                       true, "", "input.bam", cmd);

        TCLAP::ValueArg<string> outputArg("o", "output", "Output file",
                                          true, "", "output_file", cmd);

        cmd.parse(argc, argv);

        // TODO change to Bam output
        initOutput(output, outputArg.getValue());

        initBamReader(bam_reader, bamArg.getValue());

        initGff(gff_stream, gffArg.getValue());

        initMultipleStackStreams(stack_streams, stacksArg.getValue());
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

    for (vector<std::ifstream*>::iterator it = stack_streams.begin();
         it != stack_streams.end(); ++it)
    {
        Feature j;
        while (stack_reader.getNextFeature(stack_stream, j))
        {
            junctions.add(j);
        }
    }

    // TODO require alignments are sorted by ID

    Alignment al;
    while (reader.GetNextAlignment(al))
    {
        if (!al.isPaired())
        {
            output.SaveAlignment(al);
        }
        else if (al.isFirstMate())
        {
            Alignment mate;
            if (reader.GetNextAlignment(mate))
            {
                Feature gap;
                getAlignmentPairGap(al, mate, gap);

                if (gap.getLength() <= MAX_GAP)
                {
                    output.SaveAlignment(al);
                    output.SaveAlignment(mate);
                }
                else
                {
                    vector<Feature> overlaps;
                    junctions.overlappingFeature(gap, overlaps);

                    vector<vector<Feature> > combos;
                    nonOverlappingJunctionCombos(overlaps, combos);

                    if (gap.getLength() - len <= MAX_GAP)
                    {
                        output.SaveAlignment(al);
                        output.SaveAlignment(mate);
                    }
                }
            }
        }
    }
}
