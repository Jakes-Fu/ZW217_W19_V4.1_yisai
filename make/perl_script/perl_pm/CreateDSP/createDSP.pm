BEGIN{push @INC,'./make/perl_script/perl_pm'};
package createDSP;
use strict;
use File::Basename;

my $workspace_small="msdevkernel";
my $workspace_big="MSDev";

###############################################################################
sub Get_Top
{

my ($lib) = @_;
my $result = <<__DSP_SCRIPT__
# Microsoft Developer Studio Project File - Name="$lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=$lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "$lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "$lib.mak" CFG="$lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "$lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "$lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "$lib"
# PROP Scc_LocalPath "..\\..\\..\\.."
CPP=cl.exe
RSC=rc.exe

__DSP_SCRIPT__
;
return $result;
}

sub Get_Top_dll
{
my ($lib) = @_;
my $result = <<__DSP_SCRIPT__
# Microsoft Developer Studio Project File - Name="$lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=$lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "$lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "$lib.mak" CFG="$lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "$lib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "$lib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "$lib"
# PROP Scc_LocalPath "..\\..\\..\\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

__DSP_SCRIPT__
;
return $result;
}



###############################################################################
sub Get_Compile_Option
{
my ($lib,$compile_option,$inc_path,$tmpdir,$outdir,$pdbflag) = @_;

my $result;
my $myoption_list = "";
foreach (split("\n",$compile_option.$inc_path))
{
	my $myoption=$_;
	$myoption=~ s/^\s+|\s+$//g;
	if($myoption ne "")
	{
		$myoption_list .= "# ADD CPP $myoption\n";
	}
}
$result = <<__DSP_SCRIPT__
!IF  "\$(CFG)" == "$lib - Win32 Release"
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
__DSP_SCRIPT__
;

$result .= <<__DSP_SCRIPT__
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
__DSP_SCRIPT__
;

$result .= <<__DSP_SCRIPT__
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
!ELSEIF  "\$(CFG)" == "$lib - Win32 Debug"
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
__DSP_SCRIPT__
;

if($pdbflag eq "TRUE")
{
$result .= <<__DSP_SCRIPT__
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI 
__DSP_SCRIPT__
;
}
else
{
$result .= <<__DSP_SCRIPT__
# ADD BASE CPP /nologo /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /GX
__DSP_SCRIPT__
;
}

$result .= <<__DSP_SCRIPT__
$myoption_list
# ADD CPP   /FD /D /GZ /Zm1000 /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"$outdir/$lib.lib"
!ENDIF
__DSP_SCRIPT__
;
return $result;
}



###############################################################################
sub Get_Compile_Option_dll
{
my ($lib,$compile_option,$inc_path,$tmpdir,$outdir,$add_link32,$pdbflag) = @_;

my $result;

my $myoption_list = "";
foreach (split("\n",$compile_option.$inc_path))
{
	my $myoption=$_;
	$myoption=~ s/^\s+|\s+$//g;
	if($myoption ne "")
	{
		$myoption_list .= "# ADD CPP $myoption\n";
	}
}

$result = <<__DSP_SCRIPT__
!IF  "\$(CFG)" == "$lib - Win32 Release"
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MSKERNEL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MSKERNEL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32

# ADD CPP /FR /FD /GZ /c
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 $add_link32 /nologo /dll /machine:I386 /pdbtype:sept
# ADD LINK32 $add_link32 /nologo /dll /machine:I386

!ELSEIF  "\$(CFG)" == "$lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "$outdir"
# PROP Intermediate_Dir "MSKernel_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
__DSP_SCRIPT__
;

if($pdbflag eq "TRUE")
{
$result .= <<__DSP_SCRIPT__
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MSKERNEL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /Gm /GX /ZI
__DSP_SCRIPT__
;
}
else
{
$result .= <<__DSP_SCRIPT__
# ADD BASE CPP /nologo /MTd /W3 /GX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MSKERNEL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp4 /MTd /W4 /GX
__DSP_SCRIPT__
}

$result .= <<__DSP_SCRIPT__
$myoption_list
# ADD CPP /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 $add_link32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32  $add_link32 /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCMTD.lib" /nodefaultlib:"LIBCD.lib" /nodefaultlib:"LIBCMT.lib" /nodefaultlib:"LIBC.lib" /out:"$outdir/${workspace_small}.dll" 
# SUBTRACT LINK32 /pdb:none

!ENDIF 
__DSP_SCRIPT__
;
return $result;
}

