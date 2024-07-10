#!/usr/bin/perl

$NUM_ARGV = @ARGV;
$pre_str = "";
$img_main = "";
$img_main_src = "";
$img_main_dst = "";
$img_pac = "";
$img_pac_src = "";
$img_pac_dst = "";

$ROOT_DIR =~s/\//\\/g;
$BUILD_DIR =  $ROOT_DIR."\\build\\$PROJECT_NAME"."_builddir";
$dloader_ini = "$BUILD_DIR\\ResearchDownload\\ResearchDownload.ini";
$abup_define = $ROOT_DIR."Third-party\\abup\\hal\\include\\abup_define.h";
$abup_info_name = "adups_info.txt";
$abup_info = "";

$abup_version = "";
$abup_project = "";

#
$abup_build_datetime = $ROOT_DIR."Third-party\\abup\\hal\\include\\abup_build_datetime.h";

$DateTime = "";

if(-e $abup_build_datetime)
{
    open (ABUPTIME,$abup_build_datetime) || die "$abup_build_datetime";
    @alllines = <ABUPTIME>;
    $str="";
    foreach (@alllines)
    {
		$str=$_;
		# print $str;
		if($str =~ /#define ABUP_BUILD/)		
		{
			print $str;
			if ($str =~ m/#define ABUP_BUILD_TIME "([\w+]{1,})/g) 
			{
				$DateTime = $1;
				#print $1;
				#print "\r\n";
			}
		}
    }
	
	close ABUPTIME;
}


# mkdir outdir & copy image/pac
if($NUM_ARGV == 5)
{
	$item_list  = $ARGV[0]; # directory list
	$pre_str  = $ARGV[1];   # a prefix string for each directory
	$img_main  = $ARGV[2];  # main rom image bin
	$img_pac  = $ARGV[3];   # pac image
	$abup_project  = $ARGV[4];   # pac image
}
else
{
	if($NUM_ARGV == 1)
	{
		$item_list  = $ARGV[0];
	}
	else
	{
		return 1;
	}
}
@all_item = split(" ",$item_list);
foreach $item(@all_item)
{
	$item=~s/(^\s+|\s+$)//g;
	$mydir = "$pre_str/$item";
	$mydir = $item if($pre_str eq "");

	$mydir_out = "build\\".$mydir."\\img\\"."ABUP_UIX8910_".$DateTime;

	#$img_main_src = "build\\".$mydir."\\img\\".$img_main;
	#$img_main_dst = $mydir_out."\\".$img_main;
	#$img_pac_src = "build\\".$mydir."\\img\\".$img_pac;
	#$img_pac_dst = $mydir_out."\\".$img_pac;

	$img_main_src = "build\\".$mydir."\\img\\UIX8910_UIS8910_240x320BAR_16MB_DS_DEBUG_stone.bin";
	$img_cp_src  = "build\\".$mydir."\\img\\UIX8910_UIS8910_240x320BAR_16MB_DS_DEBUG_cp.bin";
	$img_res_src  = "build\\".$mydir."\\img\\mmi_res_240x320.bin";
	$img_nv_src  = "build\\".$mydir."\\img\\nvitem.bin";
	$img_delta_nv_src  = "build\\".$mydir."\\img\\UIS8910_240x320BAR_16MB_DS_DEBUG_delta_nv.bin";

	$abup_info = $mydir_out."\\".$abup_info_name;
	$abup_version = "version=".$abup_project."_".$DateTime;

	print "Version: $abup_version\r\n";
	
	if($item ne "")
	{
		if(!-e $mydir_out)
		{
			mkdir($mydir_out) ;
			print "Directory: $mydir_out create OK! \r\n";
			print "Copy src:$img_main_src, dst:$mydir_out \r\n";
		}
	}

	# copy image
	if(-e $img_main_src)
	{
		system ("copy /B /Y $img_main_src $mydir_out ");
	}
	if(-e $img_cp_src)
	{
		system ("copy /B /Y $img_cp_src $mydir_out ");
	}
	if(-e $img_res_src)
	{
		system ("copy /B /Y $img_res_src $mydir_out ");
	}
	if(-e $img_nv_src)
	{
		system ("copy /B /Y $img_nv_src $mydir_out ");
	}	
	if(-e $img_delta_nv_src)
	{
		system ("copy /B /Y $img_delta_nv_src $mydir_out ");
	}
}

# generate adups_info.txt
if(-e $abup_define)
{
	open(F,'>adups_info.txt');

    open (ABUP,$abup_define) || die "$abup_define";
    @alllines = <ABUP>;
    $str="";
    foreach (@alllines)
    {
		$str=$_;
		# print $str;
		if($str =~ /#define ADUPS_FOTA/)
		{
			print F $str ;
		}
    }
	
	close ABUP;
	print F $abup_version; 
	close F;
	system("move /Y adups_info.txt $mydir_out");

	#system("perl make\\perl_script\\abup_zip.pl $mydir_out ABUP_UIX8910_$DateTime") && die "Sender2.pl fail\n";

}
