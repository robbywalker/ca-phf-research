#!/usr/bin/perl

@searches =

(
#[38,13,3,3], # score 3 after 200k iterations
#[49,14,3,3], # score 40 after 5k iterations
#[58,15,3,3], # score 68 after 5k iterations
#[84,18,3,3], # score 96 after 1k iterations

  [33,37,4,4],

#[20,11,5,4], #not promising (best score = 6 after 18k iterations)
  [25,14,5,4],

  [14,5,6,4],
  [26,9,6,4],

  [11,15,5,5],
  [12,21,5,5],
  [19,52,5,5],
  [20,56,5,5],

  [17,18,6,5],
  [18,20,6,5],

  [11,5,7,5],
  [21,15,7,5],
  [23,17,7,5],

  [25,5,12,5],

  [15,63,6,6],
  [16,76,6,6],

  [18,38,7,6],

  [13,6,9,6],

  [16,5,11,6],
  [17,6,11,6],

   [240,12,5,3],
   [132,6,6,3],

   [26,13,8,5],
   [27,14,8,5],
   [28,15,8,5],
   [30,16,8,5],

   [31,12,9,5],
   [33,13,9,5],
);


my $done = 1;
while ( $done ) {
   $done = 0;
   foreach $search ( @searches )
   {
      my ($k,$n,$v,$t) = @$search;
      $filename = "$n-$k-$v-$t.phf";

      unless ( -e $filename ) {
         print "\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nProcessing $n $k $v $t\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
         system("tabu","--phf","--fs","--timelimit",15*60,$n,$k,$v,$t);
         $done = 1;
      }
   }
}