my %added_file_list_table;



###############################################################################
sub Get_File_Compile_Option
{
	my ($lib,$fullpath,$node,$WIN32_COMPILE_OPTION,$tmpdir) = @_;

	my $result = "";
	$result .= "# Begin Source File \n";
	$result .= "SOURCE=$tmpdir$fullpath/$node\n";
	if(defined($$WIN32_COMPILE_OPTION{"$fullpath/$node"}))
	{
		$result .= "# ADD CPP " . $$WIN32_COMPILE_OPTION{"$fullpath/$node"} . "\n";
	}
	elsif(defined($$WIN32_COMPILE_OPTION{$node}))
	{
		$result .= "# ADD CPP " . $$WIN32_COMPILE_OPTION{$node} . "\n";
	}
	$result .= "# End Source File \n";
	return $result;
}



###############################################################################
sub Get_File_Group
{
	my ($level,$lib,$fullpath,$groupName,$TopName,$file_list,$WIN32_COMPILE_OPTION,$tmpdir) = @_;
	my $result = "";
        if($groupName ne "c" && $groupName ne "source" && $groupName ne "src")
        {
		$result .= "# Begin Group \"$groupName\"\n" ;
        }
	$level++;
	foreach my $node (sort split(" ",Files_in_Dir($fullpath,$file_list)))
	{
		if(-d "$fullpath/$node")
		{
			my $content = Get_File_Group($level,$lib,"$fullpath/$node",$node,$TopName,$file_list,$WIN32_COMPILE_OPTION,$tmpdir);
			$result .= $content;
		}
		else
		{
			next if($node =~ /\.s$/i);
			next if(defined($added_file_list_table{"$fullpath/$node"}));
			$added_file_list_table{"$fullpath/$node"} = 1;
			$result .= Get_File_Compile_Option($lib, $fullpath,$node,$WIN32_COMPILE_OPTION,$tmpdir);
		}
	}
        if($groupName ne "c" && $groupName ne "source" && $groupName ne "src")
	{$result .= "# End Group\n" ;
        }
	$level--;
	return $result;
}



###############################################################################
sub Get_File_List
{
	my ($lib,$file_list,$WIN32_COMPILE_OPTION,$tmpdir) = @_;
	my $File_list = "";
	foreach my $node (sort split(" ",Files_in_Dir("",$file_list)))
	{
		$File_list .= Get_File_Group(0,$lib,"$node","$node","$node",$file_list,$WIN32_COMPILE_OPTION,$tmpdir);
	}
	my $result;
	$result = "\n";
	$result .= "# Begin Target\n";
	$result .= "# Name \"$lib - Win32 Release\"\n";
	$result .= "# Name \"$lib - Win32 Debug\"\n";
	$result .= $File_list."\n";
	return $result;
}

###############################################################################
sub Get_VS_2010_File_Name_Compile_Option
{
	my ($lib,$fullpath,$node,$WIN32_COMPILE_OPTION,$groupName) = @_;

	my $result = "";
	my $filter_src_list = "";
	my $filter_name_list = "";
	my $filter = "";
	
  my @values = split($groupName, $fullpath);
  my @filter_node = split("/", $values[0]);
  
#  print "case1".$fullpath."\n";
#  print "case2".$groupName."\n";

	foreach my $var (@filter_node)
	{                
#		print "case3".$var."\n";
		$filter .= $var."///";

#		$filter .= $var."\\\\";
	}
	$filter .= $groupName." \\\n";
	
	$result .= $filter ; 

	return $result;
}

###############################################################################

