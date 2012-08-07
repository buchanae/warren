#!/bin/bash

# One paired read may product multiple valid paired alignments.
# Before matchmaker, all valid paired alignments will have the same IDs.
# After matchmaker, we want each valid pair of alignments to have a unique ID,
# so we append an incrementing ID to each valid pair.


# (abuchanan says...)
# This is a quick-and-dirty test script.  I haven't decided how to test
# (and automate testing of) the more complex tools like matchmaker.
# This exists because nothing at all, but currently it doesn't run as part of the
# automated unit tests.


# Run this from the build directory.
# Ya, these paths are hard-coded.  Suckage.

./matchmaker -b ../tests/dummies/matchmaker-fix-valid_count.sorted.bam -o combined.bam
samtools view combined.bam > combined.sam
diff ../tests/dummies/matchmaker-fix-valid_count.expected.sam
rm -f combined.sam combined.bam
