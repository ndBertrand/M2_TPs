#! /usr/bin/perl
use strict;
use warnings;




#system ("ping -c 4 -q 172.16.38.15");

system(`my @return = 'ping -c 4 -q 172.16.38.15'`);

#$return[$#return-1]=~/([0-9.]+)\% packet loss/;
#$losses = $1;

#print $losses;

#$return[$#return]=~/ [0-9]+\/([0-9.]+)/;
#my $delay = $1;
#print $delay;