sub Get_VS_2010_File_Group_Name
{
	my ($level,$lib,$fullpath,$groupName,$TopName,$file_list,$WIN32_COMPILE_OPTION,$static_group_name) = @_;
	my $result = "";

  if($groupName ne "c" && $groupName ne "source" && $groupName ne "src")
  {
		$static_group_name ="";
		$static_group_name .= $groupName;
  }
	$level++;
	foreach my $node (sort split(" ",Files_in_Dir($fullpath,$file_list)))
	{                
		if(-d "$fullpath/$node")
		{
		  #print "case1".$node."\n";
			my $content = Get_VS_2010_File_Group_Name($level,$lib,"$fullpath/$node",$node,$TopName,$file_list,$WIN32_COMPILE_OPTION,$static_group_name);
			$result .= $content;
		}
		else
		{
		  #print "case2".$node."\n";
			next if($node =~ /\.s$/i);
			next if(defined($added_file_list_table{"$fullpath"}));
			$added_file_list_table{"$fullpath"} = 1;
#			print "Get_File_Group_Name add added_file_list_table "."$fullpath"."\n";
			$result .= Get_VS_2010_File_Name_Compile_Option($lib, $fullpath,$node,$WIN32_COMPILE_OPTION,$static_group_name);
		}
	}

	$level--;
	return $result;
}

###############################################################################
sub Get_VS_2010_File_Group
{
	my ($file_list,$File_group_name_list) = @_;
	my @all_group_name_list = split("\n",$File_group_name_list);
	my @all_file_list = split("\n",$file_list);
	my $result = "";
	my $has_sub_folder = 0;
	my @group_name_token ="";
 	my @file_path_token ="";
  my $fragment = "" ;
  my $folder_list = "" ;

	foreach my $group_name (@all_group_name_list)
  {
#  	print "Get_File_Group_ex  case1 group_name is $group_name \n" ;
#  	@group_name_token = split(/.+\\\\/,$group_name);  
  	@group_name_token = split (/(.+[\/])/g ,  $group_name) ;  
		foreach (@group_name_token)
		{ 
			next if($_ eq "");
			next if(index($_ , "Filter_list") ne -1);
			next if(index($_ , "///") ne -1);

			$fragment = substr($_ , 0 , -2);
#			print "Get_File_Group_ex  case2  group_name_token $fragment \n";	
			$result .= "Filter_Group_$fragment = \\\n";
			
			foreach my $file_path (@all_file_list)
			{
				next if($file_path =~ /\.s$/i);				
				next if(index($file_path,$fragment) eq -1);				
#				print "Get_File_Group_ex  case3  file_path $file_path \n";	
				next if(index($file_path , "/$fragment/") eq -1);
				$folder_list = substr($file_path , index($file_path , "/$fragment/") , -1);
#				print "Get_File_Group_ex  case4  folder_list $folder_list \n";
				$has_sub_folder = 0;
					
				foreach my $folder_token (split("/",$folder_list))
				{
#					print "Get_File_Group_ex  case5  folder_token $folder_token \n";
					if($folder_token ne "" && $folder_token ne "$fragment" && $folder_token ne "c" && $folder_token ne "source" && $folder_token ne "src" && index($folder_token , ".") eq -1)
					{		
#						print "Get_File_Group_ex  case6  has sub_folder_token $folder_token \n";
						$has_sub_folder = 1;
					}
				}
				
				if($has_sub_folder eq 0)
				{
					$result .= "$file_path \\\n" ;
				}
			}			
			
			$result .= "\$(NULL) #$fragment\n\n";
		}  		
  }

	return $result;
}


###############################################################################
sub Get_VS2010_File_List
{   
	my ($lib,$compile_option ,$inc_path , $file_list,$WIN32_COMPILE_OPTION,$tmpdir) = @_;
	my $File_list = "";
	my $File_group_name = "";
	my $All_Flie_list = "";

	$File_group_name .= "Filter_list = \\\n";
	
	foreach my $node (sort split(" ",Files_in_Dir("",$file_list)))
	{
		$File_group_name .= Get_VS_2010_File_Group_Name(0,$lib,"$node","$node","$node",$file_list,$WIN32_COMPILE_OPTION,$tmpdir);
	}

	$File_list .= Get_VS_2010_File_Group($file_list,$File_group_name);

	$All_Flie_list = "ALL_sources = \\\n";

	foreach my $node1 (sort split("\n",$file_list))
	{
		next if($node1 =~ /\.s$/i);	
		$All_Flie_list .= "$node1 \\\n";
	}

	my $result;
	$result .= $File_group_name;
	$result .= "\$(NULL)\n\n";
	$result .= $File_list."\n";
	$result .= $All_Flie_list;
	$result .= "\$(NULL)\n\n";
	$result .= "ALL_Macro = \\\n";
	$result .= $compile_option."\n";
	$result .= "\$(NULL)\n\n";
	$result .= "ALL_Inc = \\\n";
	$result .= $inc_path;
	$result .= "\$(NULL)\n\n";
	
	return $result;
}

