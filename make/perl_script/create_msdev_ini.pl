#!/usr/bin/perl

$NUM_ARGV = @ARGV;

if($NUM_ARGV == 7)
{
	$PROJECTNAME = $ARGV[0];
	$MAINLCD_SIZE = $ARGV[1];
	$MMI_RES_DIR = lc($ARGV[2]);
	$FILE_NAME_SUFFIX  = $ARGV[3];
	$Root_Path  = $ARGV[4];
	$resource_win  = $ARGV[5];
	$MMI_Dir    = $ARGV[6];
}
elsif($NUM_ARGV == 6)
{
	$PROJECTNAME = $ARGV[0];
	$MAINLCD_SIZE = $ARGV[1];
	$MMI_RES_DIR = lc($ARGV[2]);
	$FILE_NAME_SUFFIX  = $ARGV[3];
	$Root_Path  = $ARGV[4];
	$MMI_Dir  = $ARGV[5];
}
else
{
	return;
}

$MAINLCD_SIZE=~s/X/_/g;

#MSSimCfg.ini
$bmp_path="$Root_Path\\$MMI_Dir\\source\\winsimulator_tp\\skin\\mssim_duallcd_"."$MAINLCD_SIZE"."$FILE_NAME_SUFFIX.png";
$ini_path="$Root_Path\\$MMI_Dir\\source\\winsimulator_tp\\skin\\mssim_duallcd_"."$MAINLCD_SIZE"."$FILE_NAME_SUFFIX.ini";
if($resource_win == "")
{
	$bin_path="$Root_Path\\build\\$PROJECTNAME"."_builddir\\img\\$MMI_RES_DIR".".bin";
}
else
{
	$bin_path="$Root_Path\\build\\$PROJECTNAME"."_builddir\\img\\$MMI_RES_DIR".$resource_win.".bin";
}

$RN = "\n";

$prj_str = "[FILE]$RN";
$prj_str = $prj_str . "bmp_path=$bmp_path$RN";
$prj_str = $prj_str . "ini_path=$ini_path$RN";
$prj_str = $prj_str . "bin_path=$bin_path$RN";

open (inf,">$Root_Path\\build\\$PROJECTNAME"."_builddir\\win\\bin\\MSSimCfg.ini") || die ("$Root_Path\\build\\$PROJECTNAME"."_builddir\\win\\bin\\MSSimCfg.ini");
print inf $prj_str;
close(inf);

#SIMTest.ini
$prj_str = "[SIMTest]$RN";
$BaseDir="$Root_Path\\$MMI_Dir\\source\\winsimulator_tp\\win_platform\\\sim_set_files";
$prj_str = $prj_str . "BaseDir=$BaseDir$RN";

open (inf,">$Root_Path\\build\\$PROJECTNAME"."_builddir\\win\\bin\\SIMTest.ini") || die ("$Root_Path\\build\\$PROJECTNAME"."_builddir\\win\\bin\\SIMTest.ini");
print inf $prj_str;
close(inf);