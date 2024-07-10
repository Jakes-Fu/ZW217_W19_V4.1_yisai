#!/usr/bin/perl

#  FOTA_SUPPORT_ADUPS
$project_name = "";

$NUM_ARGV = @ARGV;
if($NUM_ARGV == 1)
{
	$project_name  = $ARGV[0]; # project
}
else
{
	return 1;
}

$now_year   = (localtime())[5] + 1900;
$now_month  = (localtime())[4] + 1;
$now_day    = (localtime())[3];
$now_hour   = (localtime())[2];
$now_minute = (localtime())[1];
$now_second = (localtime())[0];

$now_month  = "0".$now_month if(length($now_month) ==1);
$now_day    = "0".$now_day if(length($now_day) ==1);
$now_month  = "0".$now_month if(length($now_month) ==1);
$now_hour   = "0".$now_hour if(length($now_hour) ==1);
$now_minute = "0".$now_minute if(length($now_minute) ==1);
$now_second = "0".$now_second if(length($now_second) ==1);

$AdupsDateTime = $now_year.$now_month.$now_day."_".$now_hour.$now_minute;

print "#define ADUPS_BUILD_TIME "."\"".$AdupsDateTime."\""."\r\n";

$ROOT_DIR =~s/\//\\/g;
$myfile = $ROOT_DIR."Third-party\\adups\\hal\\include\\adups_build_datetime.h";

# if(!-e $myfile)
{
	open (inf,">$myfile") || die ("$myfile");
	print inf "#define ADUPS_BUILD_TIME "."\"".$AdupsDateTime."\""."\r\n";
	print inf "#define ADUPS_PROJECT_NAME "."\"".$project_name."\""."\r\n";
	close (inf);
}


