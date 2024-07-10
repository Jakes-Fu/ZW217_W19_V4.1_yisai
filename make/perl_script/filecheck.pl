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
if(-e $input_dir)
{
  opendir($DIR,$input_dir) or die "$input_dir";
  my   $line=readdir($DIR);
  while   ($line)   
  {   
	if($line   ne   '.'   &&   $line   ne   '..' && -f $input_dir."/".$line)   
  	{
		open (inf,$input_dir."/".$line) || die ($input_dir."/".$line);
		$error=0;$temp1="";$temp2="";
		@alllines = <inf>;
		foreach $myline (@alllines)
		{
			$_=$myline;
			chomp;
			($temp1,$temp2)=($_=~m|(.*):(.*)|);
			$temp2 =~ s/^\s+|\s+$//g;
			if($temp2 !~ /^\//)
			{
				if(!-e $temp2)
				{
					$error=1;last;
				}
			}
		}
		close (inf);
		if($error==1)
		{
			unlink($input_dir."/".$line);
		}
  	}   
  	$line=readdir($DIR);   
  }   
  closedir($DIR);
}   

