#!/usr/bin/perl

################################################################
#  Checkout 特定的文件。                                       #
#                                                              #
#          Author: wind.deng      wind.deng@spreadtrum.com.cn  #
################################################################


###### 定义全局变量 ##################
$OP_KIND = "co";
$PATH_ROOT = "..\\..\\..\\..\\Common\\Bin";
$COMMENT = "Build MSDev, automated operation do ......       ";
@files = ("MSDev.exe",
		 "MSDevBridge.lib",
		 "MSDevKernel.dll",  
		 "MSDevKernel.lib",  
		 "SIMConPad.lib", 
		 "SIMConPad.dll", 
		 "ConPan.dll",  
		 "ConPan.lib"
);

$filestr = "";
foreach $file (@files)
{ 
	$filestr = $filestr." "."$PATH_ROOT\\$file";
}


# 需要打LABEL的VOB在此数组中定义
#@VOB_LIST = ("P_Taiji","GMMI","WAP");

######  读取参数   ###################
#
#  Usage: ccperl coci.pl co | ci
#
#  检查参数
$VAR = $ARGV[0];    #第一个参数
$NUM_ARGV = @ARGV;  #参数的个数

#print "$NUM_ARGV \n";

if($NUM_ARGV = 1)
{
	$OP_KIND = $ARGV[0];
}else
{
	do Usage();
}



#
# checkout files
#foreach $file (@files)
#{
	#$CMD = "cleartool $OP_KIND -c \"$COMMENT\" $PATH_ROOT\\$file";
	$CMD = "cleartool $OP_KIND -c \"$COMMENT\" $filestr";
	system($CMD);
#}



###  显示命令用法  ###########################
# Usage: do Usage();
##############################################
sub Usage
{
	print "usage: \n      ccperl coci.pl [co|ci]\n";
	exit(-1);	
}