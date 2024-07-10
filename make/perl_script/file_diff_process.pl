#!/usr/bin/perl
BEGIN{push @INC,'./make/perl_script/perl_pm'};
use File::Copy;

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 3)
{
	$file_old  = $ARGV[0];
	$file_new  = $ARGV[1];
	$file_cmp  = $ARGV[2];
}
else
{
	return 1;
}

if(-s $file_cmp)
{
	unlink("$file_old");
	copy($file_new, $file_old) ;
}
