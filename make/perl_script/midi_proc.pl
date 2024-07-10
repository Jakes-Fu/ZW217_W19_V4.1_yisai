#!/usr/bin/perl

#########################################################################
## 
## File Name:   ListMidi.pl
## Author:      Hongliang.xin
## Description: get the midi file list from resource.
## Usage:       ListMidi.pl path_xrp_def.h -resdir res_path -tmpdir tmp_path -isbf in_soundbank_file
##              -sbps soundbank_bytes_per_sample -osbf out_soundbank_file
## 
## Modification History
## 2010-11-19   hongliang.xin
##              Add new parameter -rep [true|false]
#########################################################################

BEGIN{push @INC,'./make/perl_script/perl_pm'};

use File::Copy;
use File::Find;
use File::DosGlob 'glob';
use File::Path;

if(scalar(@ARGV)<11)
{
	die "[listmidi.pl] param error, number of params at least is 11\n";
}
#in_soundbank_file soundbank_bytes_per_sample
my $xrp_def_path = ${ARGV[0]};
my $res_dir = "";
my $tmp_dir = "";
my $in_soundbank_file = "";
my $soundbank_bytes_per_sample = "";
my $out_soundbank_file = "";
my $pi = 0;
for($pi = 0; $pi<scalar(@ARGV); $pi++)
{
	print "${ARGV[$pi]} ";
} 
print "\narg num:".scalar(@ARGV)."\n";

for($pi = 1; $pi<scalar(@ARGV); $pi++)
{
  $parm_temp = ${ARGV[$pi]};    
  if(lc($parm_temp) eq "-resdir")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		  $res_dir = ${ARGV[$pi+1]};
		  $res_dir =~ s/\\+/\//g;
		  $pi++;
		}
		else
		{
		    die "[listmidi.pl] param error, -resdir [res_path].\n";
		}
	}
	elsif(lc($parm_temp) eq "-tmpdir")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		  $tmp_dir = ${ARGV[$pi+1]};
		  $tmp_dir =~ s/\\+/\//g;
		  $pi++;
		}
		else
		{
		    die "[listmidi.pl] param error, -tmp_dir [tmp_path].\n";
		}
	}	
	elsif(lc($parm_temp) eq "-isbf")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		  $in_soundbank_file = ${ARGV[$pi+1]};
		  $in_soundbank_file =~ s/\\+/\//g;
		  $pi++;
		}
		else
		{
		    die "[listmidi.pl] param error, -isbf [in_soundbank_file].\n";
		}
	}	
	elsif(lc($parm_temp) eq "-sbps")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		  $soundbank_bytes_per_sample = ${ARGV[$pi+1]};	
		  $pi++;
		}
		else
		{
		    die "[listmidi.pl] param error, -sbps [soundbank_bytes_per_sample].\n";
		}
	}
	elsif(lc($parm_temp) eq "-osbf")
	{
		if(($pi+1) < scalar(@ARGV))
		{
		  $out_soundbank_file = ${ARGV[$pi+1]};	
		  $pi++;
		}
		else
		{
		    die "[listmidi.pl] param error, -osbf [out_soundbank_file].\n";
		}
	}
}

$xrp_def_path =~ s/\\+/\//g;

my @module_path = ();
my @midi_list= ();

my $path_base = $xrp_def_path;
$path_base =~ s/(.*)\/[^\/]*.h/$1/g;

if($res_dir eq "")
{
	$res_dir = $path_base;
}
my $path = "";
my $id;
my $num;

