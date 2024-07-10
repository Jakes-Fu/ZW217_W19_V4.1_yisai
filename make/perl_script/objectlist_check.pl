#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$input_dir  = $ARGV[0];
	$input_file  = $ARGV[1];
}
else
{
	return 1;
}
%map = (); 

open (inf,$input_dir."/".$input_file) || die ("file error!!!");
@alllines = split(" ",<inf>);
close (inf);
foreach $myline (@alllines)
{
	$_=$myline;
	chomp;
	$_ =~ s/^\s+|\s+$//g;
	$map{$_}=1;
}
opendir($DIR,$input_dir) or die "$input_dir";
my   $line=readdir($DIR);
while   ($line)   
{   
	if($line ne '.'  &&  $line ne '..'  &&  $line ne $input_file)   
  	{
		$_=$line;
		chomp;
		$_ =~ s/^\s+|\s+$//g;
		if($map{$_}!=1)
		{
			unlink($input_dir."/".$line);
			print "delete ".$input_dir."/".$line."!\r\n";
		}
		else
		{
			$map{$_}=2;
		}
  	}   
  	$line=readdir($DIR);   
}   
closedir($DIR);   

foreach $myline (@alllines)
{
	$_=$myline;
	chomp;
	$_ =~ s/^\s+|\s+$//g;
	if($map{$_}!=2)
	{
		print "\r\n$_ error!!!\r\n\r\n";
		return 1;
	}
}