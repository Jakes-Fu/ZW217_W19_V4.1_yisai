#!/usr/bin/perl

$myVcxprojFilePath  = $ARGV[0];

$fileExist = -e "$myVcxprojFilePath/msdevkernel.vcxproj";
if( !$fileExist )
{
print "$myVcxprojFilePath/msdevkernel.vcxproj is not exist!";
}
print "$myVcxprojFilePath/msdevkernel.vcxproj";
my $ret = system ("python ./make/perl_script/setVcxprojProperties.py $myVcxprojFilePath");
if($ret != 0)
{
die "set properties failed\n";
}
