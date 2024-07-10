#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV != 3)
{
	return 1;
}
else
{
	$myDir  = $ARGV[0];
	$myfile1  = $ARGV[1];
	$myfile2  = $ARGV[2];
}
if(-e "$myDir/$myfile1")
{
	print $myfile1;
}
else
{
	print $myfile2;
}
