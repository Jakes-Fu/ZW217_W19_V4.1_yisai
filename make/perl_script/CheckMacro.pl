#
# 功能：
#	检测非开放代码是否使用了客户可修改的宏
#
# 命令格式:
# 	perl CheckMacro.pl IDH代码路径  完整代码路径
#	其中路径要写到MS_Code一级
#
# 示例如下:
# 	perl CheckMacro.pl H:\MOCOR_10A_880XG_W11.36_P1_IDH\MS_Code  H:\mingjian.liu_11.13P_view\MS_Code >result.txt 
#
# 输出信息:文件,行,不应该使用的宏
# 	File: ./MS_Ref/source/dv/dv_sc6600l/src/decoder/aac_dec.c
#        	Line:   584      Macro:    BLUETOOTH_SUPPORT
#

#!/usr/bin/perl

$in_dir_IDH	= $ARGV[0];     # IDH 代码路径 
$in_dir_ALL	= $ARGV[1];     # 完整代码路径


#客户可修改宏
%map = (); 
$Custom_Macro_flag=0;

opendir (inf,$in_dir_IDH) || die $in_dir_IDH;
@allfiles = readdir(inf);
close (inf);

for(local $i = 0; $i < @allfiles; $i++)
{
        next if($allfiles[$i] eq "." || $allfiles[$i] eq "..");
	$full_dir = $in_dir_IDH."/".$allfiles[$i];
	next if(-d $full_dir);
	if($allfiles[$i]=~/^project_\w+\.mk$/)
	{
		open(inf1,$full_dir) || die $full_dir;
		@alllines = <inf1>;
		close (inf1);

		$str="";
		foreach (@alllines)
		{
			if($_ =~/^\s*\#\s+Feature\s+Options\s+FOR\s+SPRD/i)
			{
				$Custom_Macro_flag=0;
			}
			if($_ =~/^\s*\#\s*Feature\s+Options\s+\(\s*FOR\s+User\s*\)/i)
			{
				$Custom_Macro_flag=1;
			}
			if($Custom_Macro_flag==1)
			{
    				$str.=$_;
			}
		}

		while($str=~/(\n*\s*)(\w+)(\s+=\s*)(\w+)(\s*\#*)(.*?)(((\n\s*\#\s*Option\d{1}\s*\:\s*)(\w+)(.*))+)/g)
		{
    			$all_options = $7;
    			$current_option = $2;
    			$current_option_value = $4;
    			$tmp= $all_options ;
    			$tmp=~s/(.*?)(Option\d{1})(\s*\:\s*)(\w+)(.*)/$4/g;
    			@all_option = split("\n",$tmp);
    			foreach $CurrentOpt(@all_option)
    			{  
				$CurrentOpt=~s/^\s*|\s*$//g;
				
				$map{$current_option}=1;
				if($CurrentOpt ne "TRUE" && $CurrentOpt ne "FALSE" && $CurrentOpt ne "")
				{
					$temp = $current_option."_".$CurrentOpt;
					$map{$temp}=1;
				}
     			}
		}
	}
}

foreach (sort keys(%map))
{
	#print $_."  ";
}

chdir($in_dir_ALL);

scandir(".");

sub scandir
{
    my $dir = shift;
    opendir ($folder,$dir) or die "Can not open $dir directory!\n";
    local @array=readdir($folder);

    for(local $i = 0; $i < @array; $i++)
    {
        next if($array[$i] eq "." || $array[$i] eq "..");
        $full_dir = $dir."/".$array[$i];
	if(-f $full_dir && ($array[$i] =~ /\.c$/i || $array[$i] =~ /\.cpp$/i || $array[$i] =~ /\.cxx$/i || $array[$i] =~ /\.h$/i || $array[$i] =~ /\.def$/i || $array[$i] =~ /\.nv$/i))
	{
		if(!-e ($in_dir_IDH.$full_dir))
		{
			Check_OPTION($full_dir);
		}
	}
        if(-d $full_dir)
        {
	        scandir($full_dir);
        }
    }	
    closedir($folder);
}


sub Check_OPTION
{
	my ($filename) = @_;
	open (inf1,$filename) || die $filename;
	@alllines = <inf1>;
	close (inf1);

	%filemap = (); 

	$str="";

	$cnt=1;

	foreach (@alllines)
	{
		$tmp=$_;
		$tmp=~s/\n//g;
		$tmp=~s/\r//g;

		$tmp0=$tmp;

		if($tmp=~/(\#\s*ifdef\s+|\#\s*ifndef\s+)(\w+)/)
		{
			while($tmp=~/(\#\s*ifdef\s+|\#\s*ifndef\s+)(\w+)/g)
			{
				$item=$2;
				if($map{$item} eq 1)
				{
					print "\n\nFile: ".$filename."\n" if($filemap{$filename} ne 1);
					print "\t\t\tLine:   $cnt\t\tMacro:\t$item\n";
					$filemap{$filename}=1;
				}
			}
		}
		else 
		{
			if($tmp=~/(defined\s*\(\s*|defined\s+)(\w+)/)
			{
				while($tmp=~/(defined\s*\(\s*|defined\s+)(\w+)/g)
				{
					$item=$2;
					if($map{$item} eq 1)
					{
						print "\n\nFile: ".$filename."\n" if($filemap{$filename} ne 1);
					        print "\t\t\tLine: $cnt\t\tMacro:\t$item\n";
						$filemap{$filename}=1;
					}
				}
			}
			else
			{
				if($tmp=~/(defined\s+)(\w+)/)
				{
					while($tmp=~/(defined\s+)(\w+)/g)
					{
						$item=$2;
						if($map{$item} eq 1)
						{
							print "\n\nFile: ".$filename."\n" if($filemap{$filename} ne 1);
							print "\t\t\tLine: $cnt\t\tMacro:\t$item\n";
							$filemap{$filename}=1;
						}
					}
				}
				else
				{
					if($tmp=~/(\#\s*elif\s+)(\w+)/)
					{
						while($tmp=~/(\#\s*elif\s+)(\w+)/g)
						{
							$item=$2;
							if($map{$item} eq 1)
							{
								print "\n\nFile: ".$filename."\n" if($filemap{$filename} ne 1);
								print "\t\t\tLine: $cnt\t\tMacro:\t$item\n";
								$filemap{$filename}=1;
							}
						}
					}
				}
			}
		}
		$cnt++
	}
}

