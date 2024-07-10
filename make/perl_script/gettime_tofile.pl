#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 1)
{
	$myfile  = $ARGV[0];  # location
}
else
{
	return 1;
}
$now_year = (localtime())[5] + 1900;
$now_month = (localtime())[4] + 1;
$now_day = (localtime())[3];
$currentTime = (localtime())[2].":".(localtime())[1].":".(localtime())[0];
$DateTime = $now_month."-".$now_day."-".$now_year." ".$currentTime;
if(!-e $myfile)
{
	open (inf,">$myfile") || die ("$myfile");
	print inf "#define PROJECT_BUILD_TIME "."\"".$DateTime."\""."\r\n";
	print inf "#define PROJECT_BUILD_YEAR ".$now_year."\r\n";
	close (inf);
}