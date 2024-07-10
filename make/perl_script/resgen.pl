#!/usr/bin/perl

#########################################################################
## 
## File Name:   resgen.pl
## Author:      Hongliang.xin
## Description: generate MMI resource bin file.
## Usage:       resgen.pl path_xrp_def.h path_res.bin [path_perl_cmd] [-L]\
##                        [-rep [true|false]] [-langsplit [true|false]]
## 
## Modification History
## 2012-03-20   hongliang.xin
##              Add new parameter -alu [true|false] -crm [true|false]
## 2011-06-10   hongliang.xin
##              Add new parameter -langsplit [true|false]
## 2011-05-24   hongliang.xin
##              Add new parameter -rep [true|false]
## 2010-11-19   hongliang.xin
##              Add RES_ADD_XXX_RANGE and RES_ADD_XXX_TAG_RANGE
## 2010-10-15   hongliang.xin
##              replace mkpath with mkdir
## 2010-09-08   hongliang.xin
##              Parse RES_ADD_LOGO and RES_ADD_XXX_TAG functions.
## 2010-08-25   hongliang.xin
##              delelete all bin file if ResGen failed.
## 2010-06-23   hongliang.xin 
##              deal with "RES_SET_MAX_RING_SIZE"
## 2010-06-04   hongliang.xin  
##              Add new parameter '-L' to only generate little endian 
##              resource bin file.
#########################################################################

BEGIN{push @INC,'./make/perl_script/perl_pm'};

use File::Copy;
use File::Find;
use File::DosGlob 'glob';
use File::Path;

if(scalar(@ARGV)<3)
{
	die "[resgen.pl] param error, number of params at least is 3\n";
}

my $xrp_def_path = ${ARGV[0]};
my $bin_file_path = ${ARGV[1]};
my $perl_cmd_path = ${ARGV[2]};
my $ResGen_New = ${ARGV[3]};
my $only_little = 0;
my $rep_flag = "true";
my $lang_split = "false";
my $parm_temp = "";
my $res_dir = "";
my $copy_flag = 1;
my $all_unicode="true";
my $cross_remove_str="false";
my $gen_csc_str="false";
my $gen_font2csc_str="true";


my $pi = 0;
for($pi = 0; $pi<scalar(@ARGV); $pi++)
{
	print "${ARGV[$pi]} ";
} 
print "\narg num:".scalar(@ARGV)."\n";
for($pi = 4; $pi<scalar(@ARGV); $pi++)
{
    $parm_temp = ${ARGV[$pi]};    
    if(lc($parm_temp) eq "-l")
	{
		$only_little = 1;	
	}
	elsif(lc($parm_temp) eq "-rep")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		    $rep_flag = ${ARGV[$pi+1]};
		    $pi++;
		}
		else
		{
		    die "[resgen.pl] param error, -rep [true|false].\n";
		}
	}
	elsif(lc($parm_temp) eq "-langsplit")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		    $lang_split = ${ARGV[$pi+1]};
		    $pi++;
		}
		else
		{
		    die "[resgen.pl] param error, -langsplit [true|false].\n";
		}
	}
	elsif(lc($parm_temp) eq "-resdir")
	{
		if(($pi+1) < scalar(@ARGV))
		{
			$copy_flag=0;
		    $res_dir = ${ARGV[$pi+1]};
		    $res_dir =~ s/\\+/\//g;
		    $pi++;
		}
		else
		{
		    die "[resgen.pl] param error, -langsplit [true|false].\n";
		}
	}
	elsif(lc($parm_temp) eq "-alu")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		    $all_unicode = ${ARGV[$pi+1]};
		    $pi++;
		}
		else
		{
		    die "[resgen.pl] param error, -alu [true|false].\n";
		}
	}
	elsif(lc($parm_temp) eq "-crs")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		    $cross_remove_str = ${ARGV[$pi+1]};
		    $pi++;
		}
		else
		{
		    die "[resgen.pl] param error, -crs [true|false].\n";
		}
	}
	elsif(lc($parm_temp) eq "-gencsc")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		    $gen_csc_str = ${ARGV[$pi+1]};
		    $pi++;
		}
		else
		{
		    die "[resgen.pl] param error, -gencsc [true|false].\n";
		}
	}
	elsif(lc($parm_temp) eq "-font2csc")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		    $gen_font2csc_str = ${ARGV[$pi+1]};
		    $pi++;
		}
		else
		{
		    die "[resgen.pl] param error, -font2csc [true|false].\n";
		}
	}
}

