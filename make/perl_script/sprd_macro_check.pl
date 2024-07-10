#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$input_file1  = $ARGV[0];
	$input_file2  = $ARGV[1];
}
else
{
	return 1;	
}

if(!-e $input_file1  || !-e $input_file2)
{
	return 1;
}
else
{
	$file1_str="";
	$file2_str="";
	open (inf,$input_file1) || die ("$input_file1!!!");
	$file1 = <inf>;
	@file1_all = split('\s+',$file1);
	close (inf);

	open (inf,$input_file2) || die ("$input_file2!!!");
	$file2 = <inf>;
	@file2_all = split('\s+',$file2);
	close (inf);

	$NUM_file1 = @file1_all;
	$NUM_file2 = @file2_all;

	for($i=0;$i<($NUM_file1-1) && $i<($NUM_file2-1);$i++)
	{
		if($file1_all[$i] ne $file2_all[$i])
		{
			print "\n$file1_all[$i] don't match $file2_all[$i]!\n\n";
			return 1;
		}
	}	

	if($NUM_file1 lt $NUM_file2)
	{
		print "\n";
		for($i=$NUM_file1;$i<($NUM_file2-1);$i++)
		{
			print "$file2_all[$i] ";
		}
		print "is missing!\n\n";
		return 1;
	}
	if($NUM_file1 gt $NUM_file2)
	{
		print "\n";
		for($i=$NUM_file2;$i<($NUM_file1-1);$i++)
		{
			print "$file1_all[$i] ";
		}
		print "don't be found in SPRD MACRO list!\n\n";
		return 1;
	}
}