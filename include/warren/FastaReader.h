#ifndef WARREN_FASTAREADER_H
#define WARREN_FASTAREADER_H

#include <istream>
#include <string>

#include "warren/Fasta.h"

using std::string;

class FastaReader
{
    std::istream& input;

    public:
        FastaReader (std::istream& i) : input(i) {};

        bool read (Fasta& record)
        {
            Fasta temp;
            int c;

            // The following is a DFA for parsing a FASTA record
            enum State
            {
                Init,
                Header,
                Seq
            };
            State state = Init;

            c = input.get();
            while (input.good())
            {
                if (state == Init && c == '>') state = Header;

                else if (state == Header)
                {
                    if (c == '\n') state = Seq;
                    else temp.header += c;
                }

                else if (state == Seq)
                {
                    if (c == '>') 
                    {
                        input.unget();
                        break;
                    }
                    else if (c != '\n') temp.sequence += c;
                }
                c = input.get();
            }

            if (state == Seq)
            {
                record = temp;
                return true;
            }

            return false;
        }
};

#endif
