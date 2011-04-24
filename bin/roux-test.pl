#!/usr/bin/perl

use Data::Dumper;

if ( $#ARGV != 3 ) {
	print STDERR "Usage: roux-test.pl t s k v";
	exit;
} else {
	print STDERR "t = $ARGV[0]\n";
	print STDERR "s = $ARGV[1]\n";
	print STDERR "k = $ARGV[2]\n";
	print STDERR "v = $ARGV[3]\n";
}

my ($t, $s, $k, $v) = @ARGV;

print STDERR "$t-$k-$v.ca : " . (( -e "$t-$k-$v.ca" ) ? "Found" : "REQUIRED") . "\n";

$bigv = $v ** $s;
$t_1 = $t - 1;

print STDERR "$t_1-$k-$bigv.ca : " . (( -e "$t_1-$k-$bigv.ca" ) ? "Found" : "REQUIRED") . "\n";

exit unless (( -e "$t-$k-$v.ca" ) && ( -e "$t_1-$k-$bigv.ca" ));

open ARRAY_A, "$t-$k-$v.ca";
my ($a_n, $a_k, $a_v, $a_t) = split /\s+/, <ARRAY_A>;

open ARRAY_B, "$t_1-$k-$bigv.ca";
my ($b_n, $b_k, $b_v, $b_t) = split/\s+/, <ARRAY_B>;

$N = $a_n + $b_n;
$K = $s * $k;

print "$N $K $v $t\n";

for ( $i = 0; $i < $a_n; $i++ ) {
	chomp( $x = <ARRAY_A> );
	if ( $x =~ /^\s*$/ ) {
		$i--;
		next;
	}
	
	for ( $j = 0; $j < $s; $j++ ) {
		print $x;
	}
	print "\n";
}

close ARRAY_A;

for ( $i = 0; $i < $b_n; $i++ ) {
	chomp( $x = <ARRAY_B> );
	if ( $x =~ /^\s*$/ ) {
		$i--;
		next;
	}

	@x = split/\s+/, $x;	
	
	for ( $j = 0; $j < $k; $j++ ) {
		$val = $x[$j];
		$out = [];
		for ( $l = 0; $l < $s; $l++ ) {
			$out->[$l] = ( $val % $v );
			$val = ($val - $out->[$l]) / $v;
		}	
		$x[$j] = $out;
	}	
	
	for ( $l = 0; $l < $s; $l++ ) {
		for ( $j = 0; $j < $k; $j++ ) {
			print $x[$j]->[$l], " ";
		}
	}
	
	print "\n";
}