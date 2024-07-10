#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 3)
{
	$FILENAME  = $ARGV[0];
	$IP  = $ARGV[1];
	$PROJECT_NAME  = $ARGV[2];
}
else
{
	return 1;
}
open (inf,"$FILENAME") || die ("file error!!!");
@alllines = <inf>;
close (inf);
$error=1;
foreach $myline (@alllines)
{
	$_=$myline;
	if($_ =~ /^Finished\:.*and 0 error messages\./)
	{
		$error=0;
	}
} 
 
if($error==0)
{
	system("net send $IP $PROJECT_NAME build ok!");
}
else
{
	system("net send $IP $PROJECT_NAME build error!");
}