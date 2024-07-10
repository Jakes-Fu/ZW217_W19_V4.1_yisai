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
if(!-e $myfile)
{
	open (inf,">$myfile") || die ("$myfile");
	print inf "";
	close (inf);
}
