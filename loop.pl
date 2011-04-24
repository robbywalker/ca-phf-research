#!/usr/bin/perl

@searches =

( [3,20,4,4],
  [3,12,2,5],
  [5,16,2,5],
  [3,13,3,5] );
  



while (1) {
   foreach $search ( @searches ) 
   {
   	my ($n,$k,$v,$t) = @$search;
   	$filename = "$n-$k-$v-$t.cphf";
   	
   	unless ( -e $filename ) {
   	   print "\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nProcessing $n $k $v $t\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
   	   system("tabu","--timelimit",20*60,"--movelimit","1200",$n,$k,$v,$t);
   	}
   }
}