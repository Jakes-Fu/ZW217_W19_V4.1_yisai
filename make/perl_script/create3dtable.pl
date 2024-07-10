#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 3)
{
	$SPECIAL_EFFECT_ICON_SIZE  = $ARGV[0];
	$MAINLCD_SIZE  = $ARGV[1];
	$object_path  = $ARGV[2];
}
else
{
	return 1;
}
$object_path =~ s/\//\\/g;
@myIS = split("X",$SPECIAL_EFFECT_ICON_SIZE);

@myMS = split("X",$MAINLCD_SIZE);

$CMD = "tools/DEBUG_TOOL/BMP2Array/Bin/BTACmd.exe 3dtable $myIS[0] $myIS[1] $myMS[0] $myMS[1] \"$object_path\"";

$temp=system($CMD);