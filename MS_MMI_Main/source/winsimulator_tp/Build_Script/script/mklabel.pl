#!/usr/bin/perl

################################################################
#  本程序用于给指定的文件打上LABEL。                           #
#                                                              #
#          Author: wind.deng      wind.deng@spreadtrum.com.cn  #
################################################################


###### 定义全局变量 ##################
$CCUSER = "cathy.cao";
$COMMENT = "MSDev Build";
$LBNAME = "TEMP";
$ADM_VOB = ("Tools");
$VOB_ROOT = "..\\..\\..\\..\\..";


# 需要打LABEL的VOB在此数组中定义
#@VOB_LIST = ("P_Taiji","GMMI","WAP");

######  读取参数   ###################
#
#  Usage: ccperl mklabel.pl lbname
#
#  检查参数
$VAR = $ARGV[0];    #第一个参数
$NUM_ARGV = @ARGV;  #参数的个数

#print "$NUM_ARGV \n";

if($NUM_ARGV = 1)
{
#	print "$VAR \n";
	$LBNAME = $ARGV[0];
}else
{
	do Usage();
}



#
# 创建LABEL TYPE
@CMD = ();
#$CMD[0] = "cleartool mklbtype -global -c \"$COMMENT\" $LBNAME@\\$ADM_VOB";
#$CMD[1] = "cleartool lock -nusers $CCUSER lbtype:$LBNAME@\\$ADM_VOB";
$CMD[2] = "cleartool mklbtype -global -c \"$COMMENT\" $LBNAME@\\MS_Code";
$CMD[3] = "cleartool lock -nusers $CCUSER lbtype:$LBNAME@\\MS_Code";

foreach $CMD (@CMD)
{
	system($CMD);
}

#
#要对@VOB_LIST中的VOB进行LABEL
#foreach $VOB (@VOB_LIST)
#{
#	$MKCMD = "cleartool mklabel -replace -r $LBNAME $VOB_ROOT\\$VOB";
	#system($MKCMD);
#}

@MKCMD = ();
#$MKCMD[0] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\.";
#$MKCMD[1] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common";
#$MKCMD[2] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin";
$MKCMD[3] = "cleartool mklabel -replace -r $LBNAME $VOB_ROOT\\Tools\\Diagnostic\\MSDev";
#$MKCMD[4] = "cleartool mklabel -replace -r $LBNAME $VOB_ROOT\\Tools\\Diagnostic\\MSSim";
#$MKCMD[5] = "cleartool mklabel -replace -r $LBNAME $VOB_ROOT\\Tools\\Common\\Include";
$MKCMD[6] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\ConPan.dll";
$MKCMD[7] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\ConPan.lib";
$MKCMD[8] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\MSDev.exe";
$MKCMD[9] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\MSDevBridge.lib";
$MKCMD[10] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\MSDevKernel.dll";
$MKCMD[11] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\MSDevKernel.lib";
$MKCMD[12] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\filesys.dll";
$MKCMD[13] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\PS\\Lib\\DevPS.lib";
$MKCMD[14] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\Ref\\Lib\\driver_common_win.lib";
$MKCMD[15] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\Ref\\Lib\\driver_windows.lib";
$MKCMD[16] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\Ref\\Lib\\img.lib";
$MKCMD[17] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\Ref\\Lib\\ref_base_win.lib";
$MKCMD[18] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\RTOS\\Lib\\rtos_win.lib";
#$MKCMD[19] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\MSSim_TD_SCDMA.bmp";
#$MKCMD[20] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\MSSim_TD_SCDMA.ini";
$MKCMD[21] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\SIMConPad.dll";
$MKCMD[22] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\Tools\\Common\\Bin\\SIMConPad.lib";
$MKCMD[23] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\MM_Ref\\Lib\\driver_common_win.lib";
$MKCMD[24] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\MM_Ref\\Lib\\driver_windows.lib";
$MKCMD[25] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\MM_Ref\\Lib\\img.lib";
$MKCMD[26] = "cleartool mklabel -replace  $LBNAME $VOB_ROOT\\MS_Code\\MM_Ref\\Lib\\ref_base_win.lib";

foreach $CMD (@MKCMD)
{
	system($CMD);
}

system("cleartool lock -replace lbtype:$LBNAME@\\$ADM_VOB");


###  显示命令用法  ###########################
# Usage: do Usage();
##############################################
sub Usage
{
	print "usage: \n      ccperl mklabel.pl LABEL-NAME \n";
	exit(-1);	
}