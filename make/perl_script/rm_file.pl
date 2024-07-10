#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$item_list  = $ARGV[0]; # file list
	$path_str  = $ARGV[1];  # a prefix string for each file
}
else
{
	return 1;
}
if($item_list=~/\*/)
{
	$item_list=~s/\*//g;
	if(-e $path_str)
	{
		opendir($DIR,$path_str) or die "$path_str";
		my   $line=readdir($DIR);
		while   ($line)   
		{   
			if($line ne '.'  &&  $line ne '..')   
  			{
				$_=$line;
				chomp;
				$_ =~ s/^\s+|\s+$//g;
				if($line=~/$item_list$/)
				{
					unlink($path_str."/".$line);
					#print "delete ".$path_str."/".$line."!\r\n";
				}
  			}   
  			$line=readdir($DIR);   
		}   
		closedir($DIR); 
	}  
}
else
{
	@all_item = split(" ",$item_list);
	foreach $item(@all_item)
	{
		$item =~ s/(^\s+|\s+$)//g;
		if($item ne "")
		{
			if(-e $path_str.$item)
			{
				unlink($path_str.$item);
				if($item=~/(\.tmp$|\.cmp$|\.xml$|\.txt$)/)
				{
					#
				}
				else
				{
					print "File: ".$path_str.$item." delete OK! \n";
				}
			}
		}	
	}
}
