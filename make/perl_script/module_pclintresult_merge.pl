#!/usr/bin/perl

BEGIN{push @INC,'./make/perl_script/perl_pm'};
use File::Path;

$NUM_ARGV = @ARGV;
if($NUM_ARGV == 2)
{
	$input_dir   = $ARGV[0];
	$pclint_dir  = $ARGV[1];
}
else
{
	return 1;
}
%modules_pclint = (); 
opendir ($folder_dir,$input_dir) or die "Can not open $input_dir directory!\n";
local @array_dir = readdir($folder_dir);
	
for(local $j = 2; $j < @array_dir; $j++)
{
	$tmp_dir = "$input_dir/$array_dir[$j]";
	if(-d $tmp_dir)
	{
		if($array_dir[$j] =~ m/(.*?)(__\d+__\d+)/)
  		{
			$tmp_warning = "";
			$tmp_module = $1;
			if($modules_pclint{$tmp_module} eq "")
			{
				$modules_pclint{$tmp_module} = "<lintanalysis>\n";
			}
			open (inf,"$tmp_dir/aloa.xml") || die ("$tmp_dir/aloa.xml");
			@alllines = <inf>;
			close (inf);
			foreach (@alllines)
			{
				chomp;
				if($_ !~ /lintanalysis>$/)
				{
					$tmp_warning.="$_\n";
				}
			} 
			$modules_pclint{$tmp_module}.="$tmp_warning\n" if($tmp_warning ne "");
			rmtree("$tmp_dir");
		}
	}
}
closedir($folder_dir);
foreach  (sort keys %modules_pclint) 
{
	$modules_pclint{$_}.="</lintanalysis>\n";
	if(!-e "$input_dir/$_")
	{
		mkdir("$input_dir/$_") ;
	}
	open F,">$input_dir/$_/aloa.xml" or die "$input_dir/$_/aloa.xml";
        print F $modules_pclint{$_};
        close F;
	system("\"$pclint_dir/aloa.exe\" \"$input_dir/$_/aloa.xml\" > \"$input_dir/$_/Result.txt\"");

}