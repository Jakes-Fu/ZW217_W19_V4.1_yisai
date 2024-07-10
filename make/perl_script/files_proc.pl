#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$input_dir  = $ARGV[0];
	$processCMD = $ARGV[1];
}
else
{
	return 1;
}

if(-e "$input_dir")
{
    opendir($DIR,"$input_dir") or die "$input_dir";
    my   $line=readdir($DIR);
    while   ($line)   
    {   
	if($line ne '.'  &&  $line ne '..')   
  	{
		$_=$line;
		chomp;
		$_ =~ s/^\s+|\s+$//g;
                print $_;
		rename("$input_dir/$_","$input_dir/$_".".bin") if($processCMD eq "rename");
		unlink("$input_dir/$_") if($processCMD eq "del");
  	}   
  	$line=readdir($DIR);   
    }   
    closedir($DIR); 
}
