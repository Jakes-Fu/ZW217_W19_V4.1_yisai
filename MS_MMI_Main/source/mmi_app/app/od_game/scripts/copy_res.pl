#!/usr/bin/perl
BEGIN{push @INC,'./make/perl_script/perl_pm'};
use File::Copy;

(my $res, my $lcd_size, my $chip_ver) = @ARGV;
my $root_dir = `cd`;
chomp $root_dir;
my $ms_mmi_dir = "";
if($chip_ver=~ /7701/)
{
	$ms_mmi_dir = "MS_MMI_Main";
}
else
{
	$ms_mmi_dir = "MS_MMI";
}
my $res_dir = "\.\\".$ms_mmi_dir."\\source\\resource\\${res}";
my $od_des_res = $res_dir."\\od_game";
my $od_img_res = $od_des_res."\\MMI_RES_DEFAULT\\IMAG";
my $od_src_res = ".\\".$ms_mmi_dir."\\source\\mmi_app\\app\\od_game\\resource\\mmi_res_$lcd_size";
my $res_common_dir = "\.\\".$ms_mmi_dir."\\source\\resource\\common\\RING";
my $od_ring_res = ".\\".$ms_mmi_dir."\\source\\mmi_app\\app\\od_game\\resource\\mmi_res_ring";
#copy the res 
print "rd /s/q $od_des_res\n";
#system "pause";

if(-e $od_des_res)
{
	system "rd /s/q $od_des_res";
	#system "pause";
}
print "xcopy /S/Q/Y $od_src_res $res_dir \n";
system "xcopy /S/Q/Y $od_src_res $res_dir";

print "coping the res file $res_common_dir...\n";
system "xcopy /S/Q/Y  $od_ring_res $res_common_dir";

