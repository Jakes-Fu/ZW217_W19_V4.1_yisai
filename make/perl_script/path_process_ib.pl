#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 1)
{
	$input_file  = $ARGV[0];
}
else
{
	die $NUM_ARGV;
	return 1;
}
$result="";
$temp1="";
$temp2="";
$temp3="";
$temp4="";
open (inf,$input_file) || die ("$input_file!!!");
@alllines = <inf>;
close (inf);
foreach $line (@alllines)
{
	$_=$line;
	chomp;
	$_ =~ s/^\s+|\s+$//g;
	s/\t+//g;
	s/\\/\//g;
	s/\s/\\ /g;
	($temp1,$temp2)=($_=~m|([^/]+)\.o\s*:(.*)|);
        $temp2=~s/^\\//g;
	$_=$temp1.".cmpl"." ".$temp1.".ibd"." : ".$temp2;
	if($result eq "")
	{
		$result = $_;
	}
	else
	{
		$result = $result."\n".$_;
	}
}

# makefile depend file
$result_t= ProcessStr($result);

print $result_t;

sub ProcessStr
{
	local($string) = @_;
	@files = split("\n",$string);
	undef %saw;
	@out = grep(!$saw{$_}++,@files);
	
	$stringStr="";
	foreach $CurrentFile(@out)
	{
      		$_ = $CurrentFile;
      		chomp;
		if($stringStr eq "")
		{
			$stringStr = $_;
		}
		else
		{
			$stringStr = $stringStr."\n".$_;
		}	
	}
	return $stringStr;
}

