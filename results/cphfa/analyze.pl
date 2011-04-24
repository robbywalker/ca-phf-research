#!/usr/bin/perl

$strength = $ARGV[0];

opendir(DIR, '..') || die "can't opendir '..': $!";
@cphfs = grep { ! /^\./ && -f "../$_" && /-$strength\.cphf$/ } readdir(DIR);
closedir DIR;

$count = 0;
$rcount = 0;
$fcount = 0;

foreach $cphf ( @cphfs ) {
	$cphf =~ /^(\d+)-(\d+)-(\d+)-(\d+)\./;
	
	if ( $2 > 50 || $3 > 7 ) {
	   print STDERR "Skipping $cphf\n\n";
	   print "$cphf: skipping\n\n";
	   next;
	}

	system "rm -rf *.tmp";
	system "cphf_to_ca ..\\$cphf > test.ca";
	print STDERR "Analyzing $cphf\n";
	$results = `analyze_ca test.ca`;
	
	if ( $results =~ /Count: 0/ ) {
	   print STDERR "Redundant rows\n";
	   print "$cphf: contains redundant rows\n";
	   $rcount++;
	}

	$results =~ /(\d+) elements of (\d+)/m;
	print "$cphf: $1/$2\n\n";
	print STDERR "$1/$2\n\n";
	
	if ( $1 != 0 ) {
	   $fcount++;
	}
	
	$count++;
}

print "Analyzed $count arrays\n";
print "$rcount had redundant rows\n";
print "$fcount had flexible positions\n";