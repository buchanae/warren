#include "gtest/gtest.h"
#include "gmock/gmock.h"


TEST(BamDbTest, open)
{
    BamDb db = new BamDb("path/to/bamdb");

    // test error when opening a file that isn't a bam db
    // bam dbs must have a specific header, to separate them from regular bam files
    EXPECT_THROW(new BamDb("path/to/not_a_bamdb"), BamDb::ArgException);

    // test that file is created
    new BamDb("path/to/new_bamdb");

    // test creating file failed
    EXPECT_THROW(new BamDb("path/to/not_writable/new_bamdb"));

    // test reading file failed
    EXPECT_THROW(new BamDb("path/to/not_readable"));

    // test error when no db file given (shell test)

    // option to disable sort check

    // loading an unsorted file will sort that file into a temp. file, then insert

    db->insert(BamFileReader("path/to/input.bam"));
    db->insert(SamFileReader("path/to/input.sam"));

    // try to insert a file that's not readable

    // temp directory is configurable

    db->query("ATCG");
}

insert? or add?

bamdb-insert --bam path/to/input.bam --db path/to/dbfile
bamdb-insert --sam path/to/input.sam --db path/to/dbfile
bamdb-insert --bam path/to/input.bam --sam path/to/input.sam --db path/to/dbfile
bamdb-insert --bam path/to/input.bam --bam path/to/input.bam --db path/to/dbfile
bamdb-insert --no-sort-check ...
bamdb-insert --temp-dir path/to/tmp ...

bamdb-get path/to/dbfile ATCG
bamdb-get path/to/dbfile --fasta queries.fasta > output.sam

bamdb-get_all path/to/dbfile > all.sam

bamdb-status path/to/dbfile

bamdb-checkup path/to/dbfile
