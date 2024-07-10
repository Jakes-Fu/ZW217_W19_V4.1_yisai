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
my $prj = shift(@ARGV);
my $tmp = "_builddir";
my $output_clearcase_filelist = "build/$prj$tmp/tmp/MS_Code_correct_pathname\.txt";

if(!-s $output_clearcase_filelist)
{
    system("dir /A:-D /B /S *.h *.c > $output_clearcase_filelist");
    print "$output_clearcase_filelist created\n";
}