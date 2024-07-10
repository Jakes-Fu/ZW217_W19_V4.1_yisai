#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 2)
{
	$input_file  = $ARGV[0];
	$dloader_file  = $ARGV[1];
}
else
{
	return 1;
}
$str="";
open (inf,$input_file) || die ("$input_file!!!");
@alllines = <inf>;
close (inf);
foreach (@alllines)
{
	$str.=$_;
}
$language_item="";
if($str =~/\n\s*typedef enum\s*\n*\{([^}]+)\}\s*MMISET_LANGUAGE_TYPE_E\;/sg)
{
	$language_item = $1;
}
@allitem = split("\n",$language_item);
$outstring = "";
$dloader="";
$index = -1;
foreach (@allitem)
{
	$tmp = $_;
	$tmp =~ s/(^\s+|\s+$)//g;
	if($tmp =~ /MMISET_LANGUAGE_/)
	{
		$index ++;
		$currentLanguage = $tmp;
		$currentLanguage =~ s/MMISET_LANGUAGE_//g;
		$currentLanguage =~ s/,//g;
		$dloader.=($index+1)."=".$currentLanguage."\n";
		if($tmp =~ /ENGLISH/)
		{
#			$outstring .= "#if defined(MULTI_LAN_SUPPORT)\n";
#			$outstring .= "ProductionParam\\multi_langue_cfg\\flag\\flag[$index]=0x5\n";
			$outstring .= "#if defined(IM_".$currentLanguage."_SUPPORT)\n";
			$outstring .= "ProductionParam\\multi_langue_cfg\\flag\\flag[$index]=0x7\n";
			$outstring .= "#endif\n";	
		}
		else
		{
#			$outstring .= "#if defined(MULTI_LAN_SUPPORT) && defined(IM_".$currentLanguage."_SUPPORT)\n";
			$outstring .= "#if defined(IM_".$currentLanguage."_SUPPORT)\n";
			$outstring .= "ProductionParam\\multi_langue_cfg\\flag\\flag[$index]=0x7\n";
			$outstring .= "#endif\n";	
		}
	}
}
print $outstring;

open (inf,$dloader_file) || die ("$dloader_file!!!");
@alllines = <inf>;
close (inf);

$str="";
foreach (@alllines)
{
	$str.=$_;
}

$str =~ s/\n\s*\[MultiLangs\][^[]+/\n\n\[MultiLangs\]\n$dloader\n/g; 

open (inf,">$dloader_file") || die ("$dloader_file");
print inf $str;
close (inf);


