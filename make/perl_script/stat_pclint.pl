#!/usr/bin/perl

# This script stat all warning.
# And generates corresponding '.txt' file.

# Usage: perl stat_warning.pl <input directory>


$number = scalar @ARGV;

if($number < 1){
	die "Error! Usage: perl stat_warning.pl <input directory>\n"
}

#*********************** main entry ***************************
local $in_dir = ${ARGV[0]};

$out_dir = $in_dir;

$out_file = $out_dir."/stat_results.txt";

open(TGT, ">$out_file") or die "Can't open $out_file" ;

$flag_empty = 0;
$flag_start = 0;

opendir ($folder,$in_dir) or die "Can not open $in_dir directory!\n";
local @array=readdir($folder);

for(local $i = 2; $i < @array; $i++)
{
	$full_dir = "";
	$folder_dir = "";
	
	$full_dir = $in_dir."/".$array[$i];
	if(-d $full_dir)
	{
		opendir ($folder_dir,$full_dir) or die "Can not open $full_dir directory!\n";
		local @array_dir = readdir($folder_dir);
	
		for(local $j = 2; $j < @array_dir; $j++)
		{	
			if($array_dir[$j] =~ m/.txt/)
  			{
				statfile($full_dir."\\".$array_dir[$j]);
			}
		}
		closedir($folder_dir);
	}
}

closedir($folder);

close TGT;


#***********************finish ***************************
print "Successfull! Generate at $out_file!\n";

#***********************stat file ***************************
sub statfile{
	my $file = shift;
	open(TASK, "<$file") or die "Can't open $file" ;
	while($L = <TASK>)
	{
		if($L =~ m/^Legend/)
		{
			$flag_start = 0;
		}
		
		if($flag_start == 1)
		{
			print TGT $L;
		}
		
		if($L =~ m/Total number of issues found : ([0-9]+)/)
		{
			if($1 != 0)
			{
				print TGT $full_dir."\\".$array_dir[$j]."\r\n";
				$flag_empty = 0;
				print TGT $L."\r\n";
			}
			else
			{
				$flag_empty = 1;
			}
		}
		
		if($L =~ m/Rank    Msg  Sev  Count/ && $flag_empty != 1)
		{
			$flag_start = 1;
			print TGT $L;
		}
		
	}
	if($flag_empty == 0)
	{
		print TGT "------------------------------------------------------------------------------\r\n";
	}
	close TASK;
}

