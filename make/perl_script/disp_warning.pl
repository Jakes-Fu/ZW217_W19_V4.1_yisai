#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$item_list  = $ARGV[0]." link"; # modules list
	$path_str  = $ARGV[1];  # location
}
else
{
	return 1;
}

@all_item = split(" ",$item_list);
$str_out="";
foreach $item(@all_item)
{
	if($item ne "")
	{
		$item =~ s/\!//g;
		if(-e $path_str."/".$item.".log")
		{
			open (inf,$path_str."/".$item.".log") || die ("$path_str/$item".".log");
			@alllines = <inf>;
			close (inf);
			$all_warning=0;
			foreach $myline (@alllines)
			{
				$_=$myline;
				if($_ =~ /: Warning:/)
				{
					$all_warning++;
				}
			}
			$space="";
			for($j=0;$j<20;$j++)
			{
				if($j>length($item))
				{
					$space.=" ";
				}
			}
			$str_out = $str_out. "[".$item."]$space\t".$all_warning."\n";
		}
	}
}
if($str_out ne "")
{
	print $str_out;
}