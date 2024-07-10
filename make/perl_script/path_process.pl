#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 3)
{
	$input_file  = $ARGV[0];
	$module_dep  = $ARGV[1];
	$make_cmd  =   $ARGV[2];
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
	if($_ =~/^\(null\)/)
	{
 	    ($temp1,$temp2)=($_=~m|(.*?):(.*)|);
            $temp2=~s/^\\//g;
            $input_file =~ s/.*\///g;
            $input_file =~ s/\.d\.tmp//g;
	    $_=$input_file.".o"." ".$input_file.".d"." : ".$temp2;
	    $temp4=$input_file.".o"." ".$input_file.".d : ";         
        }
        else
        {
	    ($temp1,$temp2)=($_=~m|([^/]+)\.o\s*:(.*)|);
            $temp2=~s/^\\//g;
	    $_=$temp1.".o"." ".$temp1.".d"." : ".$temp2;
	    $temp4=$temp1.".o"." ".$temp1.".d : ";
        }
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
$result_t= $temp4.$module_dep."\n".ProcessStr($result);

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

