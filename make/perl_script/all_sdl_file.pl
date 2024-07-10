#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 1)
{
	$input_dir  = $ARGV[0];
}
else
{
	return 1;
}
opendir($DIR,$input_dir) or die "$input_dir";
my   $line=readdir($DIR);
while   ($line)   
{   
	if($line   ne   '.'   &&   $line   ne   '..' && $line=~/(\.c$|\.h$)/)   
  	{
		$_ = $line;
		$_ =~ s/^\s+|\s+$//g;
		print $input_dir."/".$_." ";
  	}   
  	$line=readdir($DIR);   
}   
closedir($DIR);   
