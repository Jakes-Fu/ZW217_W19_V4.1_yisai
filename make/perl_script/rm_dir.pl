#!/usr/bin/perl

BEGIN{push @INC,'./make/perl_script/perl_pm'};
use File::Path;

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$item_list  = $ARGV[0]; # directory list
	$pre_str  = $ARGV[1];   # a prefix string for each dir 
}
else
{
	return 1;
}
@all_item = split(" ",$item_list);
foreach $item(@all_item)
{
	$item=~s/(^\s+|\s+$)//g;
	$mydir = "$pre_str/$item";
	$mydir = $item if($pre_str eq ""); 
	if($item ne "")
	{
		if(-d "$mydir")
		{
			rmtree("$mydir");
			print "Directory: $mydir delete OK! \r\n";
		}
	}	
}