my $xrp_def_path_bak = $xrp_def_path;

$xrp_def_path =~ s/\\+/\//g;

my $xrp_pl_path = $xrp_def_path;

my @module_path;
my @font_lines;
my @ring_lines;
my $font_def_rel_path;

my @skins = ("DEFAULT");
my @langs;
my $keycolor = "0x000000";
my @img_cmp_default;

my $img_type;
my $cmp_type;
my $cmp_para;

$xrp_pl_path =~ s/^(.*).h$/$1.pl/g;



open(DEF, "<$xrp_def_path") or die "Can not open $xrp_def_path" ;
open(PL, ">$xrp_pl_path") or die "Can not create and open $xrp_pl_path" ;

#######################################################
## read mmi_res_prj_def.h file and tidy each line to the new file mmi_res_prj_def.pl
##
## mmi_res_prj_def.pl
## #!/usr/bin/perl
## require "xrp_temp.pl"
## START_GEN_XRP(\"$xrp_def_path\");

print PL "#!/usr/bin/perl\r\n\r\n";
print PL "BEGIN{push \@INC,'./make/perl_script'};\r\n\r\n";
print PL "BEGIN{push \@INC,'./make/perl_script/perl_pm'};\r\n\r\n";
print PL "require \"xrp_temp.pl\";\r\n\r\n";
print PL "START_GEN_XRP(\"$xrp_def_path\",\"$res_dir\");\r\n";
print PL "\r\n";

