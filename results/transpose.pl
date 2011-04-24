@arr = ();

open TEMP, '<temp.txt';

$i = 0;
while (<TEMP>) {
	print $_;
	chomp;
	$arr[$i++] = [ split /\s+/, $_ ];
}

for ( $j = 0; $j < 0+@{$arr[0]}; $j++ ) {
	for ( $i = 0; $i < 0+@arr; $i++ ) {
		print $arr[$i]->[$j] . ' & ';
	}
	print "\n";
}