###############################################################################
sub Get_Module_vcxproj
{

	my ($lib,$dep_filename,$compile_option,$inc_path,$file_list,$WIN32_COMPILE_OPTION,$tmpdir,$outdir,$add_link32,$pdbflag) = @_;

	my $result = '';

#  print "dep_filename $dep_filename \n";
#  print "compile_option $compile_option \n";

	$result .= Get_VS2010_File_List($lib,$compile_option ,$inc_path ,  $file_list,$WIN32_COMPILE_OPTION,$tmpdir);

	open F,">$dep_filename" or die "Can't open $dep_filename";
	
	print F $result;
	close F;
}

###############################################################################
sub Get_Module_Dsp
{

	my ($lib,$dep_filename,$compile_option,$inc_path,$file_list,$WIN32_COMPILE_OPTION,$tmpdir,$outdir,$add_link32,$pdbflag) = @_;

	my $result = '';

	if($lib eq $workspace_small)
	{
		$result .= Get_Top_dll($lib);
	}
	else
	{
		$result .= Get_Top($lib);
	}
	my $dll_dir = "../bin";
        $dll_dir =~ s/\//\\/g;
	if($lib eq $workspace_small)
	{
		$result .= Get_Compile_Option_dll($lib,$compile_option,$inc_path,$tmpdir,$dll_dir,$add_link32,$pdbflag);
	}
	else
	{
		$result .= Get_Compile_Option($lib,$compile_option,$inc_path,$tmpdir,$outdir,$pdbflag);
	}

	$result .= Get_File_List($lib,$file_list,$WIN32_COMPILE_OPTION,$tmpdir);

	$result .= "\n# End Target\n";
	$result .= "\n# End Project\n";

	open F,">$dep_filename" or die "Can't open $dep_filename";
	
	print F $result;
	close F;
}






















































































































