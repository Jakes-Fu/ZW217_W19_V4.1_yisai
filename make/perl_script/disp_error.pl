#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 3)
{
	$item_list = $ARGV[0];  # modules list
	$path_str  = $ARGV[1];  # location
        $project_name  = $ARGV[2];  # location
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
			foreach $myline (@alllines)
			{
				$_=$myline;
				if($_ =~ /: Serious error:/ || $_ =~ /: Error:/ || $_ =~ /^Error:/ || $_ =~ / : error C/ || $_ =~ /fatal error C/ || $_ =~ /LINK : fatal error / || $_ =~ /: error LNK/ || $_ =~ /failed$/  || $_ =~ /\*\*\* No rule to make target/ || $_ =~ /Fatal error:/ ||  $_ =~ /^\[Error\]/)
				{
					$str_out = $str_out. "[".$item."] ".$_;
				}
			}
		}
	}
}
if($str_out ne "")
{
	print "\n******************************** $project_name Error list ********************************\n\n";
	print $str_out;
}