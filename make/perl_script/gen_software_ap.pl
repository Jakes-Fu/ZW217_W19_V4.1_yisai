#!/usr/bin/perl -w
BEGIN{push @INC,'./make/perl_script/perl_pm'};
use strict;
my $svn_info = `svn info  --xml`;
my ($svnprop, $svn_num);
 if ($svn_info =~ /^\<commit\n\s+revision\=\"(\d+)\"\>/m) {
    $svn_num = $1;
  }
my $svn_version = $svn_num;
#my $svn_version=readpipe("svn info|grep 'Last Changed Rev' | sed 's/.*: //'");
my $nv_string;
# Here to skip the front 24 character and get the next 14 character
my $software_version_file = './version/version_software_ap.h';

if(!$svn_version)
{
	$svn_version="0000"
}
print $svn_version;
$svn_version=~s/[\r\n]//g;

my $str = $svn_version;

$nv_string = $nv_string . '#define AP_SW_VERSION "';
$nv_string = $nv_string . $str;
$nv_string = $nv_string .  "\"\n"; 

print $nv_string;
open (my $inf,'>', "./version/version_software_ap.h") || die ("./version/version_software_ap.h");
print $inf $nv_string;
close $inf;


