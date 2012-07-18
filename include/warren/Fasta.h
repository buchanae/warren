#ifndef WARREN_FASTA_H
#define WARREN_FASTA_H

#include <string>

using std::string;

struct Fasta
{
    string header;
    string sequence;

    string toString (void)
    {
        return ">" + header + "\n" + sequence;
    }
};

struct FastaPair
{
    Fasta a;
    Fasta b;

    string toString (void)
    {
        return a.toString() + "\n" + b.toString();
    }
};

#endif
