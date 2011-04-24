#!/usr/bin/perl

%arrays = ();

opendir CURDIR, ".";

$redundant = 0;

while ( $_ = readdir CURDIR ) {
	if ( m/(\d+)-(\d+)-(\d+)-(\d+)\.cphf/ ) {
		my $id = "$4 $3 $1";
		if ( exists $arrays{$id} ) {
			if ( $arrays{$id} < $2 ) {
				print "Redundant: $1 " . $arrays{$id} . " $3 $4\n";
				$arrays{$id} = $2;
				$redundant++;
			} else {
				print "Redundant: $1 $2 $3 $4\n";
				$redundant++;
			}
		} else {
			$arrays{$id} = $2;
		}
	}
}

closedir CURDIR;

if ( $redundant ) {
	print "\n\n";
}

$lastT = 0;
$lastV = 0;

foreach ( sort sort_func keys %arrays ) {
	$_ =~ /(\d+) (\d+) (\d+)/;
	($t,$v,$n) = ($1,$2,$3);
	
	if ( $lastT && ( $lastT != $t || $lastV != $v ) ) {
		printf("\n");
	}
	
	print "CAN($t," . $arrays{$_} . ",$v) <= " . (( $v ** $t - $v ) * $n + $v) . "\n";
	
	$lastT = $t;
	$lastV = $v;
}


##########################

sub sort_func {
	$a =~ /(\d+) (\d+) (\d+)/;
	($At, $Av, $An) = ($1,$2,$3);
	
	$b =~ /(\d+) (\d+) (\d+)/;
	($Bt, $Bv, $Bn) = ($1,$2,$3);	
	
	return ( $At <=> $Bt ) ? $At <=> $Bt : 
	       ( $Av <=> $Bv ) ? $Av <=> $Bv :
	       ( $An <=> $Bn );
}