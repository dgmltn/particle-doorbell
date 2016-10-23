#!/usr/bin/perl

# This splits a file containing many rtttl tones into many files, named appropriately

# cat test.txt | perl split.pl

while (my $match = <STDIN>) {
    $match =~ /([^:]*):(.*)/;
    my $filename = lc ($1);
    $filename =~ s/\W+/_/g;
    $filename =~ s/_$//;
    $filename =~ s/^_//;
    $filename =~ s/^([0-9])/_\1/;
    $filename = "$filename.rtttl";
    print "$filename\n$match";
    print "\n";
    open (my $fh, '>', $filename);
    print $fh "$match";
    close $fh;
}
