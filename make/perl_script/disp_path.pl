#!/usr/bin/perl

#tp_func.o tp_func.d:MS_MMI/source/mmi_app/app/tp/h/tp_export.h

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 1)
{
	$myfile  = $ARGV[0];  # location
}
else
{
	return 1;
}
$str_out="";
if(-e $myfile)
{
		open (inf,"$myfile") || die ("$myfile");
		@alllines = <inf>;
		close (inf);
		$temp_i=0;
		foreach $myline (@alllines)
		{
			$temp_i++;
			next if($temp_i<=2);
			$_=$myline;
			$_=~s/.*:\s+//;
			$_=~s/\/[^\/]+$//g;
			s/(^\s+|\s+$)//g;
			if($_ =~/\\/)
			{
				$_ = "\"".$_."\"";
				s/\\//g;
			}
			$str_out = $str_out."\n-I".$_;
		}
}

print ProcessStr($str_out);

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
			$stringStr = $stringStr." ".$_;
		}	
	}
	return $stringStr;
}