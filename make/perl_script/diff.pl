#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 3)
{
	$input_file1  = $ARGV[0];
	$input_file2  = $ARGV[1];
	$output_file  = $ARGV[2];
}
else
{
	return 1;
}

if(!-e $input_file1  || !-e $input_file2)
{
	open (inf,">$output_file") || die ("$output_file");
	print inf " ";
	close (inf);
}
else
{
	$file1_str="";
	$file2_str="";
	open (inf,$input_file1) || die ("$input_file1!!!");
	@file1_all = <inf>;
	close (inf);
	foreach $myline (@file1_all)
	{
		$file1_str =$file1_str."\r\n".$myline;
	}

	open (inf,$input_file2) || die ("$input_file2!!!");
	@file2_all = <inf>;
	close (inf);
	foreach $myline (@file2_all)
	{
		$file2_str =$file2_str."\r\n".$myline;
	}

	if($file1_str ne $file2_str)
	{
		open (inf,">$output_file") || die ("$output_file");
		print inf " ";
		close (inf);
	}
	else
	{
		open (inf,">$output_file") || die ("$output_file");
		print inf "";
		close (inf);
	}
}