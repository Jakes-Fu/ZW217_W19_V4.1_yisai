#!/usr/bin/perl

$NUM_ARGV = @ARGV;
$pre_str = "";
if($NUM_ARGV == 2)
{
	$item_list  = $ARGV[0]; # directory list
	$pre_str  = $ARGV[1];   # a prefix string for each directory
}
else
{
	if($NUM_ARGV == 1)
	{
		$item_list  = $ARGV[0];
	}
	else
	{
		return 1;
	}
}
@all_item = split(" ",$item_list);
foreach $item(@all_item)
{
	$item=~s/(^\s+|\s+$)//g;
	$mydir = "$pre_str/$item";
	$mydir = $item if($pre_str eq ""); 

	if($item ne "")
	{
		if(!-e $mydir)
		{
			mkdir($mydir) ;
			print "Directory: $mydir create OK! \r\n";
		}
	}	
}

