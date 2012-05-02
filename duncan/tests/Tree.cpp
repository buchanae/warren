/*

initial read
  all = read_gff(file stream)


iterate over immediate children only
  some_feature.children.filterType("transcript")


iterating over types
  it = all.byType("transcript", "mRNA", ...).begin()

  possible to have unknown number of args?  could pass vector of allowed types
  for arity > X

  maybe this should be a filter/where type clause?
    e.g. all.descendents.filterType("transcript"...)


access to a vector of features so you could do custom sorting?

methods for modifying tree?

method for outputting tree back to gff?

*/
