#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 1)
{
	$input_file  = $ARGV[0];
}
else
{
	return 1;
}

$result='';

open (inf,$input_file) || die ("file error!!!");
@alllines = <inf>;
close (inf);
foreach $myline (@alllines)
{
	$_=$myline;
	chomp;
	$_=~s/^\s+|\s+$//g;

	if($_ =~ m/cust_def_flag\s*=\s*(.*)/)
	{	
		$result = $1;
                last;
	} 
}

print $result;