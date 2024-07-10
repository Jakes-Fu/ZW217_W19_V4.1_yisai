#!/usr/bin/perl

################################################################
#  Checkout �ض����ļ���                                       #
#                                                              #
#          Author: wind.deng      wind.deng@spreadtrum.com.cn  #
################################################################


###### ����ȫ�ֱ��� ##################
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


# ��Ҫ��LABEL��VOB�ڴ������ж���
#@VOB_LIST = ("P_Taiji","GMMI","WAP");

######  ��ȡ����   ###################
#
#  Usage: ccperl coci.pl co | ci
#
#  ������
$VAR = $ARGV[0];    #��һ������
$NUM_ARGV = @ARGV;  #�����ĸ���

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



###  ��ʾ�����÷�  ###########################
# Usage: do Usage();
##############################################
sub Usage
{
	print "usage: \n      ccperl coci.pl [co|ci]\n";
	exit(-1);	
}