#######################################################
## tidy each line
## each paramer will convert to string type by add ""
while($line = <DEF>)
{
	##remove space char
	##$line =~ s/[ 	]*//g;
	if($line =~m/^\s*\#.*/)
	{
		next;
	}
	elsif($line =~m/^\s*MMI_.*/)
	{#MMI use this macro, script omit it.		
		next;
	}
	elsif($line =~s/RES_ADD_LANGUAGE\s*\(\s*([^, ]*)\s*,\s*([^, ]*)\s*\)/RES_ADD_LANGUAGE\(\"$1\",$2\);/g )
	{
		print PL $line;
		
		## save langugage information to an array @langs
		$lang = $2;
		$lang =~s/\"([^,\"]*)\"/$1/g; ##get language name
		$lang =~s/[\r\n]*//g; ## remove \r\n chars
		push @langs,$lang; 
	}
	elsif($line =~s/RES_SET_VERSION\s*\(\s*([^, ]*)\s*\)/RES_SET_VERSION\($1\);/g )
	{##add ";" to tail
		print PL $line;
	}
	elsif($line =~s/RES_SET_MAX_MODULE_NUM\s*\(\s*([^, ]*)\s*\)/RES_SET_MAX_MODULE_NUM\(\"$1\"\);/g )
	{## add "" to the parameter and add ";" to tail
		print PL $line;
	}
	elsif($line =~s/RES_SET_IMG_GLOBAL_COMPRESS_TYPE\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_SET_IMG_GLOBAL_COMPRESS_TYPE\(\"$1\",\"$2\"\);/g )
	{## add "" to the all parameters and add ";" to tail
		print PL $line;
	}
	elsif($line =~s/RES_SET_ANIM_GLOBAL_COMPRESS_TYPE\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_SET_ANIM_GLOBAL_COMPRESS_TYPE\(\"$1\",\"$2\"\);/g )
	{## add "" to the all parameters and add ";" to tail
		print PL $line;
	}
	elsif($line =~s/RES_SET_IMG_DEFAULT_COMPRESS_TYPE\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_SET_IMG_DEFAULT_COMPRESS_TYPE\(\"$1\",\"$2\"\);/g )
	{## add "" to the all parameters
		print PL $line;
	}
	elsif($line =~s/RES_SET_ANIM_DEFAULT_COMPRESS_TYPE\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_SET_ANIM_DEFAULT_COMPRESS_TYPE\(\"$1\",\"$2\"\);/g )
	{## add "" to the all parameters and add ";" to tail
		print PL $line;
	}
	elsif($line =~s/RES_SET_ANIM_GLOBAL_KEY_COLOR\s*\(\s*([^, ]*)\s*\)/RES_SET_ANIM_GLOBAL_KEY_COLOR\(\"$1\"\);/g )
	{## add "" to the parameter and add ";" to tail
		print PL $line;
		
		## save key color information
		$color = $line;
		$color =~s/RES_SET_ANIM_GLOBAL_KEY_COLOR\(\"([^, \"]*)\"\);/$1/g; ##get color value
		$color =~s/[\r\n]*//g; ## remove \r\n chars
		$keycolor = $color;	 
	}
	elsif($line =~s/RES_SET_GLOBAL_JPEG_COMPRESS_RATE\s*\(\s*([^, ]*)\s*\)/RES_SET_GLOBAL_JPEG_COMPRESS_RATE\(\"$1\"\);/g )
	{## add "" to the parameter and add ";" to tail
		print PL $line;
	}
	elsif($line =~s/RES_SET_IMG_CMP_DEFAULT\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*\)/RES_SET_IMG_CMP_DEFAULT\(\"$1\",\"$2\",\"$3\"\);/g )
	{## add "" to the parameter and add ";" to tail	
=head		
		$img_type = $line;
		$img_type =~s/RES_SET_IMG_CMP_DEFAULT\(\"([^, ]*)\",\"([^,]*)\",\"([^,]*)\"\)/$1/g ;
		$img_type =~s/[ ]*//g; ## remove space chars
		push @img_cmp_default $img_type;
		
		$cmp_type = $line;
		$cmp_type =~s/RES_SET_IMG_CMP_DEFAULT\(\"([^, ]*)\",\"([^,]*)\",\"([^,]*)\"\)/$2/g ;
		$cmp_type =~s/[ ]*//g; ## remove space chars
		push @img_cmp_default $cmp_type;
		
		$cmp_para = $line;
		$cmp_para =~s/RES_SET_IMG_CMP_DEFAULT\(\"([^, ]*)\",\"([^,]*)\",\"([^,]*)\"\)/$3/g ;
		$cmp_para =~s/[ \r\n]*//g; ## remove space chars
		push @img_cmp_default $cmp_para;
=cut		
		$img_type = $1;
		$cmp_type = $2;
		$cmp_para = $3;		
		
		$img_type =~s/[ ]*//g; ## remove space chars
		push @img_cmp_default,$img_type;
		
		$cmp_type =~s/[ ]*//g; ## remove space chars
		push @img_cmp_default,$cmp_type;
		
		$cmp_para =~s/[ \r\n]*//g; ## remove space chars
		push @img_cmp_default,$cmp_para;
		
		
	}
	elsif($line =~s/RES_SET_MAX_RING_SIZE\s*\(\s*([^, ]*)\s*\)/RES_SET_MAX_RING_SIZE\(\"$1\"\);/g )
	{## add "" to the parameter and add ";" to tail
		print PL $line;
	}
	elsif($line =~s/RES_ADD_SKIN\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*\)/RES_ADD_SKIN\($1,$2,\"$3\"\);/g )
	{## add "" to the third parameter [id] and add ";" to tail
	
		print PL $line;
		
		## save skin information to an array @skins
		$skin = $line;
		$skin =~s/RES_ADD_SKIN\(\"([^, \"]*)\",([^,]*),([^,]*)\);/$1/g; ##get skin name 
		$skin =~s/[\r\n]*//g; ## remove \r\n chars
		push @skins,$skin; 
	}
	elsif($line =~s/RES_ADD_MODULE\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_MODULE\(\"$1\",$2\);/g )
	{## add "" to the first parameter [id] and add ";" to tail
	
		my $path = $line;
		$path =~ s/RES_ADD_MODULE\(([^, ]*),\"([^,\"]*)\"\);/$2/g ;
		$path =~ s/[ \r\n]*//g;	
		
		## FONT is expecail module name, and its path must be redefined
		if( lc($path) eq "font" )
		{
			$path = "\\\\FONT\\\\font_mdu_def.h"; #"\\FONT\\font_mdu_def.h"
			$font_def_rel_path = $path;			
			$line =~s/RES_ADD_MODULE\(([^, ]*),([^,]*)\);/RES_ADD_MODULE\($1,\"$path\"\);/g ;
		}
		
		print PL $line;
		push @module_path, $path;
	}
	## rings are sprecial,must add to the first module define file. So save them first,deal with them later.
	elsif($line =~s/RES_ADD_RING\s*\(\s*([^, ]*)\s*,\s*\"([^,\"]*)\"\s*\)/RES_ADD_RING\($1,\"\\\\MMI_RES_##$2\"\)/g )
	{
		push @ring_lines,$line;
	}
	elsif($line =~s/RES_ADD_RING_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*\"([^,\"]*)\"\s*\)/RES_ADD_RING_RANGE\($1,$2,\"\\\\MMI_RES_##$3\"\)/g )
	{
		push @ring_lines,$line;
	}	
	## fonts are sprecial,must create a module define file, so save them first and create the file later.
	elsif($line =~s/RES_ADD_FONT\s*\(\s*([^, ]*)\s*,\s*\"([^,\"]*)\"\s*,\s*\"([^,\"]*)\"\s*\)/RES_ADD_FONT\($1,\"\\\\MMI_RES_##$2\",\"\\\\MMI_RES_##$3\"\)/g )
	{
		if(!$3)
		{
			$line =~s/RES_ADD_FONT\(([^, ]*),\"([^,\"]*)\",\"([^,\"]*)\"\)/RES_ADD_FONT\($1,\"$2\",\"\"\)/g 
		}
		push @font_lines,$line;
	}
	elsif($line =~s/RES_ADD_FONT_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*\"([^,\"]*)\"\s*,\s*\"([^,\"]*)\"\s*\)/RES_ADD_FONT_RANGE\($1,$2,\"\\\\MMI_RES_##$3\",\"\\\\MMI_RES_##$4\"\)/g )
	{
		if(!$4)
		{
			$line =~s/RES_ADD_FONT_RANGE\(([^, ]*),(\d+),\"([^,\"]*)\",\"([^,\"]*)\"\)/RES_ADD_FONT_RANGE\($1,$2,\"$3\",\"\"\)/g 
		}
		push @font_lines,$line;
	}
	elsif($line =~s/RES_ADD_LOGO\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^, ]*)\s*\)/RES_ADD_LOGO\(\"$1\",$2,\"$3\",\"$4\"\);/g )
	{## 4 params id,path,cmp_type,cmp_param
		print PL $line;
	}
	elsif($line =~s/RES_ADD_RING_TAG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_RING_TAG\($1,$2\)/g )
	{
		push @ring_lines,$line;
	}
	elsif($line =~s/RES_ADD_RING_TAG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_RING_TAG_RANGE\($1,$2,$3\)/g )
	{
		push @ring_lines,$line;
	}
	elsif($line =~s/RES_ADD_FONT_TAG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_FONT_TAG\($1,$2\)/g )
	{
		push @font_lines,$line;
	}
	elsif($line =~s/RES_ADD_FONT_TAG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_FONT_TAG_RANGE\($1,$2,$3\)/g )
	{
		push @font_lines,$line;
	}
	else
	{
		$line =~ s/[ \r\n\t]*//g;
		## the line no match above, maybe there is some syntax error.
		if($line)
		{
			close PL;
			close DEF;
			print $line."\n";
			die  "there is syntax error";
		}
	}
}


print PL "\r\n\r\n";
print PL "END_GEN_XRP();\r\n";

close PL;
close DEF;

my $path_base = $xrp_def_path;
$path_base =~ s/(.*)\/[^\/]*.h/$1/g;


#######################################################
## merge the ring lines to first module define file
my $first_module_path = $path_base;
$first_module_path .= $module_path[0];
$first_module_path =~ s/\\\\/\//g;

my @lines = undef;
open (FIRST,"<$first_module_path") or die "Can not open [$first_module_path]\r\n";
## backup them to an array if they are not RES_ADD_RING
## the purpers is able to execute this script many times, and must delete the addings.
while($line = <FIRST>)
{
	if($line !~ s/RES_ADD_RING/RES_ADD_RING/g)
	{
		push @lines, $line;
	}
}
close FIRST;

open (FIRST,">$first_module_path") or die "Can not open [$first_module_path]\r\n";
foreach $line (@lines){
	print FIRST $line;
}

foreach $ring_line (@ring_lines){
	print FIRST $ring_line;
}
close FIRST;

## release memory
@lines=();
@ring_lines=();

##move ring files to first moule resource dir
my $src_ring_path_base = $path_base . "/RING";
my $src_ring_path = $src_ring_path_base;
my $dst_ring_path_base = $first_module_path;
$dst_ring_path_base =~ s/(.*)\/[^ \/]*/$1/g;
my $dst_ring_path = $dst_ring_path_base;

if($copy_flag == 1 && -d $src_ring_path_base)
{	
	my $ri = 0;
	for($ri = 0; $ri<scalar(@skins); $ri++)
	{
		$dst_ring_path = $dst_ring_path_base;
		$dst_ring_path .= "/MMI_RES_$skins[$ri]";
		if (!(-d $dst_ring_path))
		{
			mkpath $dst_ring_path or die "can not create folder [$dst_ring_path]";
		}
		$dst_ring_path .= "/RING";
		if (!(-d $dst_ring_path))
		{
			mkpath $dst_ring_path or die "can not create folder [$dst_ring_path]";
		}
		
		$src_ring_path = $src_ring_path_base;
		if($ri > 0)
		{
			$src_ring_path .= "/$skins[$ri]";			
		}
		
		if(-d $src_ring_path)
		{
			sub copyRing{
				my $dir = $File::Find::dir;
				my $filepath = $File::Find::name;
				my $filename = $filepath;
				my $filename =~ s/.*\/([^ \/]*\.[^ \/\.]*)\z/$1/g;	
				if(($dir eq $src_ring_path) && ($filepath gt $dir ) && (-f $filepath))
				{		
					copy($filepath, $dst_ring_path."/".$filename) or die "can not copy file [$filepath] [$dst_ring_path]\n";		
				}	
			}
					
			find( \&copyRing,$src_ring_path);
		}
			
	}
	print "-----copy rings complete-----\n";
}


#######################################################

#######################################################
## create and write font module define file 
if($copy_flag == 1 )
{
	my $dst_font_path = $path_base;
	my $src_font_path = $path_base . "/FONT";
	if($font_def_rel_path)
	{
		my $font_def_abs_path = $path_base;
		$font_def_abs_path .= $font_def_rel_path;
		$font_def_abs_path =~ s/\\+/\//g;
		
		open (FONT,">$font_def_abs_path") or die "Can not open [$font_def_abs_path]\r\n";
		foreach $font_line (@font_lines){
			print FONT $font_line;
		}
		close FONT;
		my $fi= 0;
		for($fi = 0; $fi<scalar(@skins);$fi++)
		{
			$dst_font_path = $path_base;
			$dst_font_path .= "/FONT/MMI_RES_$skins[$fi]";
			if (!(-d $dst_font_path))
			{
				mkpath $dst_font_path or die "can not create folder [$dst_font_path]";
			}
			$dst_font_path .= "/FONT";
			if (!(-d $dst_font_path))
			{
				mkpath $dst_font_path or die "can not create folder [$dst_font_path]";
			}
			
			$src_font_path = $path_base . "/FONT";
			if($fi > 0)
			{
				$src_font_path .= "/$skins[$fi]";
			}
			
			if(-d $src_font_path)
			{
				sub copyFont{
					my $dir = $File::Find::dir;
					my $filepath = $File::Find::name;
					my $filename = $filepath;
					$filename =~ s/.*\/([^\/]*\.lib)\z/$1/g;		
					if(($dir eq $src_font_path)  && ( -f $filepath ) && ($filepath =~ m/\.lib\z/))
					{		
						copy($filepath, $dst_font_path."/".$filename) or die "can not copy file [$filepath]\n";						
					}
					
				#	print "$File::Find::name\n";
				}
				find( \&copyFont,$src_font_path);
			}
		
		}
		
		print "-----copy fonts complete-----\n";
	}
	else
	{
		die "-----not found font-----\n";
	}
}
else
{
	my $font_mdu_path = $path_base . "/FONT";
	if (!(-d $font_mdu_path))
	{
		mkpath $font_mdu_path or die "can not create folder [$font_mdu_path]";
	}
	if($font_def_rel_path)
	{
		my $font_def_abs_path = $path_base;
		$font_def_abs_path .= $font_def_rel_path;
		$font_def_abs_path =~ s/\\+/\//g;
		
		
		open (FONT,">$font_def_abs_path") or die "Can not open [$font_def_abs_path]\r\n";
		foreach $font_line (@font_lines){
			print FONT $font_line;
		}
		close FONT;
	}
}
## release memory
@font_lines = ();

#######################################################

print "\r\n---------------------------------------------\r\n";

my $ret = system "\"$perl_cmd_path\" \"$xrp_pl_path\"";

if($ret != 0)
{
	die "Generate xrp file [$xrp_def_path] failed\n";
}

print "---------------------------------------------\r\n";



my $skin_str;
for($i = 0; $i<scalar(@skins);$i++)
{
	$skin_str .= $skins[$i];
	if($i != (scalar(@skins) - 1))
	{
		$skin_str .= ",";
	}
}

my $lang_str;
for($i = 0; $i<scalar(@langs);$i++)
{
	$lang_str .= $langs[$i];
	if($i != (scalar(@langs) - 1))
	{
		$lang_str .= ",";
	}
}

my $img_cmp_str;
for($i = 0; $i<scalar(@img_cmp_default);$i++)
{
	$img_cmp_str .= $img_cmp_default[$i];
	if($i != (scalar(@img_cmp_default) - 1))
	{
		$img_cmp_str .= ",";
	}
}
######################################
##create perl file for extract sub string table
=head
my $str_pl_path = $xrp_def_path_bak;
$str_pl_path =~ s/^(.*)\/[^\/]+$/$1\/str_table.pl/g;
#$str_pl_path =~ s/[\/]+/\\\\/g;

my $str_table_path = $xrp_def_path_bak;
$str_table_path =~ s/^(.*)\/[^\/]+$/$1\/str_table.xls/g;
#$str_table_path =~ s/[\/]+/\\\\/g;

if(-e $str_pl_path)
{
	unlink $str_pl_path;
}

open(STR_PL, ">$str_pl_path") or die "Can not create $str_pl_path" ;

print STR_PL "#!/usr/bin/perl\r\n\r\n";
print STR_PL "BEGIN{push \@INC,'./make/perl_script'};\r\n\r\n";
print STR_PL "BEGIN{push \@INC,'./make/perl_script/perl_pm'};\r\n\r\n";
print STR_PL "require \"str_temp.pl\";\r\n\r\n";

print STR_PL "print \"-----------------nnnnn-------------\";\r\n";

print STR_PL "STRART_EXTRACT_SUB_TABLE(\"$str_table_path\",\"$lang_str\");\r\n";
print STR_PL "\r\n";

close STR_PL;
=cut
######################################

foreach $mdu_path (@module_path){
	
	my $mdu_abs_path = $path_base;
	$mdu_abs_path .= $mdu_path;
	$mdu_abs_path =~ s/\\+/\//g;
	my $mdu_res_path = $mdu_abs_path;
	if($copy_flag==0)
	{
		$mdu_res_path = $res_dir.$mdu_path;
		$mdu_res_path =~ s/\\+/\//g;
	}
	$mdu_res_path =~ s/(.*)\/[^\/]*.h/$1/g;
	
	my $ret = system "\"$perl_cmd_path\" ./make/perl_script/tidy_xrm.pl \"$mdu_abs_path\" \"$lang_str\" \"$skin_str\" \"$keycolor\" \"$img_cmp_str\" \"$perl_cmd_path\" \"$mdu_res_path\" ";
	if($ret != 0)
	{
		die "Generate xrm file [$mdu_abs_path] failed\n";
	}
	print "---------------------------------------------\r\n";
	
}

=head
open(STR_PL, ">>$str_pl_path") or die "Can not open $str_pl_path" ;
print STR_PL "\r\nEND_EXTRACT_SUB_TABLE();\r\n";
print STR_PL "1;\r\n";
close STR_PL;
=cut


# using this to parse excel will be very slow
#my $ret = system "\"$perl_cmd_path\" \"$str_pl_path\"";
#if($ret != 0)
#{
#	die "Generate text file failed\n";
#}


my $bin_file_all = $bin_file_path;
my $bin_file_arm = $bin_file_path;
my $bin_file_win = $bin_file_path;
$bin_file_win =~ s/^(.*).bin$/$1_win.bin/g;
$bin_file_all =~ s/^(.*).bin$/$1*.bin/g;

my $xrp_path = $xrp_def_path_bak;
$xrp_path =~ s/^(.*)_prj_def.h$/$1.xrp/g;
$xrp_path =~ s/[\/]+/\\\\/g;
#\\test to \\\\test for net dir
$xrp_path =~ s/^\\\\/\\\\\\\\/g;
$bin_file_path =~ s/[\/]+/\\\\/g;
#\\test to \\\\test for net dir
$bin_file_path =~ s/^\\\\/\\\\\\\\/g;


#print "---------------------------------------------\r\n";

if($^O=~/MSWin32/)
{
    print " MSWin32 \r\n";
}
else
{
    print " not MSWin32 \r\n";
    $bin_file_path =~ s/\\\\\\\\/\//g;
    $xrp_path =~ s/\\\\\\\\/\//g;
    $bin_file_path =~ s/\\\\/\//g;
    $xrp_path =~ s/\\\\/\//g;
}
if($ResGen_New == 1)
{
	print " use new resgen \r\n";
	print "./make/perl_script/ResGen_New/resgen.py -xrp \"$xrp_path\" -bin \"$bin_file_path\" -rep \"$rep_flag\" -langsplit \"$lang_split\" -alu \"$all_unicode\" -crs \"$cross_remove_str\" -gencsc \"$gen_csc_str\" -font2csc \"$gen_font2csc_str\"\n";
	my $ret = system ("python ./make/perl_script/ResGen_New/resgen.py -xrp \"$xrp_path\" -bin \"$bin_file_path\" -rep \"$rep_flag\" -langsplit \"$lang_split\" -alu \"$all_unicode\" -crs \"$cross_remove_str\" -gencsc \"$gen_csc_str\" -font2csc \"$gen_font2csc_str\"");
}
else
{
	print "use old resgen\r\n";
	print "./make/perl_script/ResGen/ResCmd.exe -xrp \"$xrp_path\" -bin \"$bin_file_path\" -rep \"$rep_flag\" -langsplit \"$lang_split\" -alu \"$all_unicode\" -crs \"$cross_remove_str\" -gencsc \"$gen_csc_str\" -font2csc \"$gen_font2csc_str\"\n";
	my $ret = system "./make/perl_script/ResGen/ResCmd.exe -xrp \"$xrp_path\" -bin \"$bin_file_path\" -rep \"$rep_flag\" -langsplit \"$lang_split\" -alu \"$all_unicode\" -crs \"$cross_remove_str\" -gencsc \"$gen_csc_str\" -font2csc \"$gen_font2csc_str\"";
}

if($ret != 0)
{
	if(-e $bin_file_arm)
	{
		unlink $bin_file_arm;	
	}
	
	if(-e $bin_file_win)
	{
		unlink $bin_file_win;	
	}	
	
	unlink glob $bin_file_all;
	
	die "Generate resource file failed\n";
}
else
{
	# judge the resource bin size
	if((-e $bin_file_arm) && (-s $bin_file_arm )< 51200 )
	{
		unlink $bin_file_arm;	
		if(-e $bin_file_win )
		{
			unlink $bin_file_win;			
		}
		unlink glob $bin_file_all;
	}
		
	if((-e $bin_file_win ) && (-s $bin_file_win )< 51200 )
	{
		unlink $bin_file_win;
		if(-e $bin_file_arm )
		{
			unlink $bin_file_arm;
		}
		unlink glob $bin_file_all;	
	}
	
	if( !(-e $bin_file_arm) || !(-e $bin_file_win))
	{
		unlink glob $bin_file_all;
	}
}

if($only_little == 1)
{
	if(-e $bin_file_arm)
	{
		unlink $bin_file_arm;	
	}
	
	if(-e $bin_file_win)
	{		
		rename($bin_file_win, $bin_file_arm);	
	}
}
print "---------------------------------------------\r\n";

1;