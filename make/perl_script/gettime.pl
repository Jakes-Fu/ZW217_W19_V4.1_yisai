#!/usr/bin/perl
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

$currentTime = $now_hour.":".$now_minute.":".$now_second;
$DateTime = $now_year.$now_month.$now_day." ".$currentTime;#$DateTime = $now_month."-".$now_day."-".$now_year." ".$currentTime;

print "#define PROJECT_BUILD_TIME "."\"".$DateTime."\""."\r\n";
print "#define PROJECT_BUILD_YEAR ".$now_year."\r\n";