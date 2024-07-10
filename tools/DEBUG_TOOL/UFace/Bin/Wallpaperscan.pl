#!/usr/bin/perl

#########################################################################
## 
## File Name:   wallperscan.pl
## Author:      Hongliang.xin
## Description: generate a ini file for live wallpaper tool.
## Usage:       wallperscan.pl xxx_def.h yyy.ini
## 
## Modification History
## 2012-02-22   hongliang.xin
##              create
#########################################################################

#BEGIN{push @INC,'./make/perl_script/perl_pm'};

#use File::Copy;
#use File::Find;
#use File::DosGlob 'glob';
#use File::Path;

if(scalar(@ARGV)<2)
{
	die "[wallperscan.pl] param error, number of params at least is 2\n";
}

my $def_path = ${ARGV[0]};
my $ini_path = ${ARGV[1]};

for($i = 0; $i<scalar(@ARGV); $i++)
{
	print "${ARGV[$i]} ";
} 
#print "\narg num:".scalar(@ARGV)."\n";
print "\n";


$def_path =~ s/\\+/\//g;
$ini_path =~ s/\\+/\//g;

my %h_macro = {};
my @v_macro =  ();

my %h_id = {};
my @v_id = ();

my %h_dir = {};
my @v_dir = ();

open(DEF, "<$def_path") or die "Can not open $def_path" ;

#######################################################
## deal with each line
while($line = <DEF>)
{
	#$line =~ s/[ 	]*//g;
	if($line =~ m/^\s*#define\s*([^ ]+)\s*(.*)/)
	{	    
	    $key = $1;
	    $value = $2;
	    $key =~ s/^\s*//g;
	    $key =~ s/\s*$//g;
	    $value=~ s/^\s*//g;
	    $value=~ s/\s*$//g;
	    
	    #print $key."=".$value."\n";
	    $macro{$key} = $value;	
	    push @v_macro, $key;
	}
	elsif($line =~ m/^\s*RES_ADD_IMG\s*\(.*\)/ )
	{## 4 params id,path,cmp_type,cmp_param
	    foreach $mm (@v_macro)
	    {
	        $line =~ s/\b$mm\b/$macro{$mm}/g;	    
	    }
	    #print $line;
	    if($line =~s/\s*RES_ADD_IMG\s*\(\s*([^, ]*)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^, ]*)\s*\)/RES_ADD_IMG\(\"$1\",$2,\"$3\",\"$4\"\);/g )
		{
		   $id = $1;
		   $path = $2;
		   $path =~ s/\"//g;
		   $path =~ s/\s*\\\\MMI_RES_##\\\\IMAG\\\\//g;			   
		   $path =~ s/\\\\/\\/g;
		   $path =~ s/\s*$//g;
		   $path =~ s/^\s*//g;
		   
		   if(exists($h_id{$id}))
		   {
		        if($path =~ m/\.png$/)
		        {
		           $h_id{$id} = $path;	
		        }
		        elsif(($path =~ m/\.(jpg|jpeg)$/) && !($h_id{$id} =~ m/\.png$/))
		        {
		            $h_id{$id} = $path;
		        }
		        elsif(($path =~ m/\.gif$/) && !($h_id{$id} =~ m/\.png$/) && !($h_id{$id} =~ m/\.(jpg|jpeg)$/))
		        {
		            $h_id{$id} = $path;
		        }		        
		   }
		   else
		   {
		        push @v_id, $id;
		        $h_id{$id} = $path;	
		        #print $id;
		   }		       
		}
	}
	elsif($line =~ m/^\s*RES_ADD_IMG_RANGE\s*\(.*\)/ )
	{
	    foreach $mm (@v_macro)
	    {
	        $line =~ s/\b$mm\b/$macro{$mm}/g;	    
	    }
	    
    	if($line =~s/\s*RES_ADD_IMG_RANGE\s*\(\s*([^, ]*)\s*,\s*(\d+)\s*,\s*([^,]*)\s*,\s*([^,]*)\s*,\s*([^, ]*)\s*\)/RES_ADD_IMG_RANGE\(\"$1\",$2,$3,\"$4\",\"$5\"\);/g )
    	{## 5 params id,num,path,cmp_type,cmp_param
    		$idb = $1;
    		$count = $2;
    		$pathb = $3;
    		$pathb =~ s/\\\\MMI_RES_##\\\\IMAG\\\\//g;	
    		$pathb =~ s/\"//g;	
    		$pathb =~ s/\\\\/\\/g;
    		for($i=1;$i<=$count;$i++)
    		{
    		   $path = $pathb;
    		   my $id = $idb."_".$i;
    		   if($i>1)
    		   {
    		       $path =~ s/(_1\.)/_$i\./g;	 
    		   }
    		   if(exists($h_id{$id}))
    		   {
    		        if($path =~ m/\.png$/)
    		        {
    		           $h_id{$id} = $path;	
    		        }
    		        elsif(($path =~ m/\.(jpg|jpeg)$/) && !($h_id{$id} =~ m/\.png$/))
    		        {
    		            $h_id{$id} = $path;
    		        }
    		        elsif(($path =~ m/\.gif$/) && !($h_id{$id} =~ m/\.png$/) && !($h_id{$id} =~ m/\.(jpg|jpeg)$/))
    		        {
    		            $h_id{$id} = $path;
    		        }		        
    		   }
    		   else
    		   {
    		        push @v_id, $id;
    		        $h_id{$id} = $path;	
    		        #print $id;
    		   } 
    		}
    		
    	}
    }
    elsif($line =~ m/^\s*LW_RES_IMG_PATH\s*\(\s*([^, ]*)\s*,\s*([^\)]*)\s*\)/)
    {
        $id = $1;
        $path = $2;
        $path =~ s/\\\\MMI_RES_##\\\\IMAG\\\\//g;	
    	$path =~ s/\"//g;	
    	$path =~ s/\\\\/\\/g;
    	$id =~ s/\"//g;	
    	
    	if(exists($h_dir{$path}))
    	{
    	    $h_dir{$path} = $h_dir{$path}.",".$id ;
    	}
    	else
    	{
        	$h_dir{$path} = $id;	
    	    push @v_dir, $path;
	    }
    }
}

close DEF;

open(INI, ">$ini_path") or die "Can not create and open $ini_path" ;

print INI "[Settings]\n";
#print INI "[dir]\n";

foreach $path (@v_dir)
{
    print INI uc($path)."=".uc($h_dir{$path})."\n"; 
}

#print INI "[Settings]\n";

foreach $id (@v_id)
{
    print INI uc($h_id{$id})."=".uc($id)."\n"; 
}



close INI;


print "complete\n";


1;