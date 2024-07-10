#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 1)
{
	$lib_list  = $ARGV[0];
	@files = split(" ",$lib_list);
	$stringStr="";
	foreach $CurrentFile(@files)
	{
      		$_ = $CurrentFile;
      		chomp;
		if($_ =~ /\\\(/g)
		{
			$_ =~ s/\\\(/(/g;
		}
		if($_ =~/\\\)/g)
		{
			$_ =~ s/\\\)/)/g;
		}
		$stringStr = $stringStr." ".$_;	
	}
	print $stringStr;
}
else
{
	return 1;
}

