#!/usr/bin/perl

open INPUT, $ARGV[0];
$last = '';

while ( <INPUT> ) {
	$line = $_;
	while ( $line =~ /<b>([^<\s]+)<\/b>/gi ) {
		$x = $1;
		$x =~ tr/A-Z\-/a-z/;
		if ( $last ne $x && length($x) == $ARGV[1] ) {
			print $x . "\n";
		}
		$last = $x;		
	}
}