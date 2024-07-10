#!/usr/bin/perl

#########################################################################
## 
## File Name:   preload.pl
## Author:      polo.jiang
## Description: Copy files or folder to dest directory.
## Usage:       preload.pl preload_def.h resource_dir temp_dir preload_img.bin img_size
## 
## Modification History
## 2011-09-28   polo.jiang
##              Create 

#########################################################################

BEGIN{push @INC,'./make/perl_script/perl_pm'};
BEGIN{push @INC,'./perl_pm'};

use File::Copy;
use File::Find;
use File::DosGlob 'glob';
use File::Path;
use File::Basename;

if(scalar(@ARGV)<4)
{
	die "[Preload.pl] param error, number of params at least is 4\n";
}

my $srcpreloadh = ${ARGV[0]}; 
my $preloaddir = ${ARGV[1]};
my $tempdir = ${ARGV[2]};
my $outpreloaddir = ${ARGV[3]};
my $preloadsize = ${ARGV[4]};
my $bIsM8Flash = -1;

$srcpreloadh =~ s/\\+/\//g;
$preloaddir =~ s/\\+/\//g ;
$tempdir =~ s/\\+/\//g;
$outpreloaddir =~ s/\\+/\//g;

my $pi = 0;
for($pi = 0; $pi<scalar(@ARGV); $pi++)
{
	print "${ARGV[$pi]} ";
} 
print "\narg num:".scalar(@ARGV)."\n";


sub checkIfFolder{
  my $name=$_[0];
   
  if( opendir DH,$name){
     closedir DH;
     return 1;
  }
  return 0;  
}

sub CopyFolder{   
	 my $src=$_[0];      #source folder 
	 my $target=$_[1];   #target folder 
	 my $targettmp = $target;
	 
	 $targettmp =~ s/\\+/\//g;
	 $src =~ s/\\+/\//g;
	 $target =~ s/\\+/\//g; 
	 
	 if (!(-d $targettmp))
	 {
	 	print $targettmp "\n";
		mkpath $targettmp or die "can not create folder [$targettmp]";
	 } 
	 opendir DH,$src or die "Can't Open $src,Information:$!!\n"; 
	 
	 my @dirs=readdir DH;   
	 foreach my$fd(@dirs)
	 {
	 	if(checkIfFolder($src."//".$fd))
	 	{  #directory
	 		CopyFolder($src."//".$fd,$target."//".$fd) if($fd !~ /^(\.|\.\.)$/);
	 	}
	 	else
	 	{ #file,copy directly
	 		print "Copy $src\\$fd to $target\\$fd...\n";
	    copy($src."//".$fd,$target."//".$fd); 
	  }      
	}
	closedir DH;  
}

sub CopySingleFile{   
	 my $src=$_[0];      #source folder 
	 my $target=$_[1];   #target folder 
	
	 $src =~ s/\\+/\//g;
	 $target =~ s/\\+/\//g; 
	 
	 $tmptarget = $target;
	 $tmptarget =~ s/\\+/\//g;
	 my $filename = basename($tmptarget);
	 my $dir = dirname($tmptarget);
	 print "filename = $filename \n targetDir= $dir \n"; 
	 
	 if (!(-d $dir))
	 {
		mkpath $dir or die "can not create folder [$dir]";
		print "MakeDirectory $dir successfully. \n";
	 }
	 
	 print "Copy $src to $target...\n";
	 copy($src,$target); 
}

open(SRCH, "<$srcpreloadh") or die "Can not open $srcpreloadh" ;
while($line = <SRCH>)
{
	##remove space char
	if($line =~s/SFS_ADD_FOLDER\s*\(\s*([^,]*)\s*\)/$1/g )
	{
		$line =~s/^\s*"*|"*\s*$//g; #remove ""
	  CopyFolder($preloaddir."//".$line,$tempdir."//".$line); 
	}
	elsif($line =~s/SFS_ADD_FILE\s*\(\s*([^,]*)\s*\)/$1/g )
	{
		$line =~s/^\s*"*|"*\s*$//g; #remove ""	
		CopySingleFile($preloaddir."//".$line,$tempdir."//".$line); 
	}
	elsif($line =~s/SFS_SET_IS_M8_FLASH\s*\(\s*([^,]*)\s*\)/$1/g )
	{
		$line =~s/^\s*"*|"*\s*$//g; #remove ""	
		$line =lc($line);
		if( $line eq "true")
		{
			$bIsM8Flash = 1;
		}
		elsif( $line eq "false")
		{
			$bIsM8Flash = 0;
		}
		elsif( $line eq "0")
		{
			$bIsM8Flash = 0;
		}
		elsif( $line eq "1")
		{
			$bIsM8Flash = 1;
		}
		elsif( $line eq "2")
		{
			$bIsM8Flash = 2;
		}
		elsif( $line eq "3")
		{
			$bIsM8Flash = 3;
		}
		else
		{
			$bIsM8Flash = -1;
		}
	}
}
close SRCH;


my $tmpprloadfile = $outpreloaddir;#."\\preload_img.bin";
my $imgtoolsrcdir = $tempdir."//preload_img";
$tmpprloadfile =~ s/\\+/\//g;
$imgtoolsrcdir =~ s/\\+/\//g;
if (!(-d $imgtoolsrcdir))
{
	print "This project don't config preload_img,so don't need preload_img.\n";
}
else
{
	my $ret = 0;
	if ($bIsM8Flash != -1)
	{
		$ret = system "./make/make_cmd/ImgTool.exe \"$imgtoolsrcdir\" $preloadsize \"$tmpprloadfile\" $bIsM8Flash";
		print "call ImgTool parameter $imgtoolsrcdir $preloadsize $tmpprloadfile $bIsM8Flash \n";
	}
	else
	{
		$ret = system "./make/make_cmd/ImgTool.exe \"$imgtoolsrcdir\" $preloadsize \"$tmpprloadfile\"";
		print "call ImgTool parameter $imgtoolsrcdir $preloadsize $tmpprloadfile \n";
	}
	
	if($ret != 0)
	{
		die "Generate preload file failed\n";
	}
	else
	{
		print "Generate preload file successfully.\npreload file on $tmpfile\n";
	}
}


