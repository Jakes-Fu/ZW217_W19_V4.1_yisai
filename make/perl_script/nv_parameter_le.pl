#!/usr/bin/perl

$NUM_ARGV = @ARGV;

$Release_file="";
if($NUM_ARGV == 8)
{
	$nv_group = $ARGV[0];
	$out_dir  = $ARGV[1];
	$CMD_Enable = $ARGV[2];
	$Root_path = $ARGV[3];
	$Release_file = $ARGV[4];
	$ReleaseDebug = $ARGV[5];
	$ProjectName = $ARGV[6];
	$Deltanv = $ARGV[7];
}
else
{
	if($NUM_ARGV == 7)
	{
		$nv_group = $ARGV[0];
		$out_dir  = $ARGV[1];
		$CMD_Enable = $ARGV[2];
		$Root_path = $ARGV[3];
		$Release_file = $ARGV[4];
		$ReleaseDebug = $ARGV[5];
		$ProjectName = $ARGV[6];
	}
	else
	{
		if($NUM_ARGV == 6)
		{
			$nv_group = $ARGV[0];
			$out_dir  = $ARGV[1];
			$CMD_Enable = $ARGV[2];
			$Root_path = $ARGV[3];
			$Release_file = $ARGV[4];
			$ReleaseDebug = $ARGV[5];
		}
		else
		{
			return;
		}
	}
}

$RN = "\n";

$prj_str = "";
$prj_str = $prj_str . "# SpreadTrum NVEditor Project File, Format Version 1.0$RN";
$prj_str = $prj_str . "# WARNING: DO NOT EDIT OR DELETE THIS PROJECT FILE!$RN";
$prj_str = $prj_str . "$RN################################################################################$RN";
$prj_str = $prj_str . "  PROJECT     =  NVitem$RN";
$prj_str = $prj_str . "  ALIGNMENT   =  4$RN";
$prj_str = $prj_str . "  TARGET      =  FILE_0$RN";
$prj_str = $prj_str . "DESCRIPTION =  $RN";
$prj_str = $prj_str . "$RN################################################################################$RN";
$prj_str = $prj_str . "# Begin Project Module$RN$RN";
#$prj_str = $prj_str . "MODULE     =  FILE_1$RN";
#$prj_str = $prj_str . "MODULE     =  FILE_2$RN";
#$prj_str = $prj_str . "MODULE     =  FILE_3$RN";
#$prj_str = $prj_str . "MODULE     =  FILE_4$RN";
#$prj_str = $prj_str . "MODULE     =  FILE_5$RN";
#$prj_str = $prj_str . "MODULE     =  FILE_6$RN";
#$prj_str = $prj_str . "$RN# End Project Module$RN";
#$prj_str = $prj_str . "$RN################################################################################$RN";

$out_dir =~ s/(^\s+|\s+$)//g;
$nv_group=~ s/\\\;/\;/g;
@all_group = split("\\;",$nv_group);

