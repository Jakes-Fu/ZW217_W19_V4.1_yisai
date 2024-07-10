#!/usr/bin/perl

# FOTA_SUPPORT_ADUPS
# my $ADUPS_BUILD_DATETIME=$DateTime;
# system("echo $ADUPS_BUILD_DATETIME");
# print "#define PROJECT_BUILD_TIME "."\"".$DateTime."\""."\r\n";


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
$adups_define = $ROOT_DIR."Third-party\\adups\\hal\\include\\adups_define.h";
$adups_info_name = "adups_info.txt";
$adups_info = "";

$adups_version = "";
$adups_project = "";

#
$adups_build_datetime = $ROOT_DIR."Third-party\\adups\\hal\\include\\adups_build_datetime.h";

$DateTime = "";

if(-e $adups_build_datetime)
{
    open (ADUPSTIME,$adups_build_datetime) || die "$adups_build_datetime";
    @alllines = <ADUPSTIME>;
    $str="";
    foreach (@alllines)
    {
		$str=$_;
		# print $str;
		if($str =~ /#define ADUPS_BUILD/)		
		{
			print $str;
			if ($str =~ m/#define ADUPS_BUILD_TIME "([\w+]{1,})/g) 
			{
				$DateTime = $1;
				#print $1;
				#print "\r\n";
			}
		}
    }
	
	close ADUPSTIME;
}


# mkdir outdir & copy image/pac
if($NUM_ARGV == 5)
{
	$item_list  = $ARGV[0]; # directory list
	$pre_str  = $ARGV[1];   # a prefix string for each directory
	$img_main  = $ARGV[2];  # main rom image bin
	$img_pac  = $ARGV[3];   # pac image
	$adups_project  = $ARGV[4];   # pac image
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

	$mydir_out = "build\\".$mydir."\\img\\"."ADUPS_SC6531EFM_".$DateTime;
	$img_main_src = "build\\".$mydir."\\img\\".$img_main;
	$img_main_dst = $mydir_out."\\".$img_main;

	$img_pac_src = "build\\".$mydir."\\img\\".$img_pac;
	$img_pac_dst = $mydir_out."\\".$img_pac;
	
	$adups_info = $mydir_out."\\".$adups_info_name;

	$adups_version = "version=".$adups_project."_".$DateTime;

	print "Version: $adups_version\r\n"; 
	
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
		system ("copy /B /Y $img_main_src $img_main_dst ");
	}
	if(-e $img_pac_src)
	{
		system ("copy /B /Y $img_pac_src $img_pac_dst ");
	}
}

# generate adups_info.txt
if(-e $adups_define)
{
	open(F,'>adups_info.txt');

    open (ADUPS,$adups_define) || die "$adups_define";
    @alllines = <ADUPS>;
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
	
	close ADUPS;
	print F $adups_version; 
	close F;
	system("move /Y adups_info.txt $mydir_out");

	system("perl make\\perl_script\\adups_zip.pl $mydir_out ADUPS_SC6531EFM_$DateTime") && die "Sender2.pl fail\n";

}
