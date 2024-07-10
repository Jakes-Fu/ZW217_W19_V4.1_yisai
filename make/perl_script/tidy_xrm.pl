#!/usr/bin/perl

#########################################################################
## 
## File Name:   xrm_tmp.pl
## Author:      Hongliang.xin
## Description: export function to generate xrm file
## Usage:       
## 
## Modification History
## 2010-11-19   hongliang.xin
##              Add RES_ADD_XXX_RANGE and RES_ADD_XXX_TAG_RANGE
## 2010-09-08   hongliang.xin
##              Parse RES_ADD_XXX_TAG functions
##
#########################################################################

if(scalar(@ARGV)<5)
{
	die "[tidy_xrm.pl] param error, number of params at least is 4\n";
}

my $xrm_def_path = ${ARGV[0]};
my $langs = ${ARGV[1]};
my $skins = ${ARGV[2]};
my $keycolor = ${ARGV[3]};
my $img_cmp_str = ${ARGV[4]};


my $perl_cmd_path = "perl";
if(scalar(@ARGV)>5)
{
	$perl_cmd_path = ${ARGV[5]};	
}
my $mdu_res_dir = "";
if(scalar(@ARGV)>6)
{
	$mdu_res_dir = ${ARGV[6]};	
}

#print "langs-----------$langs\n";
#print "skins-----------$skins\n";

$xrm_def_path =~ s/\\+/\//g;

my $xrm_pl_path = $xrm_def_path;

$xrm_pl_path =~ s/^(.*).h$/$1.pl/g;


my $has_str = 1;

=head
my $has_str = 0;
open(DEF_STR, "<$xrm_def_path") or die "Can not open $xrm_def_path\n" ;
LINE: while($line = <DEF_STR>)
{
	if($line =~ m/RES_ADD_STRING/)
	{
		$has_str = 1;
		last LINE;
	}	
}
=cut

close DEF_STR;

open(DEF, "<$xrm_def_path") or die "Can not open $xrm_def_path\n" ;
open(PL, ">$xrm_pl_path") or die "Can not create and open $xrm_pl_path\n" ;

print PL "#!/usr/bin/perl\r\n\r\n";
print PL "BEGIN{push \@INC,'./make/perl_script'};\r\n\r\n";
print PL "BEGIN{push \@INC,'./make/perl_script/perl_pm'};\r\n\r\n";
print PL "require \"xrm_temp.pl\";\r\n\r\n";
print PL "START_GEN_XRM(\"$xrm_def_path\",\"$langs\", \"$skins\", \"$keycolor\",\"$img_cmp_str\",\"$mdu_res_dir\");\r\n";
print PL "\r\n";


=head	
my $text_xls_path = $xrm_def_path;
$text_xls_path =~ s/(.*)\/[^\/]*/$1\/MMI_RES_DEFAULT\/TEXT\/text.xls/g;
my $str_table_pl_path = $xrm_def_path;
$str_table_pl_path =~ s/(.*)\/[^\/]*\/[^\/]*/$1\/str_table.pl/g;

open(STR_TABLE_PL, ">>$str_table_pl_path") or die "Can not open $str_table_pl_path\n";
print STR_TABLE_PL "\r\n\r\nSTART_MODULE(\"$text_xls_path\",$has_str);\r\n";
=cut