foreach $item (@all_group)
{
	$item =~ s/(^\s+|\s+$)//g;
	if ($item  ne "")
	{
		($groupName,$fileList)=($item=~m|(.*)=(.*)|);
		$groupName  =~ s/(^\s+|\s+$)//g;
		if($ReleaseDebug eq 1)
		{
			$groupNameOut=$groupName."_release";
		}
		else
		{
			$groupNameOut=$groupName;
		}
		@all_filelist = split(' ',$fileList);
		$i=0;
		$prj_out = $prj_str;
		$prj_filename="";
		foreach $file_item (@all_filelist)
		{
			$file_item  =~ s/(^\s+|\s+$)//g;
			if ($file_item ne "")
			{
				$i++;
				$shortFileName = GetFileName($file_item);
				$prj_out =~ s/FILE_0/$groupNameOut.bin/g;
				$prj_filename = $groupNameOut.".prj";
				$prj_out = $prj_out . "MODULE     =  $shortFileName$RN";
				if($CMD_Enable eq "1")
				{
					$out_dir=~ s/\//\\/g;
					$file_item=~ s/\//\\/g;
					system("copy /B /Y $file_item $out_dir\\$groupName\\ ");
				}
				else
				{
					system("cp -a $file_item $out_dir/$groupName/");
					system("chmod 777 $out_dir/$groupName/$shortFileName");
				}
			}
		}

		$prj_out = $prj_out . "$RN# End Project Module$RN";
		$prj_out = $prj_out . "$RN################################################################################$RN";

		if($CMD_Enable eq "1")
		{
			$prj_path = "$Root_path/$out_dir/$groupName/$prj_filename";
			$prj_path =~ s/\//\\\\/g;
			$release_seg_path = "$Root_path/$Release_file" if($Release_file ne "");;
			$release_seg_path =~ s/\//\\\\/g;
			
			open (inf,">$prj_path") || die ("$prj_path");
			@list = <inf>;
			print inf $prj_out;
			close(inf);
			print ".\\tools\\DEBUG_TOOL\\NVEDITOR\\Bin\\NVEditor.exe ".$prj_path." $release_seg_path\n";
			if($NUM_ARGV == 8)
			{
				$Deltanv =~ s/\//\\\\/g;
				if(-e $Deltanv)
				{
					system(".\\tools\\DEBUG_TOOL\\NVEDITOR_for_deltaNV\\Bin\\NVEditor.exe \"".$prj_path."\" \"$release_seg_path\" -L  -c ".$Deltanv."\"");
				}
				else
				{
					print "error!!!we can't find $Deltanv!!!\n";
				}
			}
			else
			{
				if($NUM_ARGV == 7)
				{
					system(".\\tools\\DEBUG_TOOL\\NVEDITOR\\Bin\\NVEditor.exe \"".$prj_path."\" \"$release_seg_path\" -L  -m .\\lib\\modem\\$ProjectName\\indeltanv .\\version\\nv_config.h");
				}
				else
				{
					system(".\\tools\\DEBUG_TOOL\\NVEDITOR\\Bin\\NVEditor.exe \"".$prj_path."\" \"$release_seg_path\" -L");				
				}
			}
		}
		else
		{
			$prj_path = "$Root_path/$out_dir/$groupName/$prj_filename";
			$release_seg_path = "$Root_path/$Release_file" if($Release_file ne "");			
			open (inf,">$prj_path") || die ("$prj_path!!!");
			@list = <inf>;
			print inf $prj_out;
			close(inf);
			print "./tools/DEBUG_TOOL/NVEDITOR/Linux/Bin/NVGen ".$prj_path." $release_seg_path\n";
			if($NUM_ARGV == 8)
			{
				if(-e $Deltanv)
				{
					#system("./tools/DEBUG_TOOL/NVEDITOR_for_deltaNV/Bin/NVEditor.exe \"".$prj_path."\" \"$release_seg_path\" -L  -c ".$Deltanv."\"");
					system("./tools/DEBUG_TOOL/NVEDITOR_for_deltaNV/Linux/Bin/NVGen $prj_path $release_seg_path -L  -c ./$Deltanv");
				}
				else
				{
					print "error!!!we can't find $Deltanv!!!\n";
				}

				#$hw_info_path = "./common/nv_parameters/$ProjectName";
				#if(-e $hw_info_path)
				#{
				#	system("./tools/DEBUG_TOOL/NVEDITOR_for_deltaNV/Linux/Bin/NVGen $prj_path $release_seg_path -L  -c ./common/nv_parameters/$ProjectName");
				#}
				#else
				#{
				#	print "error!!!we can't find $hw_info_path!!!\n";
				#}
			}
			else
			{
				if($NUM_ARGV == 7)
				{
					system("./tools/DEBUG_TOOL/NVEDITOR/Linux/Bin/NVGen $prj_path $release_seg_path -L  -m project/config_nv/$ProjectName version/nv_config.h");
				}
				else
				{
					system("./tools/DEBUG_TOOL/NVEDITOR/Linux/Bin/NVGen $prj_path $release_seg_path -L");				
				}
			}
			
		}
	}	
}

sub GetFileName
{
	local($filename) = @_;
	$filename=~ s/(.*)\///g;
	return $filename;
}



