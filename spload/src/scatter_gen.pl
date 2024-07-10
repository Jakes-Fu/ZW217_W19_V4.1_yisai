#!/usr/bin/perl -w

my $NUM_ARGV = @ARGV;
my $main_scatter_path;
my $old_scatter_file = "spload/src/spload_scatter.scf";
my $new_scatter_file = "spload/src/spload_scatter_gen.scf";
my $old_lcm_main_file = "spload/src/spload_main.c";
my $new_lcm_main_file = "spload/src/spload_main_gen.c";
my $cfg_file = "MS_Customize/export/inc/spload_cfg.h";
my $lcd_bin_load_addr;
my $lcd_compatible_string1 = "LCM_SENSOR_LOAD_BASE_ADDR";
my $lcd_compatible_string2 = "ROM_LOAD";
my $lcd_compatible_string3 = "ROM_EXEC";
my $lcd_compatible_string4 = "ROM_EXEC2";
my $lcd_compatible_string5 = "RAM_RW";
my $lcd_compatible_string6 = "RAM_ZI";
my $lcd_compatible_string7 = "LCM_SENSOR_EXT_STACK";
my $lcd_compatible_string8 = "LCM_SENSOR_EXT_TABLE";
my $rom_exec1_size;
my $rom_exec2_base_addr;
my $lcd_ext_stack_addr;
my $lcd_ext_stack_size;
my $lcd_ram_rw_address;
my $lcd_ram_rw_size;
my $lcd_ram_zi_address;
my $lcd_ram_zi_size;
my $lcd_main_ext_func_table_offset;
my $myline;
my $outstring = "";



#verify argument and get platform scatter file path nad file name!!!
if($NUM_ARGV == 1)
{
	$main_scatter_path  = $ARGV[0]; ##main scatter file name!!!
}
else
{
	die("argument num failed!!!");
}

#open related file
open (FH,  "<$cfg_file") || die ("failed to open spload_cfg.h!!!\n\n");
open (FH1, "<$old_scatter_file") || die ("failed to open spload_scatter.scf\n\n");
open (FH2, ">$new_scatter_file") || die ("failed to open spload_scatter_gen.scf\n\n");
open (FH3, "<$main_scatter_path")|| die ("failed to open main_platform.scf\n\n");
open (FH4, "<$old_lcm_main_file")|| die ("failed to open spload_main.c\n\n");
open (FH5, ">$new_lcm_main_file")|| die ("failed to open spload_main_gen.c\n\n");

#get lcd bin load address from sperate_load_cfg.h
my @alllines = <FH>;
close (FH);
foreach $myline (@alllines)
{
	chomp($myline);
	if($myline=~/\s*($lcd_compatible_string1)\s*(\w+)/)
	{
		$lcd_bin_load_addr = $2;
	}
} 

##get lcd sram ext stack address from $platform_$ramsize.scf!!!
@alllines = <FH3>;
close (FH3);
foreach $myline (@alllines)
{
	chomp($myline);
	if($myline=~/\s*($lcd_compatible_string7)\s*(\w+)\s*(\w+)/)
	{
		$lcd_ext_stack_addr = $2;
		$lcd_ext_stack_size = $3;
	}
	if($myline=~/\s*($lcd_compatible_string8)\s*(\w+)/)
	{
		$lcd_main_ext_func_table_offset = $2;
	}
} 

##generate new spload_main_gen.c file according to platform scatter file and spload_main.c!!!
@alllines = <FH4>;
close (FH4);
$outstring = "";
foreach $myline (@alllines)
{
	chomp($myline);
	$_ = $myline;
	if($myline=~/(\w+)(\s+)(\w+)(;)/)
	{
		s/(\w+)(\s+)(\w+)(;)/$1 $lcd_main_ext_func_table_offset$4/;
		$outstring = $outstring.$_."\n";
	}
	else
	{
		$outstring = $outstring.$myline."\n";
	}
}
print FH5 $outstring;
close (FH5);

##delete old sperate_load_scatter.scf and rename sperate_load_scatter_gen.scf to sperate_load_main.c!!!
if(-e $new_lcm_main_file)
{
	unlink($old_lcm_main_file)||die ($!);
	rename($new_lcm_main_file, $old_lcm_main_file)||die ($!);
}
else
{
	die("generete new scatter file failed!!!");
}

