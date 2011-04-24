#!/usr/bin/perl

if ( $#ARGV != 0 ) {
	print STDERR "Usage: remove-redundant.pl in.ca";
	exit;
}

$infile = $ARGV[0];

unless ( -e $infile ) {
	print STDERR "Specified file '$infile' does not exist.";
	exit;
}

while ( 1 ) {
	print "Beginning analysis...\n";
	$analysis = `analyze_ca $infile`;
	print "Analysis finished.\n";

	$row = 0;
	
	foreach $row_text ( split /\n/, $analysis ) {
		if ( $row_text =~ /Row: (\d+); Count: (\d+)/ ) {
			if ( $2 == 0 ) {
				$row = $1;
				last;
			}
		}
	}
	
	last if ( $row == 0 );	
	
	open INFILE, $infile;
	$preamble = <INFILE>;
	my ($N, $k, $v, $t) = split /\s+/, $preamble;

	print "row $row is redundant, new array will have ", $N - 1, " rows \n";
	
	@out = ();
	
	push @out, $N - 1 . " $k $v $t";	
	
	$cur_row = 1;
	while ( $x = <INFILE> ) {
		chomp( $x );
		next if ( $x =~ /^\s+$/ );
		
		if ( $row != $cur_row ) {
			push @out, $x;
		}	
		
		$cur_row++;
	}
	
	close INFILE;
	
	open OUTFILE, ">$infile";
	
	foreach $print_row ( @out ) {
		print OUTFILE $print_row, "\n";
	}
	
	close OUTFILE;	
}
		
		

