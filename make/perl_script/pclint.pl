#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 3)
{
	$input_dir  = $ARGV[0];
	$module  = $ARGV[1];
	$pclint_path  = $ARGV[2];
}
else
{
	return 1;
}

$_=$module;
chomp;
$_ =~ s/\.lnt//g;
$temp=system(mkdir "$input_dir/$_") if(!-d "$input_dir/$_/") ;
$myPath = $ENV{'PWD'};
$CMD1 = $pclint_path."/lint-nt.exe  -b -i\"$pclint_path/"."makelint\" std.lnt $myPath$input_dir/$_.lnt > $input_dir/$_/aloa.xml";
$CMD2 = $pclint_path."/aloa.exe  $myPath$input_dir/$_/aloa.xml > $myPath$input_dir/$_/Result.txt";
$temp=system($CMD1);
$temp=system($CMD2);