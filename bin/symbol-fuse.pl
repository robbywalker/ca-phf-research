#!/usr/bin/perl

if ( $#ARGV != 0 ) {
	print STDERR "Usage: symbol-fuse.pl in.ca";
	exit;
}

$infile = $ARGV[0];

unless ( -e $infile ) {
	print STDERR "Specified file '$infile' does not exist.";
	exit;
}

open INFILE, $infile;

$preamble = <INFILE>;
my ($N, $k, $v, $t) = split /\s+/, $preamble;

$new_v = $v - 1;

$outfile = "$t-$k-$new_v.ca";

open OUTFILE, ">$outfile";

print OUTFILE "$N $k $new_v $t\n";

while ( $x = <INFILE> ) {
	chomp( $x );
	next if ( $x =~ /^\s+$/ );
	
	print OUTFILE join( " ", map( { ($_ == ($v-1)) ? int(rand($new_v)) : $_ } split(/\s+/, $x))), "\n";
}

close INFILE;
close OUTFILE;