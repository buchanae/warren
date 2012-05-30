#ifndef _GFF_TOKENIZER_H
#define _GFF_TOKENIZER_H

#include <boost/tokenizer.hpp>

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
typedef tokenizer::iterator token_iter;
typedef boost::char_separator<char> separator;

#endif
