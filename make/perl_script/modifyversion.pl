#!/usr/bin/perl

$NUM_ARGV = @ARGV;
if($NUM_ARGV == 3)
{
	$label  = $ARGV[0];
	$input_file  = $ARGV[1];
	$output_file  = $ARGV[2];
}
else
{
	return 1;
}
open (inf,"$input_file") || die ("file error!!!");
open (inf2,">$output_file") || die ("file error!!!");
@alllines = <inf>;
$outstring = "";
close (inf);
foreach $myline (@alllines)
{
	$_=$myline;
	if($_ =~ /\s*VALUE\s+\"PrivateBuild\"\,/)
	{
		$_ =~ s/\s*VALUE\s+\"PrivateBuild\"\,//g;
		$_ = $&." \"".$label."\\0\""."\r\n";	
	}
	$outstring = $outstring.$_;
} 
print inf2 $outstring;
close (inf2);  