###############################################################################
sub create_workspace
{
	my $workspace_filename = shift;
	my $ini_filename = shift;
	my $active_project_name = shift;
	my $tempdir = shift;
	my $str = shift;
	my @lib_list = split(" ",$str);
       
	my $result = '';
	$result .= "Microsoft Developer Studio Workspace File, Format Version 6.00\n";
	$result .= "# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!\n";

	open INI_FILE, "<$ini_filename" or die "$ini_filename";

	my $Config_Sec_Name = "";
	my %Config_Hash;
	my @external_lib_list_msdev;


        my $dependence = '';
        my @external_lib_list_kernel;
	while (<INI_FILE>) 
	{
  		next if ((/^\s*$/) || (/^\s*#/));
  		if (/^\s*\[(\S+)\]/)
  		{
			$Config_Sec_Name = uc($1) ;
  		} 
  		elsif (/^\s*(\S+)\s*=\s*(.*)\s*$/)
  		{
			if( $Config_Sec_Name eq "ADD_DSP_MSDEV" || $Config_Sec_Name eq "DEP_DSP" || $Config_Sec_Name eq "ADD_DSP_KERNEL")
			{
				push @external_lib_list_msdev,("$1") if( $Config_Sec_Name eq "ADD_DSP_MSDEV");
				push @external_lib_list_kernel,("$1") if( $Config_Sec_Name eq "ADD_DSP_KERNEL");
    				my $myKey = "${Config_Sec_Name}_" . $1;
    				my $myContent = $2;
    				$myContent =~ s/\"//g; 	
    				$Config_Hash{$myKey} = $myContent;
			}
  		}
  		elsif (/^\s*(\S+)\s*\+=\s*(.*)\s*$/)
  		{
			if( $Config_Sec_Name eq "ADD_DSP_MSDEV" || $Config_Sec_Name eq "DEP_DSP" || $Config_Sec_Name eq "ADD_DSP_KERNEL")
			{
    				my $myKey = "${Config_Sec_Name}_". $1;
    				my $myContent = $2;
    				$myContent =~ s/\"//g; 
    				$Config_Hash{$myKey} = $Config_Hash{$myKey} . " $myContent";
			}
  		}
	}
	close INI_FILE;

	
	foreach my $lib (@lib_list,@external_lib_list_kernel) 
        {
		if($lib ne $workspace_small)
		{
			$dependence .= "\n";
		   	$dependence .= "    Begin Project Dependency\n";
		   	$dependence .= "    Project_Dep_Name $lib\n";
		   	$dependence .= "    End Project Dependency\n";
                }
	}


	foreach my $lib (@lib_list)
        {
		$result .= "\n";
		$result .= "###############################################################################\n";
		$result .= "Project: \"$lib\"=$lib/$lib.dsp - Package Owner=<4>\n";
		$result .= "\n";
		$result .= "Package=<5>\n";
		$result .= "{{{\n";
		$result .= "}}}\n";
		$result .= "\n";
		$result .= "Package=<4>\n";
		$result .= "{{{\n";
                if($lib eq $workspace_small)
                {
			$result .= "$dependence\n";
                }
		else
		{	
			$result .= "\n";
		}
		$result .= "}}}\n";
	}

	foreach my $lib (@external_lib_list_kernel)
        {
		my $dsp_name = $Config_Hash{"ADD_DSP_KERNEL_"."$lib"};
		$dsp_name =~ s/(^\s+|\s+$)//g;
		$dsp_name =~ s/^$lib//g;
		$dsp_name =~ s/\=//g;
		$dsp_name = $tempdir.$dsp_name;
		$result .= "\n";
		$result .= "###############################################################################\n";
		$result .= "Project: \"$lib\"=$dsp_name - Package Owner=<4>\n";
		$result .= "\n";
		$result .= "Package=<5>\n";
		$result .= "{{{\n";
		$result .= "}}}\n";
		$result .= "\n";
		$result .= "Package=<4>\n";
		$result .= "{{{\n";	
                if($Config_Hash{"DEP_DSP_"."$lib"} ne "")
                {
			foreach my $dep_file (split('\s+',$Config_Hash{"DEP_DSP_"."$lib"})) 
    			{
				$result .= "\n";
				$result .= "    Begin Project Dependency\n";
		   		$result .= "    Project_Dep_Name $dep_file\n";
		   		$result .= "    End Project Dependency\n";
			}
                }
		else
		{	
			$result .= "\n";
		}
		$result .= "}}}\n";
	}

	if($active_project_name eq $workspace_big)
	{
	    foreach my $lib (@external_lib_list_msdev)
            {
		my $dsp_name = $Config_Hash{"ADD_DSP_MSDEV_"."$lib"};
		$dsp_name =~ s/(^\s+|\s+$)//g;
		$dsp_name =~ s/^$lib//g;
		$dsp_name =~ s/\=//g;
		$dsp_name = $tempdir.$dsp_name;
		$result .= "\n";
		$result .= "###############################################################################\n";
		$result .= "Project: \"$lib\"=$dsp_name - Package Owner=<4>\n";
		$result .= "\n";
		$result .= "Package=<5>\n";
		$result .= "{{{\n";
		$result .= "}}}\n";
		$result .= "\n";
		$result .= "Package=<4>\n";
		$result .= "{{{\n";
                if($Config_Hash{"DEP_DSP_"."$lib"} ne "")
                {
			foreach my $dep_file (split('\s+',$Config_Hash{"DEP_DSP_"."$lib"})) 
    			{
				$result .= "\n";
				$result .= "    Begin Project Dependency\n";
		   		$result .= "    Project_Dep_Name $dep_file\n";
		   		$result .= "    End Project Dependency\n";
			}
                }
		else
		{	
			$result .= "\n";
		}
		$result .= "}}}\n";
	    }
	}
	open F,">$workspace_filename" or die "$workspace_filename";
	print F $result;
	close F;

}


###############################################################################
sub Files_in_Dir()
{
   my ($fullpath,$tmpfilelist) = @_;
   my @allfiles = split("\n",$tmpfilelist);
   my $Result_str=" ";
  foreach my $file (@allfiles)
  {
  	if($fullpath ne "")
  	{
    	if($file =~/^$fullpath\/([^\/]+)/)
    	{
    		if($Result_str !~/\s+$1\s+/)
    		{
    			$Result_str .= $1." ";
    		} 
    	}
    }
    else
    {
    	if($file =~/^([^\/]+)/)
    	{
    		if($Result_str !~/\s+$1\s+/)
    		{
    			$Result_str .= $1." ";
    		} 
    	}
    }
	}
	$Result_str =~ s/(^\s+)|(\s+)$//g;
	return $Result_str;
}

return 1;
