#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$pre_str   = $ARGV[0];  # location
	$path_str  = $ARGV[1];  # location
}
else
{
	return 1;
}
$starttime=0;
$endtime=0;
$endtime = time;

open (inf,$path_str) || die ("$path_str");
@alllines = <inf>;
close (inf);
foreach $myline (@alllines)
{
	$_=$myline;
	if($_ =~ /\d+/)
	{
		$starttime = $_;
	}
}
if($starttime ne 0 && $endtime>$starttime)
{
	print "\n$pre_str Time consuming: ";
	print $endtime-$starttime;
	print " seconds!\n";
}