#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV != 3)
{
	return 1;
}
else
{
	$myDir  = $ARGV[0];
	$mydir1  = $ARGV[1];
	$mydir2  = $ARGV[2];
}
if(-e "$myDir/$mydir1")
{
	if(-d "$myDir/$mydir1")
	{
		print $mydir1;
	}
	else
	{
		print $mydir2;
	}
}
else
{
	print $mydir2;
}
