#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 1)
{
	$FILENAME  = $ARGV[0];
}
else
{
	return 1;
}
if(!-e "$FILENAME")
{
	print "$FILENAME is not exist!\r\n";
}