while($line = <DEF>)
{
	#$line =~ s/[ 	]*//g;
	if($line =~m/^\s*\#.*/)
	{
		next;
	}
	elsif($line =~m/^\s*MMI_.*/)
	{#MMI use this macro, script omit it.		
		next;
	}
	elsif($line =~s/RES_ADD_IMG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^, ]*)\s*\)/RES_ADD_IMG\(\"$1\",$2,\"$3\",\"$4\"\);/g )
	{## 4 params id,path,cmp_type,cmp_param
		print PL $line;
	}
	elsif($line =~m/RES_ADD_IMG2\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^, ]*)\s*\)/)
	{## 4 params id,path,cmp_type,cmp_param
		my $marco = $3;    
		$line = "RES_ADD_IMG\(\"$1\",$2,\"$4\",\"$5\"\);";
	  $tmp =~ s/\"//g;
	  $line =~ s/<1>/$marco/g;
		print PL $line;
	}
	elsif($line =~s/RES_ADD_IMG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*\)/RES_ADD_IMG\(\"$1\",$2,\"$3\"\);/g )
	{## 3 params id,path,cmp_type
		print PL $line;
	}
	elsif($line =~s/RES_ADD_IMG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_IMG\(\"$1\",$2\);/g )
	{## 2 params id,path
		print PL $line;
	}
	elsif($line =~s/RES_ADD_IMG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^, ]*)\s*\)/RES_ADD_IMG_RANGE\(\"$1\",$2,$3,\"$4\",\"$5\"\);/g )
	{## 5 params id,num,path,cmp_type,cmp_param
		print PL $line;
	}
	elsif($line =~s/RES_ADD_ANIM\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^, ]*)\s*,\s*([^, ]*)\s*\)/RES_ADD_ANIM\(\"$1\",$2,\"$3\",\"$4\",\"$5\"\);/g )
	{## 5 params id,path,cmp_type,cmp_param,key_color
		print PL $line;
	}
	elsif($line =~s/RES_ADD_ANIM\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^, ]*)\s*\)/RES_ADD_ANIM\(\"$1\",$2,\"$3\",\"$4\"\);/g )
	{## 4 params id,path,cmp_type,cmp_param
		print PL $line;
	}
	elsif($line =~s/RES_ADD_ANIM\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*\)/RES_ADD_ANIM\(\"$1\",$2,\"$3\"\);/g )
	{## 3 params id,path,cmp_type
		print PL $line;
	}
	elsif($line =~s/RES_ADD_ANIM\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_ANIM\(\"$1\",$2\);/g )
	{## 2 params id,path
		print PL $line;
	}
	elsif($line =~s/RES_ADD_ANIM_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^, ]*)\s*,\s*([^, ]*)\s*\)/RES_ADD_ANIM_RANGE\(\"$1\",$2,$3,\"$4\",\"$5\",\"$6\"\);/g )
	{## 6 params id,num,path,cmp_type,cmp_param,key_color
		print PL $line;
	}
	elsif($line =~s/RES_ADD_RING\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_RING\(\"$1\",$2\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_RING_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_RING_RANGE\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_STRING\s*\(\s*([^, ]*)\s*,\s*(.*)\s*\)/RES_ADD_STRING\(\"$1\",$2\);/g )
	{

		print PL $line;
=head
		print STR_TABLE_PL $line;
=cut
	}
	elsif($line =~s/RES_ADD_STRING_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*(.*)\s*\)/RES_ADD_STRING_RANGE\(\"$1\",$2,$3\);/g )
	{

		print PL $line;

	}
	elsif($line =~s/RES_ADD_FONT\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*\)/RES_ADD_FONT\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_FONT_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*\)/RES_ADD_FONT_RANGE\(\"$1\",$2,$3,$4\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_DATA\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_DATA\(\"$1\",$2\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_DATA_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_DATA_RANGE\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_COLOR\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_COLOR\(\"$1\",\"$2\"\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_COLOR_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_COLOR_RANGE\(\"$1\",$2,\"$3\"\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_RECT\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*\)/RES_ADD_RECT\(\"$1\",\"$2\",\"$3\",\"$4\",\"$5\"\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_RECT_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*\)/RES_ADD_RECT_RANGE\(\"$1\",$2,\"$3\",\"$4\",\"$5\",\"$6\"\);/g )
	{
		print PL $line;
	}
	elsif($line =~ m/RES_ADD_IMG_NULL/)
    	{
    		next;
    	}
    	elsif($line =~s/RES_ADD_IMG_TAG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_IMG_TAG\(\"$1\",$2\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_IMG_TAG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_IMG_TAG_RANGE\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}
	elsif($line =~ m/RES_ADD_ANIM_NULL/)
    {
        next;
    }
	elsif($line =~s/RES_ADD_ANIM_TAG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_ANIM_TAG\(\"$1\",$2\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_ANIM_TAG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_ANIM_TAG_RANGE\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_RING_TAG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_RING_TAG\(\"$1\",$2\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_RING_TAG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_RING_TAG_RANGE\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_COLOR_TAG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_COLOR_TAG\(\"$1\",$2\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_COLOR_TAG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_COLOR_TAG_RANGE\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_RECT_TAG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_RECT_TAG\(\"$1\",$2\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_RECT_TAG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_RECT_TAG_RANGE\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}	
	elsif($line =~s/RES_ADD_FONT_TAG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_FONT_TAG\(\"$1\",$2\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_FONT_TAG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_FONT_TAG_RANGE\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_DATA_TAG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/RES_ADD_DATA_TAG\(\"$1\",$2\);/g )
	{
		print PL $line;
	}
	elsif($line =~s/RES_ADD_DATA_TAG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*\)/RES_ADD_DATA_TAG_RANGE\(\"$1\",$2,$3\);/g )
	{
		print PL $line;
	}
	elsif($line =~m/RES_ADD_IMG_QBTHEME_MAP.*/)
	{		
	}	
	else
	{
		$line =~ s/[ \r\n\t]*//g;
		## the line no match above, maybe there is some syntax error.
		if($line)
		{
=head
			close STR_TABLE_PL;
=cut
			
			close PL;
			close DEF;			
			print $line."\n";
			die  "there is syntax error";
		}
	}
}

=head
print STR_TABLE_PL "\r\n\r\nEND_MODULE($has_str);\r\n";
close STR_TABLE_PL;
=cut

print PL "\r\n\r\n";
print PL "END_GEN_XRM($has_str);\r\n";
print PL "1;\r\n";


close PL;
close DEF;


my $ret = system "\"$perl_cmd_path\" \"$xrm_pl_path\"";

if($ret != 0 )
{
	die "Generate xrm file [$xrm_def_path] failed\n";
}

1;