##generate new sperate_load_scatter_gen.scf file according to platform scatter file and sperate_load_cfg.h!!!
@alllines = <FH1>;
close (FH1);
$outstring = "";
foreach $myline (@alllines)
{
	chomp($myline);
	$_ = $myline;
	
	if($myline=~/\s*;/)
	{
		$outstring = $outstring.$myline."\n";
	}
	elsif($myline=~/\b($lcd_compatible_string2)\b\s*(\w+)/)
	{
		s/\b$lcd_compatible_string2\b\s*($2)/$lcd_compatible_string2 $lcd_bin_load_addr/;
		$outstring = $outstring.$_."\n";
	}
	elsif($myline=~/\b($lcd_compatible_string3)\b\s*(\w+)\s*(\w+)/)
	{
		$rom_exec1_size = $3;
		s/$lcd_compatible_string3\s*($2)\s*($3)/$lcd_compatible_string3 $lcd_bin_load_addr 0x200/;
		$outstring = $outstring.$_."\n";
	}
	elsif($myline=~/\b($lcd_compatible_string4)\b\s*(\w+)\s*(\w+)/)
	{
		$rom_exec2_base_addr =  hex($lcd_bin_load_addr) + hex($rom_exec1_size);
		$rom_exec2_base_addr =  "0x".sprintf("%x",$rom_exec2_base_addr);
		s/$lcd_compatible_string4\s*($2)/$lcd_compatible_string4 $rom_exec2_base_addr/;
		$outstring = $outstring.$_."\n";
	}
	elsif($myline=~/\b($lcd_compatible_string5)\b\s*(\w+)\s*(\w+)/)
	{
		$lcd_ram_rw_address = $lcd_ext_stack_addr;
		$lcd_ram_rw_size = hex($lcd_ext_stack_size)*1/2;
		$lcd_ram_rw_size = "0x".sprintf("%x",$lcd_ram_rw_size);
		s/$lcd_compatible_string5\s*($2)\s*($3)/$lcd_compatible_string5 $lcd_ram_rw_address $lcd_ram_rw_size/;
		$outstring = $outstring.$_."\n";
	}
	elsif($myline=~/\b($lcd_compatible_string6)\b\s*(\w+)\s*(\w+)/)
	{
		$lcd_ram_zi_address = hex($lcd_ram_rw_address) + hex($lcd_ram_rw_size);
		$lcd_ram_zi_address = "0x".sprintf("%x",$lcd_ram_zi_address);
		$lcd_ram_zi_size    = hex($lcd_ext_stack_size)*1/2;
		$lcd_ram_zi_size    = "0x".sprintf("%x",$lcd_ram_zi_size);
		s/$lcd_compatible_string6\s*($2)\s*($3)/$lcd_compatible_string6 $lcd_ram_zi_address $lcd_ram_zi_size/;
		$outstring = $outstring.$_."\n";
	}
	else
	{
		$outstring = $outstring.$myline."\n";
	}
}

print FH2 $outstring;
close (FH2);

##delete old sperate load scatter file and rename sperate_load_scatter_gen.scf to sperate_load_scatter.scf!!!
if(-e $new_scatter_file)
{
	unlink($old_scatter_file)||die ($!);
	rename($new_scatter_file, $old_scatter_file)||die ($!);
}
else
{
	die("generete new scatter file failed!!!");
}


##show debug info
printf "*******Seperate Download Info Start********\n";
printf ("project scatter is        : $main_scatter_path \n");
printf ("spload stack addr is      : $lcd_ext_stack_addr \n"); 
printf ("spload stack size is      : $lcd_ext_stack_size \n"); 
printf ("spload bin load addr is   : $lcd_bin_load_addr \n");
printf ("spload scatter is         : $new_scatter_file \n");
printf ("spload rom_load addr is   : $lcd_bin_load_addr \n");
printf ("spload rom_exec addr is   : $lcd_bin_load_addr \n");
printf ("spload rom_exec size is   : 0x200(dedicated!!!) \n");
printf ("spload rom_exec2 addr is  : $rom_exec2_base_addr \n");
printf ("spload ram_rw address is  : $lcd_ram_rw_address \n");
printf ("spload ram_rw size is     : $lcd_ram_rw_size \n");
printf ("spload ram_zi addr is     : $lcd_ram_zi_address \n");
printf ("spload ram_size size is   : $lcd_ram_zi_size \n");
printf "******Seperate Download Info End***********\n";