open(DEF, "<$xrp_def_path") or die "Can not open $xrp_def_path" ;
#######################################################
## deal with prj_def.h
while($line = <DEF>)
{
  if($line =~ m/RES_ADD_MODULE\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*\)/)
	{	
		$path = $2;
		$path =~ s/[ \r\n\"]*//g;		
		## FONT is expecail module name, and its path must be redefined		
		if( lc($path) ne "font" )
		{	
				push @module_path, $path;
		}	
	}	
	elsif($line =~m/RES_ADD_RING\s*\(\s*([^, ]*)\s*,\s*\"([^,\"]*)\"\s*\)/ )
	{		
		  $line = $res_dir . $2;				  
			if($line =~ m/(\.midi|\.mid)$/i)
			{								
				push @midi_list,$line;				
			}
	}
	elsif($line =~ m/RES_ADD_RING_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*\"([^,\"]*)\"\s*\)/)
	{	
			$id 	 = $1;
			$num  = $2;
			$path = $res_dir . $3;			
			if( $path =~ m/(\.midi|\.mid)$/i )
			{	
					if( $path =~ m/([^\.]+)_1\.([^\.]+)/)
					{
							my $path1 = $1;
							my $ext1  = $2;							
							for(my $i=1;$i<=$num;$i++)
							{
								my $id_temp1 = $id."_".$i;	
								my $path_temp1 = $path1."_".$i.".".$ext1;											
								push @midi_list,$path_temp1;
							}
					}
			}
	}	
}
close DEF;

######################################
foreach $mdu_path (@module_path){
	my $mdu_abs_path = $path_base;
	$mdu_abs_path .= $mdu_path;
	$mdu_abs_path =~ s/\\+/\//g;
	open(MDU, "<$mdu_abs_path") or die "Can not open $mdu_abs_path\n" ;
	while($line = <MDU>)
	{
		if($line =~ m/RES_ADD_RING\s*\(\s*([^, ]*)\s*,\s*\"([^,\"]*)\"\s*\)/ )
		{		
			$path = $2;
			$path =~ s/\#\#/DEFAULT/;	
			if($path =~ m/(\.midi|\.mid)$/i)
			{				
				$line = $res_dir . "$path";							
				
				push @midi_list,$line;				
			}
		}
		elsif($line =~ m/RES_ADD_RING_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*\"([^,\"]*)\"\s*\)/ )
		{
			$id 	= $1;
			$num  = $2;
			$path = $res_dir.$3;
			$path =~ s/\#\#/DEFAULT/;		
			if($path =~ m/(\.midi|\.mid)$/i)
			{									
					if( $path =~ m/([^\.]+)_1\.([^\.]+)/)
					{
							my $path2 = $1;
							my $ext2  = $2;							
							for(my $i2=1;$i2<=$num;$i2++)
							{
								my $id_temp2 = $id."_".$i2;	
								my $path_temp2 = $path2."_".$i2.".".$ext2;											
								push @midi_list,$line;
							}
					}
			}
		}	
	}		
	close MDU;
}

my $dst_ring_path = $tmp_dir."/midi";
if (!(-d $dst_ring_path))
{
	mkpath $dst_ring_path or die "can not create folder [$dst_ring_path]";
}

#my @new_midi_path=();
$in_soundbank_file =~ s/(\/)+/\\\\/g;
$in_soundbank_file ==~ s/^\\\\/\\\\\\\\/g;
$out_soundbank_file =~ s/(\/)+/\\\\/g;
$out_soundbank_file =~ s/^\\\\/\\\\\\\\/g;
my $cmd_param = "./make/make_cmd/midi_soundbank_crop.exe \"$in_soundbank_file\" \"$soundbank_bytes_per_sample\" little \"$out_soundbank_file\"";

foreach $path (@midi_list){
	$path =~ s/(\\)+/\//g;
	my $filename = $path;
	$filename =~ s/.*\/([^ \/]*\.[^ \/\.]*)\z/$1/g;	
	my $dstfile = $dst_ring_path."/".$filename;
	if(-e $dstfile)
	{ 
			unlink glob $dstfile;
	}
	if( -e $path )
	{		
		copy($path, $dstfile) or die "can not copy file [$path] [$dstfile]\n";	
		#push @new_midi_path, $dstfile;
		print $dstfile."\r\n";
		$cmd_param .= " \"$dstfile\"";
	}
	else
	{
		my $new_path = $res_dir;
		$new_path =~ s/(\\)+/\//g;
		$new_path = $new_path . "/../Common/RING/".$filename;
		print $new_path . "\n";
		
		if( -e $new_path )
		{		
			copy($new_path, $dstfile) or die "can not copy file [$new_path] [$dstfile]\n";	
			#push @new_midi_path, $dstfile;
			print $dstfile."\r\n";
			$cmd_param .= " \"$dstfile\"";
		}
	}
}
print "-----copy rings complete-----\n\n";

print $cmd_param."\n";

my $ret = system $cmd_param;
if($ret != 0)
{	
	unlink glob $out_soundbank_file;	
	die "midi_proc failed\n";
}
else
{
	print "\nmidi_proc success\n\n";
}


1;