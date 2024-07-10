#!/usr/bin/perl -w
BEGIN{push @INC,'./make/perl_script/perl_pm'};
use strict;

#################################################################################
#program starts here,
#para 1: project name, such as "8801h_h5"
#para 2: name of module, such as "m = chip_drv"
#para 3: distribute parameter, such as "ib = 1"
#output: @File_Array
#       {   ib
#           project name
#           module name
#       }
#################################################################################
#my $prj = shift(@ARGV);
#my $tmp = "_builddir";
#my $output_clearcase_filelist = "build/$prj$tmp/tmp/MS_Code_correct_pathname\.txt";
#
#if(!-s $output_clearcase_filelist)
#{
#    system("dir /A:-D /B /S *.h *.c > $output_clearcase_filelist");
#    print "$output_clearcase_filelist created\n";
#}


my $str = "0.1728.2.11.20"; 
my $hex; 
 
my $len; 
my $start = 0; 
my $index = 0; 
my $nv_string;


my $software_version_file = './version/version_software_mocor.h';
open my $info, $software_version_file or die "Could not open $software_version_file: $!";

#while( my $line = <$info>)  {   
#    print $line;    
#    last if $. == 2;
#}
my $line = <$info>;

# Here to skip the front 24 character and get the next 14 character
$str = substr $line, 24, 14;

#print $str;

close $info;

$hex = unpack('H*', "$str"); 
 
$len = length($hex); 


$nv_string = "";

while ($start < $len) { 

if (substr($hex,$start,2) eq "22") {
    last;
}

$nv_string = $nv_string . 'NV_VARIANT_CONFIG_OTHER\\SOFTWARE_VERSION\\SOFTWARE_VERSION[';
$nv_string = $nv_string .  $index;
$nv_string = $nv_string .  "]=0x";
$nv_string = $nv_string .  substr($hex,$start,2); 
$nv_string = $nv_string .  "\n"; 

$start += 2; 
$index += 1;
} 

while ($index < 32) { 
$nv_string = $nv_string .  "NV_VARIANT_CONFIG_OTHER\\SOFTWARE_VERSION\\SOFTWARE_VERSION[";
$nv_string = $nv_string .  $index;
$nv_string = $nv_string .  "]=0x00";
$nv_string = $nv_string .  "\n"; 

$index += 1;
} 

#print $nv_string;
open (my $inf,'>', "./make/nv_parameter/nv_parameter_ms/config_info_sw_version.nv") || die ("./make/nv_parameter/nv_parameter_ms/config_info_sw_version.nv");
print $inf $nv_string;
close $inf;